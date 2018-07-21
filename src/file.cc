#include "mp3edit/src/file.h"

#include <cstring>

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
    // TODO check whether tellg gives end of file consistently

    readMetaData(file_stream);
    if (read_audio_data) readAudioData(file_stream);
  } catch (const std::exception&) {
    is_valid_ = false;
  }
  file_stream.close();
}

void File::readMetaData(Filesystem::FileStream &file_stream) {
  // TODO read props
  // TODO loop all file readers until -1 is returned for all then only proceed
  // if id3v2 then scan and parse
}

void File::readAudioData(Filesystem::FileStream &file_stream) {
  // TODO Read audio
}

Bytes File::generateMetadataFront() {
  // TODO generate proper metadata using meeber data and based on filetpe
}

Bytes File::generateMetadataBack() {
  // TODO generate proper metadata using meeber data and based on filetpe
}

void File::saveFileChanges() {
  // TODO only overwrite if metadata changed
}

}  // namespace File
}  // namespace Mp3Edit
