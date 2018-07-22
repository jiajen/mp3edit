#include "mp3edit/src/reader/tag/vorbis.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Vorbis {

namespace {

using Filesystem::readBytes;

}  // namespace

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek) {
  // TODO
  return seek;
}

int seekFooterStart(Filesystem::FileStream& file_stream, int seek) {
  // TODO
  return seek;
}

}  // namespace Vorbis
}  // namespace ReaderTag
}  // namespace Mp3Edit
