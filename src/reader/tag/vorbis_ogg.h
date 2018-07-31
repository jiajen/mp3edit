#ifndef MP3EDIT_SRC_READER_TAG_VORBIS_OGG_H_
#define MP3EDIT_SRC_READER_TAG_VORBIS_OGG_H_

#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisOgg {

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek);

int seekFooterStart(Filesystem::FileStream&, int seek);

}  // namespace VorbisOgg
}  // namespace ReaderTag
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_READER_TAG_VORBIS_OGG_H_
