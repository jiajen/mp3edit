#include "mp3edit/src/reader/tag/vorbis_ogg.h"

#include <cstring>

#include <system_error>

#include "mp3edit/src/reader/tag/vorbis_shared.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisOgg {

namespace {

using Filesystem::readBytes;

const int kFirstPageLength = 58;
const int kFirstPageDataStartPos = 28;
const int kPageHeaderPrefixLength = 27;
const int kPageNumberStartPos = 18;
const int kNumberPageSegmentsPos = 26;
const int kCommonVorbisHeaderSize = 7;

// Checks the validity of an ogg file's first page
// and the second page's header.
bool verifyValidOggHeaderPrefix(const Bytes& header) {
  // First Page (Identification Header)
  if (strncmp((const char*)header.data(), "OggS", 4) != 0) return false;
  if (header[kFirstPageDataStartPos] != 0x01) return false;
  if (strncmp((const char*)header.data() + kFirstPageDataStartPos + 1,
              "vorbis", 6) != 0)
    return false;

  // Second Page (Must fulfill a template as program makes an assumption)
  if (strncmp((const char*)header.data() + kFirstPageLength, "OggS", 4) != 0)
    return false;
  if ((header[kFirstPageLength+4]|header[kFirstPageLength+5]) != 0x00)
    return false;
  if (header[kFirstPageLength+kPageNumberStartPos] != 0x01 ||
      header[kFirstPageLength+kPageNumberStartPos+1] != 0x00 ||
      header[kFirstPageLength+kPageNumberStartPos+2] != 0x00 ||
      header[kFirstPageLength+kPageNumberStartPos+3] != 0x00)
    return false;

  return true;
}

int segmentTableToSize(const Bytes& segment_table) {
  int size = 0, n = segment_table.size();
  if (n > 0 && segment_table[n-1] == 0xFF)
    throw std::system_error(std::error_code(), "Invalid OGG.");
  for (int i = 0; i < n; i++)
    size += (int)segment_table[i];
  return size;
}

bool verifyValidOggVorbisCommentHeader(const Bytes& tag) {
  if (tag[0] != 0x03) return false;
  if (strncmp((const char*)tag.data()+1, "vorbis", 6) != 0) return false;
  return true;
}

}  // namespace

// Warning: Limited functionality. Although the official specification states
// that an OGG structure can have its vorbis comment span more than a single
// page, this function assumes that the vorbis comment is entirely encapsulated
// in the second page of an OGG file. This is done to simplify the process of
// saving the OGG file as this particular case only requires overwriting the
// second page (merely requiring adjustments to the size and checksum in the
// header of the second page).
// Future Improvements: Allow reading of vorbis comments spanning more than
// one page.
int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek) {
  Bytes header, segment_table, second_page;
  readBytes(file_stream, seek, kFirstPageLength + kPageHeaderPrefixLength,
            header);
  seek += kFirstPageLength + kPageHeaderPrefixLength;
  if (!verifyValidOggHeaderPrefix(header))
    throw std::system_error(std::error_code(), "Unsupported OGG.");

  int number_segments = header[kFirstPageLength + kNumberPageSegmentsPos];
  readBytes(file_stream, seek, number_segments, segment_table);
  seek += number_segments;
  int page_size = segmentTableToSize(segment_table);

  readBytes(file_stream, seek, page_size, second_page);
  if (!verifyValidOggVorbisCommentHeader(second_page))
    throw std::system_error(std::error_code(), "Invalid OGG.");
  int vorbis_tag_size = VorbisShared::parseTag(second_page,
                                               kCommonVorbisHeaderSize,
                                               true);
  if (vorbis_tag_size == -1)
    throw std::system_error(std::error_code(), "Unsupported OGG.");
  seek += kCommonVorbisHeaderSize + vorbis_tag_size;

  return seek;
}

int seekFooterStart(Filesystem::FileStream&, int seek) {
  return seek;
}

}  // namespace VorbisOgg
}  // namespace ReaderTag
}  // namespace Mp3Edit
