#include "mp3edit/src/reader/tag/vorbis_shared.h"

#include <exception>

#include "mp3edit/src/reader/utility.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisShared {

namespace {

using Filesystem::readBytes;

const int kVendorLengthSize = 4;

// Custom exception to throw when there is nothing else to read.
class SafeReaderException: public std::exception {
  virtual const char* what() const throw() {
    return "Reached the end of byte array.";
  }
};

// This class throws an exception when it reaches
class SafeReader {
 public:
  SafeReader(const Bytes& tag, int seek) :
      tag_(tag), tag_size_(tag.size()), seek_start_(seek), seek_end_(seek) {}
  int readInt(int size) {
    if (seek_end_ + size > tag_size_) throw SafeReaderException();

    using Reader::Utility::lEndianToInt;
    // TODO
    // int foo = lEndianToInt(, , false);
  }
  std::string readString(int size) {
    if (seek_end_ + size > tag_size_) throw SafeReaderException();

    // TODO
  }
  inline int bytesRead() {
    return seek_end_ - seek_start_;
  }
 private:
  const Bytes& tag_;
  int tag_size_;
  int seek_start_;
  int seek_end_;
};

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
  SafeReader reader(tag, seek);
  try {
    // TODO
  } catch (const SafeReaderException&) {
    return -1;
  }
  return reader.bytesRead();
}

}  // namespace VorbisShared
}  // namespace ReaderTag
}  // namespace Mp3Edit
