#include "mp3edit/src/reader/tag/vorbis_flac.h"

#include <cstring>

#include <exception>

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisFlac {

namespace {

using Filesystem::readBytes;

const int kPreambleLength = 4;
const int kBlockHeaderLength = 4;
const int kBlockTypePos = 0;
const int kBlockFlagLastBlockBitPos = 7;
const int kBlockTypeVorbisBitMask = 4;
const int kBlockSizeStartPos = 1;
const int kBlockSizeSize = 3;

int toInt(Bytes::const_iterator it_begin, Bytes::const_iterator it_end) {
  int size = 0, jmp = 0;
  while (--it_end >= it_begin) {
    size += (((int)(*it_end)) << jmp);
    jmp += 8;
  }
  return size;
}

}  // namespace

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek) {
  Bytes header;
  readBytes(file_stream, seek, kPreambleLength, header);

  if (strncmp((const char*)header.data(), "fLaC", kPreambleLength) != 0)
    return seek;

  seek += kPreambleLength;
  int size;
  do {
    readBytes(file_stream, seek, kBlockHeaderLength, header);
    if ((header[kBlockTypePos]&0x7F) == 0x7F)
      throw std::system_error(std::error_code(), "Invalid FLAC.");

    size = kBlockHeaderLength +
           toInt(header.begin() + kBlockSizeStartPos,
                 header.begin() + kBlockSizeStartPos + kBlockSizeSize);
    seek += size;
  } while (!(header[kBlockTypePos]&(1<<kBlockFlagLastBlockBitPos)));

  return seek;
}

int seekFooterStart(Filesystem::FileStream&, int seek) {
  return seek;
}

}  // namespace VorbisFlac
}  // namespace ReaderTag
}  // namespace Mp3Edit
