#include "mp3edit/src/reader/tag/id3v2_3.h"

#include <cstring>

#include "mp3edit/src/filesystem.h"
#include "mp3edit/src/reader/utility.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Id3v2_3 {
namespace {

const int kTagHeaderLength = 10;
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

const char* kTagFrameIdTitle = "TIT2";
const char* kTagFrameIdArtist = "TPE1";
const char* kTagFrameIdAlbum = "TALB";
const char* kTagFrameIdTrack = "TRCK";
const char* kTagFrameIdAlbumArtist = "TPE2";

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

}  // namespace

void parseTag(const Bytes& raw_tag, std::string& title, std::string& artist,
              std::string& album, int track_num, int track_denum) {

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
    using Reader::Utility::bEndianToInt;
    frame_size = bEndianToInt(tag.begin() + seek + kTagFrameSizeStart,
                              tag.begin() + seek + kTagFrameSizeStart +
                                                   kTagFrameSizeLength, false);

    // TODO output data from tag

    const int& frame_id_len = kTagFrameHeaderIdLength;
    const char* frame_ptr = (const char*)(tag.data() + seek +
                                          kTagFrameHeaderIdStart);
    if (strncmp(frame_ptr, kTagFrameIdTitle, frame_id_len) == 0) {
      // TODO save into relevant variable
    } else if (strncmp(frame_ptr, kTagFrameIdArtist, frame_id_len) == 0) {

    } else if (strncmp(frame_ptr, kTagFrameIdAlbum, frame_id_len) == 0) {

    } else if (strncmp(frame_ptr, kTagFrameIdTrack, frame_id_len) == 0) {

    } else if (strncmp(frame_ptr, kTagFrameIdAlbumArtist, frame_id_len) == 0) {

    }

    seek += kTagFrameHeaderLength + frame_size;
  }
  // TODO sanitise
}

Bytes extractTag(Filesystem::FileStream& file_stream,
                 int seek_tag_start, int seek_tag_end) {
  using Filesystem::readBytes;
  Bytes tag;
  readBytes(file_stream, seek_tag_start, seek_tag_end - seek_tag_start, tag);
  return tag;
}

}  // Id3v2
}  // ReaderTag
}  // Mp3Edit
