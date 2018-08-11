#ifndef MP3EDIT_SRC_READER_TAG_VORBIS_FLAC_H_
#define MP3EDIT_SRC_READER_TAG_VORBIS_FLAC_H_

#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisFlac {

void parseTag(const Bytes& tag, std::string& title, std::string& artist,
              std::string& album, int& track_num, int& track_denum);

// Returns the metadata block corresponding to a vorbis comment.
Bytes extractTag(Filesystem::FileStream& file_stream,
                 int seek_tag_start, int);

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek);

int seekFooterStart(Filesystem::FileStream&, int seek);

Bytes generateTag(const std::string& title, const std::string& artist,
                  const std::string& album, int track_num, int track_denum);

}  // namespace VorbisFlac
}  // namespace ReaderTag
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_READER_TAG_VORBIS_FLAC_H_
