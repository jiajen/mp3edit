#include "mp3edit/src/file.h"

#include <cstring>

#include <exception>
#include <system_error>
#include <filesystem>

#include "mp3edit/src/filesystem.h"
#include "mp3edit/src/sanitiser.h"
#include "mp3edit/src/reader/tag/ape.h"
#include "mp3edit/src/reader/tag/id3v1.h"
#include "mp3edit/src/reader/tag/id3v2.h"
#include "mp3edit/src/reader/tag/id3v2_3.h"
#include "mp3edit/src/reader/tag/lyrics3.h"
#include "mp3edit/src/reader/tag/vorbis_flac.h"
#include "mp3edit/src/reader/tag/vorbis_ogg.h"

namespace Mp3Edit {
namespace File {

namespace {

// Must follow order in FileType
const char* kFileSupportedFileTypes[] = {
  ".mp3",
  ".flac",
  ".ogg"
};

}  // namespace

FileType getAudioExtension(const std::string& filename) {
  int n = (int)(sizeof(kFileSupportedFileTypes)/
                sizeof(kFileSupportedFileTypes[0]));

  for (int i = 0; i < n; i++) {
    int len = strlen(kFileSupportedFileTypes[i]);
    int extensionIdx = filename.length() - len;
    if (extensionIdx < 0) continue;
    if (strcmp(kFileSupportedFileTypes[i], filename.c_str()+extensionIdx) == 0)
      return (FileType)i;
  }
  return FileType::kInvalid;
}

File::File(const std::string& filepath, FileType filetype,
           bool read_audio_data):
    track_num_(-1), track_denum_(-1), filepath_(filepath),
    filetype_(filetype), filesize_(0), file_container_start_seek_(0),
    is_valid_(true) {
  Filesystem::FileStream file_stream(filepath_,
                                     std::ios::in | std::ifstream::binary);
  try {
    if (!file_stream) {
      is_valid_ = false;
      return;
    }

    file_stream.seekg(0, file_stream.end);
    filesize_ = file_stream.tellg();

    readMetaData(file_stream);
    if (read_audio_data) readAudioData(file_stream);
  } catch (const std::exception&) {
    is_valid_ = false;
  }
  file_stream.close();
}

void File::saveFileChanges(bool rename_file) {
  using Filesystem::readBytes;
  Filesystem::FileStream file_stream(filepath_, std::ios::in |
                                                std::ifstream::binary);
  if (!file_stream) {
    throw std::system_error(std::error_code(), "Error accessing file.");
  }

  Bytes metadata_front, metadata_back, audio_raw;
  try {
    switch (filetype_) {
      using namespace ReaderTag;
      case FileType::kMp3:
        metadata_front = Id3v2_3::generateTag(title_, artist_, album_,
                                              track_num_, track_denum_);
        metadata_back = Id3v1::generateTag(title_, artist_, album_,
                                           track_num_, track_denum_);
        break;
      case FileType::kFlac:
        metadata_front = VorbisFlac::generateTag(file_stream,
                                                 file_container_start_seek_,
                                                 title_, artist_, album_,
                                                 track_num_, track_denum_);
        break;
      case FileType::kOgg:
        metadata_front = VorbisOgg::generateTag(file_stream,
                                                file_container_start_seek_,
                                                audio_start_,
                                                title_, artist_, album_,
                                                track_num_, track_denum_);
        break;
      default:
        break;
    }

    if ((int)metadata_front.size() == audio_start_ &&
        filesize_ - (int)metadata_back.size() == audio_end_) {
      Bytes front_block, back_block;
      readBytes(file_stream, 0, metadata_front.size(), front_block);
      readBytes(file_stream, filesize_ - metadata_back.size(),
                metadata_back.size(), back_block);
      if (front_block == metadata_front && back_block == metadata_back) {
        file_stream.close();
        return;
      }
    }

    readBytes(file_stream, audio_start_, audio_end_ - audio_start_, audio_raw);
    file_stream.close();
  } catch (const std::exception& ex) {
    file_stream.close();
    throw ex;
  }
  if (!writeFile(audio_raw, metadata_front, metadata_back,
                 rename_file ?  Sanitiser::toValidFilename(title_) : ""))
    throw std::system_error(std::error_code(), "Unable to write.");
}

bool File::writeFile(const Bytes& audio_raw,
                     const Bytes& metadata_front, const Bytes& metadata_back,
                     const std::string& new_filename) {
  std::filesystem::path current_path = filepath_;
  std::filesystem::path target_path = filepath_;
  if (!new_filename.empty()) {
    target_path.replace_filename(new_filename);
    target_path.replace_extension(kFileSupportedFileTypes[(int)filetype_]);
  }

  if (target_path != current_path) {
    for (int i = 2; std::filesystem::exists(target_path); i++) {
      target_path.replace_filename(new_filename +
                                   " (" + std::to_string(i) + ")");
      target_path.replace_extension(kFileSupportedFileTypes[(int)filetype_]);
    }
  }

  std::filesystem::path tmp_path = std::filesystem::temp_directory_path() /
                                   target_path.filename();

  for (int i = 0; std::filesystem::exists(tmp_path); i++) {
    tmp_path.replace_filename(new_filename + "-" + std::to_string(i));
    tmp_path.replace_extension(kFileSupportedFileTypes[(int)filetype_]);
  }

  Filesystem::FileWriter file_writer(tmp_path);
  file_writer.write(metadata_front);
  file_writer.write(audio_raw);
  file_writer.write(metadata_back);
  file_writer.close();

  try {
    if (std::filesystem::exists(target_path)) {
      std::filesystem::remove(target_path);
    }
    std::filesystem::copy_file(tmp_path, target_path);
  } catch(const std::exception& ex) {
    throw ex;
  }

  std::filesystem::remove(tmp_path);
  if (target_path != current_path) std::filesystem::remove(current_path);

  return true;
}

void File::readMetaData(Filesystem::FileStream& file_stream) {
  using namespace ReaderTag;
  int seek_start = 0, seek_end = filesize_, seek;

  do {
    audio_start_ = seek_start;
    seek = Id3v2::seekHeaderEnd(file_stream, seek_start);
    if (seek != seek_start) {
      if (filetype_ == FileType::kMp3) {
        Id3v2_3::parseTag(Id3v2_3::extractTag(file_stream, seek_start, seek),
                          title_, artist_, album_, track_num_, track_denum_);
      }
      seek_start = seek;
    }
    seek_start = Ape::seekHeaderEnd(file_stream, seek_start);
  } while (seek_start != audio_start_);

  switch (filetype_) {
    case FileType::kFlac:
      seek = VorbisFlac::seekHeaderEnd(file_stream, audio_start_);
      if (seek != audio_start_) {
        using VorbisFlac::parseTag;
        using VorbisFlac::extractTag;
        file_container_start_seek_ = audio_start_;
        parseTag(extractTag(file_stream, audio_start_, seek),
                 title_, artist_, album_, track_num_, track_denum_);
        audio_start_ = seek;
      } else {
        throw std::system_error(std::error_code(), "Invalid FLAC.");
      }
      break;
    case FileType::kOgg:
      seek = VorbisOgg::seekHeaderEnd(file_stream, audio_start_);
      if (seek != audio_start_) {
        using VorbisOgg::parseTag;
        using VorbisOgg::extractTag;
        file_container_start_seek_ = audio_start_;
        parseTag(extractTag(file_stream, audio_start_, seek),
                 title_, artist_, album_, track_num_, track_denum_);
        audio_start_ = seek;
      } else {
        throw std::system_error(std::error_code(), "Invalid OGG.");
      }
      break;
    default:
      break;
  }

  do {
    audio_end_ = seek_end;
    seek = Id3v1::seekFooterStart(file_stream, seek_end);
    if (seek != seek_end) {
      if (filetype_ == FileType::kMp3) {
        Id3v1::parseTag(Id3v1::extractTag(file_stream, seek, seek_end),
                        title_, artist_, album_, track_num_, track_denum_);
      }
      seek_end = seek;
    }
    seek_end = Lyrics3::seekFooterStart(file_stream, seek_end);
    seek_end = Id3v2::seekFooterStart(file_stream, seek_end);
    seek_end = Ape::seekFooterStart(file_stream, seek_end);
  } while (seek_end != audio_end_);
}

void File::readAudioData(Filesystem::FileStream&) {
  // TODO Read audio
}

}  // namespace File
}  // namespace Mp3Edit
