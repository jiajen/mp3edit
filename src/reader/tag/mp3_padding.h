#ifndef MP3EDIT_SRC_READER_TAG_MP3_PADDING_H_
#define MP3EDIT_SRC_READER_TAG_MP3_PADDING_H_

#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Mp3Padding {

// This searches until the first instance of the sync byte which
// indicates the start of the first MP3 frame.
int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek);

}  // Mp3Padding
}  // ReaderTag
}  // Mp3Edit

#endif  // MP3EDIT_SRC_READER_TAG_MP3_PADDING_H_
