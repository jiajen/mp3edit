#ifndef MP3EDIT_SRC_FILESYSTEM_H_
#define MP3EDIT_SRC_FILESYSTEM_H_

#include <fstream>

#include "mp3edit/src/bytes.h"

namespace Mp3Edit {
namespace Filesystem {

typedef std::basic_ifstream<char> FileStream;

bool readBytes(FileStream& file_stream, int offset, int length, Bytes& output);

}  // namespace Filesystem
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_FILESYSTEM_H_
