#include "mp3edit/src/file.h"

#include <cstring>

#include "mp3edit/src/reader/tag/ape.h"
#include "mp3edit/src/reader/tag/id3v1.h"
#include "mp3edit/src/reader/tag/id3v2.h"
#include "mp3edit/src/reader/tag/id3v2_3.h"
#include "mp3edit/src/reader/tag/lyrics3.h"
#include "mp3edit/src/reader/tag/vorbis.h"

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
  int n = (int)sizeof(sizeof(kFileSupportedFileTypes)/
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
    filepath_(filepath), filetype_(filetype), filesize_(0), is_valid_(true) {
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
  int seek_start = 0, seek_end = filesize_, seek;
  do {
    audio_start_ = seek_start;
    audio_end_ = seek_end;

    using namespace ReaderTag;

    seek = Id3v2::seekHeaderEnd(file_stream, seek_start);
    if (seek != seek_start) {
      // TODO parse id3v2.3
      seek_start = seek;
    }

    // TODO Uncomment completed functions
    // seek_start = Ape::seekHeaderEnd(file_stream, seek_start);
    // seek_start = Vorbis::seekHeaderEnd(file_stream, seek_start);
    seek_start = Id3v1::seekHeaderEnd(file_stream, seek_start);
    // seek_start = Lyrics3::seekHeaderEnd(file_stream, seek_start);

    seek_end = Id3v1::seekFooterStart(file_stream, seek_end);
    // seek_end = Lyrics3::seekFooterStart(file_stream, seek_end);
    seek_end = Id3v2::seekFooterStart(file_stream, seek_end);
    // seek_end = Ape::seekFooterStart(file_stream, seek_end);
    // seek_end = Vorbis::seekFooterStart(file_stream, seek_end);

  } while (seek_start != audio_start_ || seek_end != audio_end_);
}

void File::readAudioData(Filesystem::FileStream& file_stream) {
  // TODO Read audio
}

Bytes File::generateMetadataFront() {
  // TODO generate proper metadata using meeber data and based on filetpe
  return Bytes();
}

Bytes File::generateMetadataBack() {
  // TODO generate proper metadata using meeber data and based on filetpe
  return Bytes();
}

void File::updateMetadataFromId3v2Tag() {

}

void File::updateMetadataFromVorbis() {

}

}  // namespace File
}  // namespace Mp3Edit
