#include "mp3edit/src/reader/tag/id3v2_3.h"

#include <cstring>

#include "mp3edit/src/filesystem.h"
#include "mp3edit/src/sanitiser.h"
#include "mp3edit/src/reader/utility.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Id3v2_3 {
namespace {

const int kTagHeaderLength = 10;
const int kTagSizeLength = 4;
const int kExtendedHeaderLength = 10;
const int kExtendedHeaderCrcLength = 4;
const int kExtendedHeaderTagFlagStart = 14;
const int kExtendedHeaderTagFlagSize = 2;
const int kExtendedTagFlagCrcBitPos = 7;
const int kTagFrameHeaderLength = 10;
const int kTagFrameHeaderIdStart = 0;
const int kTagFrameHeaderIdLength = 4;
const int kTagFrameSizeStart = 4;
const int kTagFrameSizeLength = 4;
const int kTagFrameFlagsLength = 2;

const char* kTagFrameIdTitle = "TIT2";
const char* kTagFrameIdArtist = "TPE1";
const char* kTagFrameIdAlbum = "TALB";
const char* kTagFrameIdTrack = "TRCK";
const char* kTagFrameIdAlbumArtist = "TPE2";

const char* kHeaderTemplate = "\x49\x44\x33\x03\x00\x00";

using Filesystem::readBytes;
using Reader::Utility::intToBEndian;
using Reader::Utility::bEndianToInt;
using Reader::Utility::bytesToString;
using Reader::Utility::bytesToTrack;

typedef const unsigned char* BytePtr;

int getPostHeaderSeek(const Bytes& tag) {
  if (tag[kExtendedHeaderTagFlagStart]&(1<<kExtendedTagFlagCrcBitPos)) {
    return kTagHeaderLength + kExtendedHeaderLength + kExtendedHeaderCrcLength;
  } else {
    return kTagHeaderLength + kExtendedHeaderLength;
  }
}

Bytes clearTagUnsync(const Bytes& raw_tag) {
  Bytes new_tag(raw_tag.begin(), raw_tag.begin()+10);
  new_tag.reserve(raw_tag.size());
  for (int i = 10, s = raw_tag.size(); i < s; i++) {
    new_tag.push_back(raw_tag[i]);
    if (i < s-1 && raw_tag[i] == 0xFF && raw_tag[i+1] == 0x00) i++;
  }
  return new_tag;
}

int calculateTagDataSize(const std::string& title, const std::string& artist,
                         const std::string& album,
                         int track_num, int track_denum) {
  int size = kTagHeaderLength;
  // +2 for leading and trailing 0x00 needed for ISO-8859 encoding
  if (!title.empty()) size += kTagFrameHeaderLength + 2 + title.length();
  if (!artist.empty()) size += kTagFrameHeaderLength + 2 + artist.length();
  if (!album.empty()) size += kTagFrameHeaderLength + 2 + album.length();
  if (track_num != -1) {
    std::string track_num_str = std::to_string(track_num);
    size += kTagFrameHeaderLength + 2 + track_num_str.length();
    if (track_denum != -1) {
      std::string track_denum_str = std::to_string(track_denum);
      size += 1 + track_denum_str.length();
    }
  }

  return (size == kTagHeaderLength) ? 0 : size;
}

std::string generateTrackString(int track_num, int track_denum) {
  if (track_denum == -1) {
    return std::to_string(track_num);
  } else {
    return std::to_string(track_num) + "/" + std::to_string(track_denum);
  }
}

// Assumes all frames are in ISO-8859.
void appendFrame(const char* frame_id, const std::string& data, Bytes& tag) {
  tag.insert(tag.end(), (BytePtr)frame_id,
                        (BytePtr)frame_id + kTagFrameHeaderIdLength);
  Bytes frame_size_bytes = intToBEndian(2 + data.length(),
                                        kTagFrameSizeLength, false);
  tag.insert(tag.end(), frame_size_bytes.begin(), frame_size_bytes.end());
  tag.insert(tag.end(), kTagFrameFlagsLength, 0x00);
  // TODO
}

}  // namespace

