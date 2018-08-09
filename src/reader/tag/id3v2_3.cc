#include "mp3edit/src/reader/tag/id3v2_3.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Id3v2_3 {

void parseTag(const Bytes& tag, std::string& title, std::string& artist,
              std::string& album, int track_num, int track_denum) {
  // TODO output data from tag
}

Bytes extractTag(Filesystem::FileStream& file_stream,
                 int seek_tag_start, int seek_tag_end) {
  // TODO separate tag from seek_start and old seek_start
}

Bytes Tag::generateTagHeader() {
  // TODO generate id3v2.3 tag purely
  return Bytes();
}

}  // Id3v2
}  // ReaderTag
}  // Mp3Edit
