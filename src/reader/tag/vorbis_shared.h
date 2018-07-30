#ifndef MP3EDIT_SRC_READER_TAG_VORBIS_SHARED_H_
#define MP3EDIT_SRC_READER_TAG_VORBIS_SHARED_H_

#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisShared {

// Returns -1 on fail, else return number of bytes read.
int parseTag(const Bytes& tag, int seek);
int parseTag(const Bytes& tag, int seek,
             std::string& title, std::string& artist, std::string& album,
             int& track_num, int& track_denum);

}  // namespace VorbisShared
}  // namespace ReaderTag
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_READER_TAG_VORBIS_SHARED_H_
