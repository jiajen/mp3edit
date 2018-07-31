#include "mp3edit/src/reader/tag/vorbis_shared.h"

#include "mp3edit/src/reader/utility.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisShared {

namespace {

using Filesystem::readBytes;

}  // namespace

int parseTag(const Bytes& tag, int seek, bool has_framing_bit) {
  std::string unused_s1, unused_s2, unused_s3;
  int unused_i1, unused_i2;
  return parseTag(tag, seek, has_framing_bit,
                  unused_s1, unused_s2, unused_s3, unused_i1, unused_i2);
}

int parseTag(const Bytes& tag, int seek, bool has_framing_bit,
             std::string& title, std::string& artist, std::string& album,
             int& track_num, int& track_denum) {

  // int vender_length = lEndianToInt(tag.begin()+seek, );
  // TODO return size of scanned vorbis comment tag
  return -1;
}

}  // namespace VorbisShared
}  // namespace ReaderTag
}  // namespace Mp3Edit
