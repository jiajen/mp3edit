#ifndef MP3EDIT_SRC_READER_TAG_VORBIS_FLAC_H_
#define MP3EDIT_SRC_READER_TAG_VORBIS_FLAC_H_

#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisFlac {

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek);

int seekFooterStart(Filesystem::FileStream& file_stream, int seek);

}  // namespace VorbisFlac
}  // namespace ReaderTag
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_READER_TAG_VORBIS_FLAC_H_
