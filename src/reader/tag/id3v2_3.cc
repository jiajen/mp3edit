#include "mp3edit/src/reader/tag/id3v2_3.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Id3v2_3 {

Tag::Tag(Filesystem::FileStream &file_stream, int seek_start, int seek_end) {
  // TODO parse
}

Bytes Tag::generateTagHeader() {
  // TODO generate id3v2.3 tag purely
}

}  // Id3v2
}  // ReaderTag
}  // Mp3Edit
