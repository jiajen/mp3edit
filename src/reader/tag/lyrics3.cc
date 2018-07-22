#include "mp3edit/src/reader/tag/lyrics3.h"

#include <cstring>

namespace Mp3Edit {
namespace ReaderTag {
namespace Lyrics3 {

namespace {

using Filesystem::readBytes;

}  // namespace

int seekHeaderEnd(Filesystem::FileStream&, int seek) {
  return seek;
}

int seekFooterStart(Filesystem::FileStream& file_stream, int seek) {
  // TODO
  return seek;
}

}  // namespace Lyrics3
}  // namespace ReaderTag
}  // namespace Mp3Edit
