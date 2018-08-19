#ifndef MP3EDIT_SRC_READER_UTILITY_H_
#define MP3EDIT_SRC_READER_UTILITY_H_

#include <string>

#include "mp3edit/src/bytes.h"

namespace Mp3Edit {
namespace Reader {
namespace Utility {

class Crc32 {
 public:
  enum class CrcPolynomial {
    kCode0x04c11db7 = 0,
  };
  Crc32(CrcPolynomial polynomial, unsigned int initial_val,
        unsigned int final_xor);
  void update(const unsigned char* data, int size);
  unsigned int checksum();
 private:
  unsigned int _crc32;
  unsigned int _final_xor;
  const unsigned int* _crc_table;
};

// Returns -1 on failure.
int lEndianToInt(Bytes::const_iterator it_begin, Bytes::const_iterator it_end,
                 bool is_sync_safe);

// Returns -1 on failure.
int bEndianToInt(Bytes::const_iterator it_begin, Bytes::const_iterator it_end,
                 bool is_sync_safe);

// Throws an exception on failure.
Bytes intToBEndian(unsigned int val, int length, bool is_sync_safe);

// Throws an exception on failure.
Bytes intToLEndian(unsigned int val, int length, bool is_sync_safe);

template<class T>
void bytesToString(T it_begin, T it_end, std::string& output);

template<class T>
void bytesToTrack(T it_begin, T it_end, int& track_num, int& track_denum);

}  // namespace Utility
}  // namespace Reader
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_READER_UTILITY_H_
