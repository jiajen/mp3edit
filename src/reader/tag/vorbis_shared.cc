#include "mp3edit/src/reader/tag/vorbis_shared.h"

#include <cstring>

#include <algorithm>
#include <exception>

#include "mp3edit/src/sanitiser.h"
#include "mp3edit/src/reader/utility.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisShared {

namespace {

using Reader::Utility::intToLEndian;

const int kLengthSize = 4;

int kCommentTitleHeaderSize = 6;
const char* kCommentTitleHeader = "TITLE=";
int kCommentArtistHeaderSize = 7;
const char* kCommentArtistHeader = "ARTIST=";
int kCommentAlbumHeaderSize = 6;
const char* kCommentAlbumHeader = "ALBUM=";
int kCommentTrackNumHeaderSize = 12;
const char* kCommentTrackNumHeader = "TRACKNUMBER=";
int kCommentTrackTotalHeaderSize = 11;
const char* kCommentTrackTotalHeader = "TRACKTOTAL=";
int kCommentAlbumArtistHeaderSize = 12;
const char* kCommentAlbumArtistHeader = "ALBUMARTIST=";

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
    memcpy(&ans[0], tag_.data() + seek_end_, size);

    seek_end_ += size;
    return ans;
  }
  void readSkip(int size) {
    if (seek_end_ + size > tag_size_) throw SafeReaderException();
    seek_end_ += size;
  }
  // Skips padding until end of page or encounters the first non 0x00 byte.
  void skipPadding() {
    while (seek_end_ < tag_size_ && tag_[seek_end_] == 0x00) seek_end_++;
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
bool matchTrackComment(const std::string& comment, const char* header,
                       int header_size, int& value) {
  std::string str;
  if (!matchComment(comment, header, header_size, str)) return false;
  Sanitiser::sanitiseIntegerString(str);
  try {
    value = std::atoi(str.c_str());
  } catch (const std::exception&) {
    // Ignore exception
  }
  if (value <= 0) value = -1;
  return true;
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
inline bool checkCommentIsTrackNum(const std::string& comment,
                                   int& value) {
  return matchTrackComment(comment, kCommentTrackNumHeader,
                           kCommentTrackNumHeaderSize, value);
}
inline bool checkCommentIsTrackTotal(const std::string& comment,
                                     int& value) {
  return matchTrackComment(comment, kCommentTrackTotalHeader,
                           kCommentTrackTotalHeaderSize, value);
}
inline bool checkCommentIsAlbumArtist(const std::string& comment,
                                      std::string& value) {
  return matchComment(comment, kCommentAlbumArtistHeader,
                      kCommentAlbumArtistHeaderSize, value);
}

void markVorbisData(const std::string& field, int field_header_size,
                    int& field_count, int& size) {
  if (field.empty()) return;
  size += kLengthSize + field_header_size + field.length();
  field_count++;
}

void markVorbisData(int field, int field_header_size,
                    int& field_count, int& size) {
  if (field == -1) return;
  markVorbisData(std::to_string(field), field_header_size, field_count, size);
}

void addVorbisField(const std::string& field, const char* field_header,
                    int field_header_size, Bytes& tag) {
  if (field.empty()) return;
  Bytes size_bytes = intToLEndian(field.length() + field_header_size,
                                  kLengthSize, false);
  tag.insert(tag.end(), size_bytes.begin(), size_bytes.end());
  tag.insert(tag.end(), field_header, field_header + field_header_size);
  tag.insert(tag.end(), field.begin(), field.end());
}

void addVorbisField(int field, const char* field_header,
                    int field_header_size, Bytes& tag) {
  if (field == -1) return;
  addVorbisField(std::to_string(field), field_header, field_header_size, tag);
}

}  // namespace

int parseTag(const Bytes& tag, int seek,
             bool has_framing_bit, bool remove_padding) {
  std::string unused_s1, unused_s2, unused_s3;
  int unused_i1, unused_i2;
  return parseTag(tag, seek, has_framing_bit, remove_padding,
                  unused_s1, unused_s2, unused_s3, unused_i1, unused_i2);
}

int parseTag(const Bytes& tag, int seek,
             bool has_framing_bit, bool remove_padding,
             std::string& title, std::string& artist, std::string& album,
             int& track_num, int& track_denum) {
  title.clear();
  artist.clear();
  album.clear();
  track_num = -1;
  track_denum = -1;

  SafeReader reader(tag, seek);
  std::string track;
  std::string album_artist;
  try {
    int vender_length = reader.readInt(kLengthSize);
    reader.readSkip(vender_length);
    int comment_list_length = reader.readInt(kLengthSize);
    while (comment_list_length--) {
      int comment_length = reader.readInt(kLengthSize);
      std::string comment = reader.readString(comment_length);
      if (checkCommentIsTitle(comment, title) ||
          checkCommentIsArtist(comment, artist) ||
          checkCommentIsAlbum(comment, album) ||
          checkCommentIsTrackNum(comment, track_num) ||
          checkCommentIsTrackTotal(comment, track_denum) ||
          checkCommentIsAlbumArtist(comment, album_artist)) continue;
    }
    Sanitiser::sanitiseString(title);
    Sanitiser::sanitiseString(artist);
    Sanitiser::sanitiseString(album);
    Sanitiser::sanitiseTrack(track_num, track_denum);
    Sanitiser::sanitiseString(album_artist);
    if (!album_artist.empty() && artist != album_artist)
      artist += " + " + album_artist;
    if (has_framing_bit && reader.readInt(1) != 0x01) return -1;
    if (remove_padding) reader.skipPadding();
  } catch (const SafeReaderException&) {
    return -1;
  }
  return reader.bytesRead();
}

Bytes generateTag(const std::string& title, const std::string& artist,
                  const std::string& album, int track_num, int track_denum,
                  bool has_framing_bit) {
  int num_fields = 0;
  int size = (int)has_framing_bit + 2*kLengthSize;  // Vendor Length and
                                                    // Number of Comments

  markVorbisData(title, kCommentTitleHeaderSize, num_fields, size);
  markVorbisData(artist, kCommentArtistHeaderSize, num_fields, size);
  markVorbisData(album, kCommentAlbumHeaderSize, num_fields, size);
  markVorbisData(track_num, kCommentTrackNumHeaderSize, num_fields, size);
  markVorbisData(track_denum, kCommentTrackTotalHeaderSize, num_fields, size);
  markVorbisData(artist, kCommentAlbumArtistHeaderSize, num_fields, size);

  Bytes tag;
  tag.reserve(size);

  tag.insert(tag.end(), kLengthSize, 0x00);
  Bytes tag_size_bytes = intToLEndian(num_fields, kLengthSize, false);
  tag.insert(tag.end(), tag_size_bytes.begin(), tag_size_bytes.end());

  addVorbisField(title, kCommentTitleHeader, kCommentTitleHeaderSize, tag);
  addVorbisField(artist, kCommentArtistHeader, kCommentArtistHeaderSize, tag);
  addVorbisField(album, kCommentAlbumHeader, kCommentAlbumHeaderSize, tag);
  addVorbisField(track_num,
                 kCommentTrackNumHeader, kCommentTrackNumHeaderSize, tag);
  if (track_num != -1)
    addVorbisField(track_denum,
                   kCommentTrackTotalHeader, kCommentTrackTotalHeaderSize, tag);
  addVorbisField(artist, kCommentAlbumArtistHeader,
                 kCommentAlbumArtistHeaderSize, tag);

  if (has_framing_bit) tag.push_back(0x01);

  return tag;
}

}  // namespace VorbisShared
}  // namespace ReaderTag
}  // namespace Mp3Edit
