#include "mp3edit/src/reader/tag/id3v2.h"

#include <cstring>

#include "mp3edit/src/reader/utility.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Id3v2 {

namespace {

using Filesystem::readBytes;

const int kTagHeaderLength = 10;
const int kTagMajorVersionPos = 3;
const int kTagMinorVersionPos = 4;
const int kTagFlagsPos = 5;
const int kTagHeaderSizePos = 6;
const int kTagFlagUnsyncBitPos = 7;
const int kTagFlagCompressionBitPos = 6;  // Only in id3v2.2
const int kTagFlagExtendedHeaderBitPos = 6;  // Only in id3v2.3 or id3v2.4
const int kTagFlagExperimentalBitPos = 5;  // Only in id3v2.3
const int kTagFlagFooterPresentBitPos = 4;  // Only in id3v2.4

}  // namespace

bool parseTagHeader(const Bytes& header, bool is_footer, int& version,
                    int& size, bool& has_unsync, bool& has_extended_header) {
  if (header.size() != kTagHeaderLength) return false;
  if (is_footer) {
    if (strncmp((const char*)header.data(), "3DI", 3) != 0) return false;
  } else {
    if (strncmp((const char*)header.data(), "ID3", 3) != 0) return false;
  }
  if (header[kTagMajorVersionPos] == 0xFF ||
      header[kTagMinorVersionPos] == 0xFF) return false;

  version = header[kTagMajorVersionPos];

  unsigned char tags = header[kTagFlagsPos];
  has_unsync = (bool)(tags&(1<<kTagFlagUnsyncBitPos));
  if (version == 3 || version == 4) {
    has_extended_header = (bool)(tags&(1<<kTagFlagExtendedHeaderBitPos));
  } else {
    has_extended_header = false;
  }
  bool has_footer = (bool)(version == 4 &&
                           (tags&(1<<kTagFlagFooterPresentBitPos)));
  if (is_footer && !has_footer) return false;

  using Reader::Utility::bEndianToInt;
  size = bEndianToInt(header.begin() + kTagHeaderSizePos,
                      header.begin() + kTagHeaderLength,
                      true);
  if (size == -1) return false;
  size += ((has_footer) ? 20 : 10);
  return true;
}

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek) {
  Bytes header;
  readBytes(file_stream, seek, kTagHeaderLength, header);

  int version, size;
  bool flag_1, flag_2;
  if (parseTagHeader(header, false, version, size, flag_1, flag_2))
    seek += size;

  return seek;
}

int seekFooterStart(Filesystem::FileStream& file_stream, int seek) {
  Bytes header;
  readBytes(file_stream, seek - kTagHeaderLength, kTagHeaderLength, header);

  int version, size;
  bool flag_1, flag_2;
  if (parseTagHeader(header, true, version, size, flag_1, flag_2))
    seek -= size;

  return seek;
}

}  // namespace Id3v2
}  // namespace ReaderTag
}  // namespace Mp3Edit
