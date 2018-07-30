#include "mp3edit/src/reader/tag/vorbis_flac.h"

#include <cstring>

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

}  // namespace

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek) {
  // TODO
  return seek;
}

int seekFooterStart(Filesystem::FileStream&, int seek) {
  return seek;
}

}  // namespace VorbisFlac
}  // namespace ReaderTag
}  // namespace Mp3Edit
