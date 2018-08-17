#include "mp3edit/src/reader/tag/mp3_padding.h"

#include <cstring>

namespace Mp3Edit {
namespace ReaderTag {
namespace Mp3Padding {

namespace {

const char* kSyncBitMask = "\xFF\xE0";
const int kSyncSize = 2;

}  // namespace

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek) {
  Bytes buffer;
  do {
    Filesystem::readBytes(file_stream, seek, kSyncSize, buffer);
    seek += kSyncSize;
    for (int i = 0; i < kSyncSize; i++) buffer[i] &= kSyncBitMask[i];
  } while (strncmp((const char*)buffer.data(), kSyncBitMask, kSyncSize) != 0);
  return seek - kSyncSize;
}

}  // Mp3Padding
}  // ReaderTag
}  // Mp3Edit
