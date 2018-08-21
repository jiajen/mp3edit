#include "mp3edit/src/reader/tag/mp3_padding.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Mp3Padding {

namespace {

const unsigned char kSyncMask0 = '\xFF';
const unsigned char kSyncMask1 = '\xE0';

inline bool checkIsValidSync(unsigned char& old_byte, const Bytes& buffer,
                             int& seek) {
  if (old_byte == kSyncMask0 && (buffer[0]&kSyncMask1) == kSyncMask1)
    return true;
  old_byte = buffer[0];
  seek++;
  return false;
}

}  // namespace

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek) {
  unsigned char old_byte;
  Bytes buffer;
  Filesystem::readBytes(file_stream, seek, 1, buffer);
  old_byte = buffer[0];
  try {
    do {
      Filesystem::readBytes(file_stream, seek+1, 1, buffer);
    } while (!checkIsValidSync(old_byte, buffer, seek));
  } catch (const std::exception& ex) {
    throw std::system_error(std::error_code(),
                            "Unable to locate a valid MP3 Frame.");
  }
  return seek;
}

}  // Mp3Padding
}  // ReaderTag
}  // Mp3Edit
