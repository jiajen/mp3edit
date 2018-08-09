#include "mp3edit/src/reader/utility.h"

namespace Mp3Edit {
namespace Reader {
namespace Utility {

namespace {

inline bool isValidByteArray(Bytes::const_iterator it_begin,
                             Bytes::const_iterator it_end) {
  return (it_begin <= it_end);
}

// it_begin starts at the lowest significant byte.
// (e.g. little endian will cause it_begin < it_end)
int bytesToInt(Bytes::const_iterator it_begin, Bytes::const_iterator it_end,
               bool is_sync_safe) {
  int size = 0;
  for (int shift = 0, byte; it_begin != it_end; shift += (is_sync_safe ? 7:8)) {
    byte = (int)(*it_begin);
    if (is_sync_safe && byte >= 0x80) return -1;
    size += (byte << shift);
    it_begin = (it_begin < it_end) ? it_begin+1:it_begin-1;
  }
  return size;
}

}  // namespace

int lEndianToInt(Bytes::const_iterator it_begin, Bytes::const_iterator it_end,
                 bool is_sync_safe) {
  if (!isValidByteArray(it_begin, it_end)) return -1;
  return bytesToInt(it_begin, it_end, is_sync_safe);
}

int bEndianToInt(Bytes::const_iterator it_begin, Bytes::const_iterator it_end,
                 bool is_sync_safe) {
  if (!isValidByteArray(it_begin, it_end)) return -1;
  return bytesToInt(it_end-1, it_begin-1, is_sync_safe);
}

void bytesToString(Bytes::const_iterator it_begin, Bytes::const_iterator it_end,
                   std::string& output) {
  // TODO
}

void bytesToTrack(Bytes::const_iterator it_begin, Bytes::const_iterator it_end,
                  int& track_num, int& track_denum) {
  // TODO
}

}  // namespace Utility
}  // namespace Reader
}  // namespace Mp3Edit
