#ifndef MP3EDIT_SRC_READER_UTILITY_H_
#define MP3EDIT_SRC_READER_UTILITY_H_

#include <string>

#include "mp3edit/src/bytes.h"

namespace Mp3Edit {
namespace Reader {
namespace Utility {

// Returns -1 on failure.
int lEndianToInt(Bytes::const_iterator it_begin, Bytes::const_iterator it_end,
                 bool is_sync_safe);

// Returns -1 on failure.
int bEndianToInt(Bytes::const_iterator it_begin, Bytes::const_iterator it_end,
                 bool is_sync_safe);

void bytesToString(Bytes::const_iterator it_begin, Bytes::const_iterator it_end,
                   std::string& output);

void bytesToTrack(Bytes::const_iterator it_begin, Bytes::const_iterator it_end,
                  int& track_num, int& track_denum);

}  // namespace Utility
}  // namespace Reader
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_READER_UTILITY_H_
