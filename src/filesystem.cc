#include "mp3edit/src/filesystem.h"

#include <exception>

namespace Mp3Edit {
namespace Filesystem {

bool readBytes(FileStream& file_stream, int offset, int length,
               Bytes& output, Way way) {
  output.resize(length);
  file_stream.seekg(offset, (way == kFront) ? file_stream.beg:file_stream.end);
  file_stream.read((char*)output.data(), length);
  if (!file_stream) {
    output.clear();
    throw std::system_error(std::error_code(), "Read error.");
  }
  return true;
}

}  // namespace Filesystem
}  // namespace Mp3Edit
