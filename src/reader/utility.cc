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
int bytesToInt(Bytes::const_iterator it_begin, Bytes::const_iterator it_end) {
  int size = 0;
  for (int shift = 0; it_begin != it_end; shift += 8) {
    size += (((int)(*it_begin)) << shift);
    it_begin = (it_begin < it_end) ? it_begin+1:it_begin-1;
  }
  return size;
}

}  // namespace

int lEndianToInt(Bytes::const_iterator it_begin, Bytes::const_iterator it_end) {
  if (!isValidByteArray(it_begin, it_end)) return -1;
  return bytesToInt(it_begin, it_end);
}

int bEndianToInt(Bytes::const_iterator it_begin, Bytes::const_iterator it_end) {
  if (!isValidByteArray(it_begin, it_end)) return -1;
  return bytesToInt(it_end-1, it_begin-1);
}

}  // namespace Utility
}  // namespace Reader
}  // namespace Mp3Edit
