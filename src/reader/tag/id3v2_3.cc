#include "mp3edit/src/reader/tag/id3v2_3.h"

#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Id3v2_3 {

void parseTag(const Bytes& tag, std::string& title, std::string& artist,
              std::string& album, int track_num, int track_denum) {
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
