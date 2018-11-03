#ifndef MP3EDIT_READER_TAG_APE_H_
#define MP3EDIT_READER_TAG_APE_H_

#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Ape {

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek);

int seekFooterStart(Filesystem::FileStream& file_stream, int seek);

}  // namespace Ape
}  // namespace ReaderTag
}  // namespace Mp3Edit

#endif  // MP3EDIT_READER_TAG_APE_H_
