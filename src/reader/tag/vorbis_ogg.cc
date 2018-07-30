#include "mp3edit/src/reader/tag/vorbis_ogg.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisOgg {

namespace {

using Filesystem::readBytes;

}  // namespace

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek) {
  // TODO
  return seek;
}

int seekFooterStart(Filesystem::FileStream& file_stream, int seek) {
  return seek;
}

}  // namespace VorbisOgg
}  // namespace ReaderTag
}  // namespace Mp3Edit
