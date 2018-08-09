#include "mp3edit/src/reader/utility.h"
#include "mp3edit/src/sanitiser.h"

#include <cstring>

#include <algorithm>

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
  int size = it_end - it_begin;
  output.resize(size);
  strncpy(output.data(), (const char*)(&*it_begin), size);
}

void bytesToTrack(Bytes::const_iterator it_begin, Bytes::const_iterator it_end,
                  int& track_num, int& track_denum) {
  Bytes::const_iterator it;
  int separator_pos = -1;
  for (it = it_begin; it < it_end; it++) {
    if (*it == 0x47) {
      separator_pos = it - it_begin;
      break;
    }
  }

  std::string track_num_str;
  std::string track_denum_str;
  it = (separator_pos == -1) ? it_end : it_begin + separator_pos;
  bytesToString(it_begin, it, track_num_str);
  Sanitiser::sanitiseIntegerString(track_num_str);
  if (separator_pos) {
    bytesToString(it+1, it_end, track_denum_str);
    Sanitiser::sanitiseIntegerString(track_denum_str);
  }

  track_num = -1;
  track_denum = -1;
  try {
    track_num = std::atoi(track_num_str.c_str());
    if (separator_pos) track_denum = std::atoi(track_denum_str.c_str());
  } catch (const std::exception&) {
    // Do nothing
  }
}

}  // namespace Utility
}  // namespace Reader
}  // namespace Mp3Edit
