#ifndef MP3EDIT_SRC_READER_TAG_ID3V1_H_
#define MP3EDIT_SRC_READER_TAG_ID3V1_H_

#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Id3v1 {

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek);

int seekFooterStart(Filesystem::FileStream& file_stream, int seek);

}  // Id3v1
}  // ReaderTag
}  // Mp3Edit

#endif  // MP3EDIT_SRC_READER_TAG_ID3V1_H_
