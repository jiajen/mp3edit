#include "mp3edit/src/reader/tag/id3v1.h"

#include <cstring>

namespace Mp3Edit {
namespace ReaderTag {
namespace Id3v1 {

namespace {

using Filesystem::readBytes;

const int kTagHeaderLength = 3;
const int kTagLength = 128;

const int kEnhancedTagHeaderLength = 4;
const int kEnhancedTagLength = 227;

}  // namespace

void parseTag(const Bytes& tag, std::string& title, std::string& artist,
              std::string& album, int& track_num, int& track_denum) {
  // TODO
}

Bytes extractTag(Filesystem::FileStream& file_stream,
                 int seek_tag_start, int seek_tag_end) {
  // TODO
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
