#ifndef MP3EDIT_FILESYSTEM_H_
#define MP3EDIT_FILESYSTEM_H_

#include <cstdio>

#include <fstream>
#include <string>

#include "mp3edit/src/bytes.h"

namespace Mp3Edit {
namespace Filesystem {

typedef std::basic_ifstream<char> FileStream;

class FileWriter {
 public:
  explicit FileWriter(const std::string& path);
  ~FileWriter();
  void write(const Bytes& data);
  void close();

 private:
  FILE* file_;
  bool closed_;
};

bool readBytes(FileStream& file_stream, int offset, int length, Bytes& output);

}  // namespace Filesystem
}  // namespace Mp3Edit

#endif  // MP3EDIT_FILESYSTEM_H_
