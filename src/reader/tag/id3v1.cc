#include "mp3edit/src/reader/tag/id3v1.h"

#include <cstring>

#include "mp3edit/src/sanitiser.h"
#include "mp3edit/src/reader/utility.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Id3v1 {

namespace {

using Filesystem::readBytes;

const int kTagHeaderLength = 3;
const int kTagLength = 128;

const int kEnhancedTagHeaderLength = 4;
const int kEnhancedTagLength = 227;

const int kTagTitlePos = 3;
const int kTagArtistPos = 33;
const int kTagAlbumPos = 63;
const int kTagTrackPos = 126;
const int kTagGenrePos = 127;
const int kTagFieldSize = 30;

const unsigned char kTagDefaultGenre = 12;  // Genre: Other

const int kEnhancedTagTitlePos = 4;
const int kEnhancedTagArtistPos = 64;
const int kEnhancedTagAlbumPos = 124;
const int kEnhancedTagFieldSize = 60;

const char* kTagPreamble = "TAG";
const char* kEnhancedTagPreamble = "TAG+";

using Reader::Utility::bytesToString;

void mergeToExistingField(const std::string& parsed_field, std::string& field) {
  if (parsed_field.empty()) {
    return;
  } else if (field.empty()) {
    field = parsed_field;
  } else if (parsed_field.length() > field.length() ||
             field.substr(0, parsed_field.length()) != parsed_field) {
    field += " + " + parsed_field;
  }
}

void mergeToExistingTracks(int parsed_track, int& track_num, int& track_denum) {
  if (parsed_track == -1) {
    return;
  } else if (track_num == -1) {
    track_num = parsed_track;
  } else if (parsed_track != track_num) {
    track_num = -1;
    track_denum = -1;
  }
  Sanitiser::sanitiseTrack(track_num, track_denum);
}

}  // namespace

void parseTag(const Bytes& tag, std::string& title, std::string& artist,
              std::string& album, int& track_num, int& track_denum) {
  std::string parsed_title;
  std::string parsed_artist;
  std::string parsed_album;
  int parsed_track_num;

  Bytes::const_iterator tag_begin = tag.end() - kTagLength;
  bytesToString(tag_begin + kTagTitlePos,
                tag_begin + kTagTitlePos + kTagFieldSize, parsed_title);
  bytesToString(tag_begin + kTagArtistPos,
                tag_begin + kTagArtistPos + kTagFieldSize, parsed_artist);
  bytesToString(tag_begin + kTagAlbumPos,
                tag_begin + kTagAlbumPos + kTagFieldSize, parsed_album);
  parsed_track_num = (int)(*(tag_begin + kTagTrackPos));
  if (parsed_track_num == 0) parsed_track_num = -1;

  if (tag.size() == kEnhancedTagLength + kTagLength) {
    tag_begin -= kEnhancedTagLength;
    std::string extended_title;
    std::string extended_artist;
    std::string extended_album;
    bytesToString(tag_begin + kEnhancedTagTitlePos,
                  tag_begin + kEnhancedTagTitlePos + kEnhancedTagFieldSize,
                  extended_title);
    bytesToString(tag_begin + kEnhancedTagArtistPos,
                  tag_begin + kEnhancedTagArtistPos + kEnhancedTagFieldSize,
                  extended_artist);
    bytesToString(tag_begin + kEnhancedTagAlbumPos,
                  tag_begin + kEnhancedTagAlbumPos + kEnhancedTagFieldSize,
                  extended_album);
    parsed_title += extended_title;
    parsed_artist += extended_artist;
    parsed_album += extended_album;
  }

  Sanitiser::sanitiseString(parsed_title);
  Sanitiser::sanitiseString(parsed_artist);
  Sanitiser::sanitiseString(parsed_album);

  mergeToExistingField(parsed_title, title);
  mergeToExistingField(parsed_artist, artist);
  mergeToExistingField(parsed_album, album);
  mergeToExistingTracks(parsed_track_num, track_num, track_denum);
}

Bytes extractTag(Filesystem::FileStream& file_stream,
                 int seek_tag_start, int seek_tag_end) {
  Bytes tag;
  readBytes(file_stream, seek_tag_start, seek_tag_end - seek_tag_start, tag);
  return tag;
}

inline int limitLen(int length) {
  return (length < kTagFieldSize) ? length : kTagFieldSize;
}

Bytes generateTag(const std::string& title, const std::string& artist,
                  const std::string& album, int track_num, int) {
  Bytes tag(kTagLength, 0x00);
  memcpy(tag.data(), kTagPreamble, kTagHeaderLength);
  memcpy(tag.data() + kTagTitlePos, title.data(), limitLen(title.length()));
  memcpy(tag.data() + kTagArtistPos, artist.data(), limitLen(artist.length()));
  memcpy(tag.data() + kTagAlbumPos, album.data(), limitLen(album.length()));
  if (track_num != -1) tag[kTagTrackPos] = (unsigned char)(255&track_num);
  tag[kTagGenrePos] = kTagDefaultGenre;
  return tag;
}

int seekFooterStart(Filesystem::FileStream& file_stream, int seek) {
  typedef const char cc;
  Bytes header;

  readBytes(file_stream, seek - kTagLength, kTagHeaderLength, header);
  if (strncmp((cc*)header.data(), kTagPreamble,
                                  kTagHeaderLength) != 0) return seek;
  seek -= kTagLength;

  readBytes(file_stream, seek - kEnhancedTagLength,
            kEnhancedTagHeaderLength, header);
  if (strncmp((cc*)header.data(), kEnhancedTagPreamble,
                                  kEnhancedTagHeaderLength) == 0)
    seek -= kEnhancedTagLength;

  return seek;
}

}  // namespace Id3v1
}  // namespace ReaderTag
}  // namespace Mp3Edit
