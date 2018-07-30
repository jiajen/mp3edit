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

// Checks the validity of an ogg file's first page
// and the second page's header.
bool verifyValidOggHeaderPrefix() {

}

}  // namespace

// Warning: Limited functionality. Although the official specification states
// that an OGG structure can have its vorbis comment span more than a single
// page, this function assumes that the vorbis comment is entirely encapsulated
// in the second page of an OGG file. This is done to simplify the process of
// saving the OGG file as this particular case only requires overwriting the
// second page (merely requiring adjustments to the size and checksum in the
// header of the second page).
// Future Improvements: Allow reading of vorbis comments spanning more than
// one page.
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
