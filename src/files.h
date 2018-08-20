#ifndef MP3EDIT_SRC_FILES_H_
#define MP3EDIT_SRC_FILES_H_

#include <string>
#include <vector>

#include "mp3edit/src/file.h"

namespace Mp3Edit {
namespace Files {

// TODO
class Files {
 public:
  Files();
  Files(const std::string& directory, bool recurse, bool read_audio_data);
  inline std::vector<File::File>::iterator begin() { return files_.begin(); }
  inline std::vector<File::File>::iterator end() { return files_.end(); }
  inline File::File& operator[](int idx) { return files_[idx]; }
 private:
  std::vector<File::File> files_;
};

}  // namespace Files
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_FILES_H_
