#include "mp3edit/src/reader/tag/vorbis_ogg.h"

#include <cstring>

#include <exception>

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisOgg {

namespace {

using Filesystem::readBytes;

const int kFirstPageLength = 58;
const int kPageHeaderPrefixLength = 27;

}  // namespace

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek) {
  Bytes header;
  readBytes(file_stream, seek, kFirstPageLength + kPageHeaderPrefixLength,
            header);
  // TODO
  return seek;
}

int seekFooterStart(Filesystem::FileStream&, int seek) {
  return seek;
}

}  // namespace VorbisOgg
}  // namespace ReaderTag
}  // namespace Mp3Edit
