#include "mp3edit/src/reader/tag/lyrics3.h"

#include <cstring>

namespace Mp3Edit {
namespace ReaderTag {
namespace Lyrics3 {

namespace {

using Filesystem::readBytes;

const int kTagFooterLength = 15;
const int kTagPreamblePos = 6;
const int kTagMaxSizeV1 = 5111;  // 5100 + 11 (LYRICSBEGIN)

// Returns -1 if no header is found for the Lyrics3v1 tag.
// Otherwise returns the size of the Lyrics3v1 tag including LYRICSBEGIN
// (but not including LYRICSEND).
int getLyrics3v1Size(const Bytes& footer) {
  // TODO look for LYRICSBEGIN
  return -1;
}

// Returns -1 if an invalid size is found.
int getLyrics3v2Size(const Bytes& footer_size) {
  // TODO
  return 0;
}

}  // namespace

int seekHeaderEnd(Filesystem::FileStream&, int seek) {
  return seek;
}

int seekFooterStart(Filesystem::FileStream& file_stream, int seek) {
  Bytes footer;
  readBytes(file_stream, seek - kTagFooterLength, kTagFooterLength, footer);

  const char* footer_preamble = (const char*)(footer.data()+kTagPreamblePos);
  if (strncmp(footer_preamble, "LYRICS200", 9) == 0) {
    // Version 2
    // TODO
  } else if (strncmp(footer_preamble, "LYRICSEND", 9) == 0) {
    // Version 1 (Obsolete)
    readBytes(file_stream, seek - kTagMaxSizeV1 - 9, kTagMaxSizeV1, footer);
    int lyrics_size = getLyrics3v1Size(footer);
    if (lyrics_size != -1) seek -= (9 + lyrics_size);
  }

  return seek;
}

}  // namespace Lyrics3
}  // namespace ReaderTag
}  // namespace Mp3Edit
