#include "mp3edit/src/reader/tag/ape.h"

#include <cstring>

#include "mp3edit/src/bytes.h"
#include "mp3edit/src/reader/utility.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Ape {

namespace {

using Filesystem::readBytes;

const int kApeTagVersion1 = 1000;
const int kApeTagVersion2 = 2000;

const int kTagFooterLength = 32;
const int kTagHeaderLength = kTagFooterLength;
const int kTagVersionStartPos = 8;
const int kTagVersionSize = 4;
const int kTagSizeStartPos = 12;
const int kTagSizeSize = 4;

const int kTagHeaderFlagByte = 23;
const int kTagFlagHeaderExistsPos = 7;

// Must be zero
const int kTagReservedStartPos = 24;
const int kTagReservedSize = 8;

// Returns false if tag is not valid.
bool parseTagFooter(const Bytes& footer, int& size) {
  if (footer.size() != kTagFooterLength) return false;
  if (strncmp((const char*)footer.data(), "APETAGEX", 8) != 0) return false;

  for (int i = 0; i < kTagReservedSize; i++) {
    if (footer[kTagReservedStartPos+i] != 0x00) return false;
  }

  using Reader::Utility::lEndianToInt;
  int version = lEndianToInt(footer.begin() + kTagVersionStartPos,
                             footer.begin() + kTagVersionStartPos +
                                              kTagVersionSize,
                             false);

  size = lEndianToInt(footer.begin() + kTagSizeStartPos,
                      footer.begin() + kTagSizeStartPos + kTagSizeSize,
                      false);

  // For version 1, size includes entire tag (includes the footer)
  // For version 2, size includes entire tag with footer but not the header
  // (if present). Presence of the header is detected via a flag. This flag
  // only exists in version 2.
  bool has_header = (version == kApeTagVersion2 &&
                     (footer[kTagHeaderFlagByte]&(1<<kTagFlagHeaderExistsPos)));

  if (has_header) size += kTagHeaderLength;

  return true;
}

}  // namespace

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek) {
  Bytes header;
  readBytes(file_stream, seek, kTagHeaderLength, header);

  int size;
  if (parseTagFooter(header, size)) seek += size;

  return seek;
}

int seekFooterStart(Filesystem::FileStream& file_stream, int seek) {
  Bytes footer;
  readBytes(file_stream, seek - kTagFooterLength, kTagFooterLength, footer);

  int size;
  if (parseTagFooter(footer, size)) seek -= size;

  return seek;
}

}  // namespace Ape
}  // namespace ReaderTag
}  // namespace Mp3Edit
