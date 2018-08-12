#include "mp3edit/src/reader/tag/vorbis_flac.h"

#include <cstring>

#include <system_error>

#include "mp3edit/src/reader/utility.h"
#include "mp3edit/src/reader/tag/vorbis_shared.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace VorbisFlac {

namespace {

using Filesystem::readBytes;
using Reader::Utility::intToBEndian;

const int kPreambleLength = 4;
const int kBlockHeaderLength = 4;
const int kBlockTypePos = 0;
const int kBlockFlagLastBlockBitPos = 7;
const int kBlockTypeVorbisBitMask = 4;
const int kBlockSizeStartPos = 1;
const int kBlockSizeSize = 3;
const int kBlockPostMetadataPos = 42;

const unsigned char kBlockTypeVorbis = 0x04;

int skipMetadataBlock(Filesystem::FileStream& file_stream, int seek,
                      bool extact_vorbis, Bytes& tag) {
  Bytes header;
  readBytes(file_stream, seek, kPreambleLength, header);

  if (strncmp((const char*)header.data(), "fLaC", kPreambleLength) != 0)
    return seek;

  seek += kPreambleLength;
  int size;
  do {
    readBytes(file_stream, seek, kBlockHeaderLength, header);
    if ((header[kBlockTypePos]&0x7F) == 0x7F)
      throw std::system_error(std::error_code(), "Invalid FLAC.");

    using Reader::Utility::bEndianToInt;
    size = kBlockHeaderLength +
           bEndianToInt(header.begin() + kBlockSizeStartPos,
                        header.begin() + kBlockSizeStartPos + kBlockSizeSize,
                        false);

    if (extact_vorbis && ((header[0]&kBlockTypeVorbis) == kBlockTypeVorbis))
      readBytes(file_stream, seek, size, tag);

    seek += size;
  } while (!(header[kBlockTypePos]&(1<<kBlockFlagLastBlockBitPos)));

  return seek;
}

}  // namespace

void parseTag(const Bytes& tag, std::string& title, std::string& artist,
              std::string& album, int& track_num, int& track_denum) {
  if (tag.empty()) return;
  VorbisShared::parseTag(tag, kBlockHeaderLength, false, false,
                         title, artist, album, track_num, track_denum);
}

Bytes extractTag(Filesystem::FileStream& file_stream,
                 int seek_tag_start, int) {
  Bytes tag;
  skipMetadataBlock(file_stream, seek_tag_start, true, tag);
  return tag;
}

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek) {
  Bytes unused;
  return skipMetadataBlock(file_stream, seek, false, unused);
}

Bytes generateTag(Filesystem::FileStream& file_stream, int seek_flac_start,
                  const std::string& title, const std::string& artist,
                  const std::string& album, int track_num, int track_denum) {
  Bytes tag;
  readBytes(file_stream, seek_flac_start, kBlockPostMetadataPos, tag);

  if (title.empty() && artist.empty() && album.empty() &&
      track_num == -1 && track_denum == -1) {
    tag[kPreambleLength+kBlockTypePos] = (1<<kBlockFlagLastBlockBitPos);
    return tag;
  } else {
    tag[kPreambleLength+kBlockTypePos] = 0;
  }

  using VorbisShared::generateTag;
  Bytes vorbis_tag = generateTag(title, artist, album,
                                 track_num, track_denum, false);

  tag.reserve(kBlockPostMetadataPos + kBlockHeaderLength + vorbis_tag.size());

  tag.push_back(0x84);
  Bytes size_bytes = intToBEndian(vorbis_tag.size(), kBlockSizeSize, false);
  tag.insert(tag.end(), size_bytes.begin(), size_bytes.end());

  tag.insert(tag.end(), vorbis_tag.begin(), vorbis_tag.end());

  return tag;
}

}  // namespace VorbisFlac
}  // namespace ReaderTag
}  // namespace Mp3Edit
