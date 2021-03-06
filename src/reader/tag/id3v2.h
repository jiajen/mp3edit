#ifndef MP3EDIT_READER_TAG_ID3V2_H_
#define MP3EDIT_READER_TAG_ID3V2_H_

#include "mp3edit/src/filesystem.h"
#include "mp3edit/src/bytes.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Id3v2 {

int seekHeaderEnd(Filesystem::FileStream& file_stream, int seek);

int seekFooterStart(Filesystem::FileStream& file_stream, int seek);

// Returns true if it is a valid id3v2 tag.
// Size is the total size of the tag (including header and footer)
bool parseTagHeader(const Bytes& header, bool is_footer, int& version,
                    int& size, bool& has_unsync, bool& has_extended_header);

}  // namespace Id3v2
}  // namespace ReaderTag
}  // namespace Mp3Edit

#endif  // MP3EDIT_READER_TAG_ID3V2_H_
