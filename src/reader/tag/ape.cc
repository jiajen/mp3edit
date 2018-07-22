#include "mp3edit/src/reader/tag/ape.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Ape {

namespace {

using Filesystem::readBytes;

const int kTagHeaderLength = 32;
const int kTagSizeStartPos = 12;
const int kTagSizeEndPos = 15;

// Must be zero
const int kTagReservedStartPos = 24;
const int kTagReservedStartPos = 31;

}  // namespace

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek) {

}

int seekFooterStart(Filesystem::FileStream& file_stream, int seek) {

}

}  // namespace Ape
}  // namespace ReaderTag
}  // namespace Mp3Edit