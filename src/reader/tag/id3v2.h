#ifndef MP3EDIT_SRC_READER_TAG_ID3V2_H_
#define MP3EDIT_SRC_READER_TAG_ID3V2_H_

#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Id3v2 {

// TODO shared functions with 2_3

int seekHeaderEnd(Filesystem::FileStream &file_stream, int seek);

int seekFooterStart(Filesystem::FileStream &file_stream, int seek);

}  // Id3v2
}  // ReaderTag
}  // Mp3Edit

#endif  // MP3EDIT_SRC_READER_TAG_ID3V2_H_
