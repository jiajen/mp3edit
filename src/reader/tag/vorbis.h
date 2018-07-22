#ifndef MP3EDIT_SRC_READER_TAG_VORBIS_H_
#define MP3EDIT_SRC_READER_TAG_VORBIS_H_

#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Vorbis {

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek);

int seekFooterStart(Filesystem::FileStream& file_stream, int seek);

}  // namespace Vorbis
}  // namespace ReaderTag
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_READER_TAG_VORBIS_H_
