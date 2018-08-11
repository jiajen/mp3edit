#ifndef MP3EDIT_SRC_READER_TAG_VORBIS_OGG_H_
#define MP3EDIT_SRC_READER_TAG_VORBIS_OGG_H_

#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisOgg {

void parseTag(const Bytes& tag, std::string& title, std::string& artist,
              std::string& album, int& track_num, int& track_denum);

// Returns the metadata tag portion of an audio file.
Bytes extractTag(Filesystem::FileStream& file_stream,
                 int seek_tag_start, int seek_tag_end);

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek);

int seekFooterStart(Filesystem::FileStream&, int seek);

}  // namespace VorbisOgg
}  // namespace ReaderTag
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_READER_TAG_VORBIS_OGG_H_
