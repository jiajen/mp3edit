#include "mp3edit/src/reader/tag/id3v2_3.h"

#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Id3v2_3 {
namespace {

const int kTagHeaderLength = 10;

int getPostHeaderSeek(const Bytes& tag) {
  // TODO handle extended header
  return -1;
}

Bytes clearTagUnsync(const Bytes& raw_tag) {
  Bytes new_tag(raw_tag.begin(), raw_tag.begin()+10);
  new_tag.reserve(raw_tag.size());
  for (int i = 10, s = raw_tag.size(); i < s; i++) {
    new_tag.push_back(raw_tag[i]);
    if (i < s-1 && raw_tag[i] == 0xFF && raw_tag[i+1] == 0x00) i++;
  }
  return new_tag;
}

}  // namespace

void parseTag(const Bytes& raw_tag, std::string& title, std::string& artist,
              std::string& album, int track_num, int track_denum) {

  bool has_unsync, has_extended_header;
  int tag_version, unused;

  Bytes header(raw_tag.begin(), raw_tag.begin() + kTagHeaderLength);
  Id3v2::parseTagHeader(header, false, tag_version, unused,
                        has_unsync, has_extended_header);
  if (tag_version != 3) return;

  Bytes tag = (has_unsync) ? clearTagUnsync(raw_tag) : raw_tag;
  int seek = (has_extended_header) ? getPostHeaderSeek(tag) : kTagHeaderLength;

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
