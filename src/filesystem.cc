#include "mp3edit/src/filesystem.h"

#include <stdexcept>

namespace Mp3Edit {
namespace Filesystem {

FileWriter::FileWriter(const std::string& path) {
  file_ = fopen(path.c_str(), "wb");
  if (file_ == NULL) {
    throw std::runtime_error("Cannot create file.");
    closed_ = true;
  }
  closed_ = false;
}

FileWriter::~FileWriter() {
  if (!closed_) close();
}

void FileWriter::write(const Bytes& data) {
  if (closed_) throw std::runtime_error("File is closed.");
  if (data.size() == 0) return;
  int written_bytes = fwrite(data.data(), 1, data.size(), file_);
  if (written_bytes != (int)data.size()) {
    throw std::runtime_error("Unable to complete writing.");
  }
}

void FileWriter::close() {
  if (closed_)
    throw std::runtime_error("File already closed.");
  fclose(file_);
  closed_ = true;
}

bool readBytes(FileStream& file_stream, int offset, int length, Bytes& output) {
  output.resize(length);
  file_stream.seekg(offset, file_stream.beg);
  file_stream.read((char*)output.data(), length);
  if (!file_stream) {
    output.clear();
    throw std::runtime_error("Read error.");
  }
  return true;
}

}  // namespace Filesystem
}  // namespace Mp3Edit
