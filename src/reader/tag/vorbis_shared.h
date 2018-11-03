#ifndef MP3EDIT_READER_TAG_VORBIS_SHARED_H_
#define MP3EDIT_READER_TAG_VORBIS_SHARED_H_

#include <string>

#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisShared {

// Returns -1 on fail, else return number of bytes read.
int parseTag(const Bytes& tag, int seek,
             bool has_framing_bit, bool remove_padding);
int parseTag(const Bytes& tag, int seek,
             bool has_framing_bit, bool remove_padding,
             std::string& title, std::string& artist, std::string& album,
             int& track_num, int& track_denum);

Bytes generateTag(const std::string& title, const std::string& artist,
                  const std::string& album, int track_num, int track_denum,
                  bool has_framing_bit);

}  // namespace VorbisShared
}  // namespace ReaderTag
}  // namespace Mp3Edit

#endif  // MP3EDIT_READER_TAG_VORBIS_SHARED_H_
