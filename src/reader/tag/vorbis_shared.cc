#include "mp3edit/src/reader/tag/vorbis_shared.h"

#include <cstring>

#include <exception>

#include "mp3edit/src/reader/utility.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisShared {

namespace {

using Filesystem::readBytes;

const int kLengthSize = 4;

int kCommentTitleHeaderSize = 6;
const char* kCommentTitleHeader = "TITLE=";
int kCommentArtistHeaderSize = 7;
const char* kCommentArtistHeader = "ARTIST=";
int kCommentAlbumHeaderSize = 6;
const char* kCommentAlbumHeader = "ALBUM=";
int kCommentTrackHeaderSize = 12;
const char* kCommentTrackHeader = "TRACKNUMBER=";

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
    int ans = lEndianToInt(tag_.begin() + seek_end_,
                           tag_.begin() + seek_end_ + size,
                           false);

    seek_end_ += size;
    return ans;
  }
  std::string readString(int size) {
    if (seek_end_ + size > tag_size_) throw SafeReaderException();

    std::string ans;
    ans.resize(size);
    strncpy(&ans[0], (char*)(tag_.data() + seek_end_), size);

    seek_end_ += size;
    return ans;
  }
  void readSkip(int size) {
    if (seek_end_ + size > tag_size_) throw SafeReaderException();
    seek_end_ += size;
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

bool matchComment(const std::string& comment, const char* header,
                  int header_size, std::string& value) {
  if ((int)comment.length() < header_size) return false;
  if (strncmp((const char*)comment.data(), header, header_size) == 0) {
    value = std::string(comment.begin() + header_size, comment.end());
    return true;
  } else {
    return false;
  }
}
inline bool checkCommentIsTitle(const std::string& comment,
                                std::string& value) {
  return matchComment(comment, kCommentTitleHeader,
                      kCommentTitleHeaderSize, value);
}
inline bool checkCommentIsArtist(const std::string& comment,
                                 std::string& value) {
  return matchComment(comment, kCommentArtistHeader,
                      kCommentArtistHeaderSize, value);
}
inline bool checkCommentIsAlbum(const std::string& comment,
                                std::string& value) {
  return matchComment(comment, kCommentAlbumHeader,
                      kCommentAlbumHeaderSize, value);
}
inline bool checkCommentIsTrack(const std::string& comment,
                                std::string& value) {
  return matchComment(comment, kCommentTrackHeader,
                      kCommentTrackHeaderSize, value);
}

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
  title.clear();
  artist.clear();
  album.clear();
  track_num = -1;
  track_denum = -1;

  SafeReader reader(tag, seek);
  std::string track;
  try {
    int vender_length = reader.readInt(kLengthSize);
    reader.readSkip(vender_length);
    int comment_list_length = reader.readInt(kLengthSize);
    while (comment_list_length--) {
      int comment_length = reader.readInt(kLengthSize);
      std::string comment = reader.readString(comment_length);
      if (!checkCommentIsTitle(comment, title) &&
          !checkCommentIsArtist(comment, artist) &&
          !checkCommentIsAlbum(comment, album) &&
          checkCommentIsTrack(comment, track)) {
        // TODO parse track
      }
    }
    if (has_framing_bit && reader.readInt(1) != 0x01)
      return -1;
  } catch (const SafeReaderException&) {
    return -1;
  }
  return reader.bytesRead();
}

}  // namespace VorbisShared
}  // namespace ReaderTag
}  // namespace Mp3Edit