void parseTag(const Bytes& raw_tag, std::string& title, std::string& artist,
              std::string& album, int& track_num, int& track_denum) {
  bool has_unsync, has_extended_header;
  int tag_version, unused;

  Bytes header(raw_tag.begin(), raw_tag.begin() + kTagHeaderLength);
  Id3v2::parseTagHeader(header, false, tag_version, unused,
                        has_unsync, has_extended_header);
  if (tag_version != 3) return;

  Bytes tag = (has_unsync) ? clearTagUnsync(raw_tag) : raw_tag;
  int seek = (has_extended_header) ? getPostHeaderSeek(tag) : kTagHeaderLength;

  int tag_size = tag.size();
  int frame_size;
  while (seek + kTagFrameHeaderLength < tag_size) {
    frame_size = bEndianToInt(tag.begin() + seek + kTagFrameSizeStart,
                              tag.begin() + seek + kTagFrameSizeStart +
                                                   kTagFrameSizeLength, false);

    const int& frame_id_len = kTagFrameHeaderIdLength;
    const char* frame_ptr = (const char*)(tag.data() + seek +
                                          kTagFrameHeaderIdStart);
    Bytes::const_iterator frame_data_ptr = tag.begin() + seek +
                                           kTagHeaderLength;
    if (strncmp(frame_ptr, kTagFrameIdTitle, frame_id_len) == 0) {
      bytesToString(frame_data_ptr, frame_data_ptr + frame_size, title);
      Sanitiser::sanitiseString(title);
    } else if (strncmp(frame_ptr, kTagFrameIdArtist, frame_id_len) == 0) {
      bytesToString(frame_data_ptr, frame_data_ptr + frame_size, artist);
      Sanitiser::sanitiseString(artist);
    } else if (strncmp(frame_ptr, kTagFrameIdAlbum, frame_id_len) == 0) {
      bytesToString(frame_data_ptr, frame_data_ptr + frame_size, album);
      Sanitiser::sanitiseString(album);
    } else if (strncmp(frame_ptr, kTagFrameIdTrack, frame_id_len) == 0) {
      bytesToTrack(frame_data_ptr, frame_data_ptr + frame_size,
                   track_num, track_denum);
      Sanitiser::sanitiseTrack(track_num, track_denum);
    } else if (strncmp(frame_ptr, kTagFrameIdAlbumArtist, frame_id_len) == 0) {
      std::string album_artist;
      bytesToString(frame_data_ptr, frame_data_ptr + frame_size,
                    album_artist);
      Sanitiser::sanitiseString(album_artist);
      if (artist != album_artist) artist += " + " + album_artist;
    } else if (*frame_ptr == 0x00) {
      break;
    }

    seek += kTagFrameHeaderLength + frame_size;
  }
}

Bytes extractTag(Filesystem::FileStream& file_stream,
                 int seek_tag_start, int seek_tag_end) {
  Bytes tag;
  readBytes(file_stream, seek_tag_start, seek_tag_end - seek_tag_start, tag);
  return tag;
}

Bytes generateTag(const std::string& title, const std::string& artist,
                  const std::string& album, int track_num, int track_denum) {
  Bytes tag;
  int tag_data_size = calculateTagDataSize(title, artist, album,
                                           track_num, track_denum);
  // Id3v2.3 forbids a tag with zero frames.
  if (tag_data_size == 0) return tag;
  tag.reserve(tag_data_size);

  tag.insert(tag.end(), (BytePtr)kHeaderTemplate,
                        (BytePtr)kHeaderTemplate + kTagHeaderLength -
                                                   kTagSizeLength);
  Bytes tag_size_bytes = intToBEndian(tag_data_size - kTagHeaderLength,
                                      kTagSizeLength, true);
  tag.insert(tag.end(), tag_size_bytes.begin(), tag_size_bytes.end());

  if (!title.empty()) {

  }
  if (!artist.empty()) {

  }
  if (!album.empty()) {

  }
  if (track_num != -1) {

  }
  // TODO generate tag frames

  return tag;
}

}  // Id3v2
}  // ReaderTag
}  // Mp3Edit
