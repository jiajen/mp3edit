#ifndef MP3EDIT_SRC_READER_TAG_ID3V1_H_
#define MP3EDIT_SRC_READER_TAG_ID3V1_H_

#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Id3v1 {

int seekFooterStart(Filesystem::FileStream& file_stream, int seek);

Bytes extractTag(Filesystem::FileStream& file_stream,
                 int seek_tag_start, int seek_tag_end);

void parseTag(const Bytes& tag, std::string& title, std::string& artist,
              std::string& album, int& track_num, int& track_denum);

// Returns the entire tag
Bytes generateTag(const std::string& title, const std::string& artist,
                  const std::string& album, int track_num, int);

}  // Id3v1
}  // ReaderTag
}  // Mp3Edit

#endif  // MP3EDIT_SRC_READER_TAG_ID3V1_H_
