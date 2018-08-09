#ifndef MP3EDIT_SRC_READER_TAG_ID3V2_3_H_
#define MP3EDIT_SRC_READER_TAG_ID3V2_3_H_

#include <string>

#include "mp3edit/src/bytes.h"
#include "mp3edit/src/filesystem.h"
#include "mp3edit/src/reader/tag/id3v2.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Id3v2_3 {

void parseTag(const Bytes& tag, std::string& title, std::string& artist,
              std::string& album, int& track_num, int& track_denum);

// Returns the metadata tag portion of an audio file.
Bytes extractTag(Filesystem::FileStream& file_stream,
                 int seek_tag_start, int seek_tag_end);

}  // Id3v2_3
}  // ReaderTag
}  // Mp3Edit

#endif  // MP3EDIT_SRC_READER_TAG_ID3V2_3_H_
