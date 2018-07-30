#include "mp3edit/src/reader/tag/vorbis_flac.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisFlac {

namespace {

using Filesystem::readBytes;

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
