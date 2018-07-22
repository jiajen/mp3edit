#ifndef MP3EDIT_SRC_READER_TAG_LYRICS3_H_
#define MP3EDIT_SRC_READER_TAG_LYRICS3_H_

#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Lyrics3 {

int seekHeaderEnd(Filesystem::FileStream&, int seek);

int seekFooterStart(Filesystem::FileStream& file_stream, int seek);

}  // namespace Lyrics3
}  // namespace ReaderTag
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_READER_TAG_LYRICS3_H_
