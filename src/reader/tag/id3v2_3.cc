#include "mp3edit/src/reader/tag/id3v2_3.h"

#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Id3v2_3 {
namespace {

const int kTagHeaderLength = 10;

int getPostHeaderSeek(const Bytes& tag, bool has_extended_header) {
  // TODO handle extended header
  return -1;
}

}  // namespace

void parseTag(const Bytes& tag, std::string& title, std::string& artist,
              std::string& album, int track_num, int track_denum) {
  bool has_unsync, has_extended_header;
  int tag_version, unused;

  Bytes header(tag.begin(), tag.begin() + kTagHeaderLength);
  Id3v2::parseTagHeader(header, false, tag_version, unused,
                        has_unsync, has_extended_header);
  if (tag_version != 3) return;


  // TODO output data from tag
}

Bytes extractTag(Filesystem::FileStream& file_stream,
                 int seek_tag_start, int seek_tag_end) {
  using Filesystem::readBytes;
  Bytes tag;
  readBytes(file_stream, seek_tag_start, seek_tag_end - seek_tag_start, tag);
  return tag;
}

}  // Id3v2
}  // ReaderTag
}  // Mp3Edit
