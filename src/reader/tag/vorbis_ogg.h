#ifndef MP3EDIT_READER_TAG_VORBIS_OGG_H_
#define MP3EDIT_READER_TAG_VORBIS_OGG_H_

#include <string>

#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisOgg {

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek);

// Returns the entire vorbis second page
// (with 0x03 and vorbis but without Ogg encapsulation).
Bytes extractTag(Filesystem::FileStream& file_stream,
                 int seek_tag_start, int);

// Returns false on failure.
bool parseTag(const Bytes& tag, std::string& title, std::string& artist,
              std::string& album, int& track_num, int& track_denum);

Bytes generateTag(Filesystem::FileStream& file_stream,
                  int seek_ogg_start, int seek_audio_start,
                  const std::string& title, const std::string& artist,
                  const std::string& album, int track_num, int track_denum);

}  // namespace VorbisOgg
}  // namespace ReaderTag
}  // namespace Mp3Edit

#endif  // MP3EDIT_READER_TAG_VORBIS_OGG_H_
