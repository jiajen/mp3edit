#include "mp3edit/src/file.h"

#include <cstring>

#include <exception>
#include <system_error>

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
    filetype_(filetype), filesize_(0), is_valid_(true) {
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

void File::saveFileChanges() {
  // TODO only overwrite if metadata changed
  // (if same size then only read raw metadata from file to compare)
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
