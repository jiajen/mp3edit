#include "mp3edit/src/reader/tag/id3v1.h"

#include <cstring>

#include "mp3edit/src/sanitiser.h"
#include "mp3edit/src/reader/utility.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Id3v1 {

namespace {

using Filesystem::readBytes;

const int kTagHeaderLength = 3;
const int kTagLength = 128;

const int kEnhancedTagHeaderLength = 4;
const int kEnhancedTagLength = 227;

const int kTagTitlePos = 3;
const int kTagArtistPos = 33;
const int kTagAlbumPos = 63;
const int kTagTrackPos = 126;
const int kTagFieldSize = 30;

const int kEnhancedTagTitlePos = 4;
const int kEnhancedTagArtistPos = 64;
const int kEnhancedTagAlbumPos = 124;
const int kEnhancedTagFieldSize = 60;

using Reader::Utility::bytesToString;

}  // namespace

void parseTag(const Bytes& tag, std::string& title, std::string& artist,
              std::string& album, int& track_num, int& track_denum) {
  std::string parsed_title;
  std::string parsed_artist;
  std::string parsed_album;
  int parsed_track_num;

  // TODO
}

Bytes extractTag(Filesystem::FileStream& file_stream,
                 int seek_tag_start, int seek_tag_end) {
  Bytes tag;
  readBytes(file_stream, seek_tag_start, seek_tag_end - seek_tag_start, tag);
  return tag;
}

Bytes generateTag(const std::string& title, const std::string& artist,
                  const std::string& album, int track_num, int track_denum) {
  // TODO
}

int seekHeaderEnd(Filesystem::FileStream&, int seek) {
  return seek;
}

int seekFooterStart(Filesystem::FileStream& file_stream, int seek) {
  typedef const char cc;
  Bytes header;

  readBytes(file_stream, seek - kTagLength, kTagHeaderLength, header);
  if (strncmp((cc*)header.data(), "TAG", kTagHeaderLength) != 0) return seek;
  seek -= kTagLength;

  readBytes(file_stream, seek - kEnhancedTagLength,
            kEnhancedTagHeaderLength, header);
  if (strncmp((cc*)header.data(), "TAG+", kEnhancedTagHeaderLength) == 0)
    seek -= kEnhancedTagLength;

  return seek;
}

}  // Id3v1
}  // ReaderTag
}  // Mp3Edit
