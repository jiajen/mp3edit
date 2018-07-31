#include "mp3edit/src/reader/tag/vorbis_shared.h"

#include <exception>

#include "mp3edit/src/reader/utility.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisShared {

namespace {

using Filesystem::readBytes;

const int kVendorLengthSize = 4;

// This class throws an exception when it reaches
class SafeReader {
 public:
  SafeReader(const Bytes& tag, int seek) :
      tag_(tag), tag_size_(tag.size()), seek_start_(seek), seek_end_(seek) {}
 private:
  const Bytes& tag_;
  int tag_size_;
  int seek_start_;
  int seek_end_;
};

class SafeReaderException: public std::exception {
  virtual const char* what() const throw() {
    return "Reached the end of byte array.";
  }
};

/*int lEndianToIntSafe(Bytes::const_iterator it_begin,
                     Bytes::const_iterator it_end) {
  using Reader::Utility::lEndianToInt;
  return lEndianToInt(it_begin, it_end, false);
}*/

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


  int end_seek = seek + kVendorLengthSize;
  /*int vender_length = lEndianToInt(tag.begin() + seek,
                                   tag.begin() + end_seek,
                                   false);
*/
  // TODO return size of scanned vorbis comment tag
  return -1;
}

}  // namespace VorbisShared
}  // namespace ReaderTag
}  // namespace Mp3Edit
