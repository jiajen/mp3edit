#ifndef MP3EDIT_SRC_FILES_H_
#define MP3EDIT_SRC_FILES_H_

#include <string>
#include <vector>

#include "mp3edit/src/file.h"

namespace Mp3Edit {
namespace Files {

class Files {
 private:
  class Error;
 public:
  Files();
  Files(const std::string& directory, bool recurse, bool read_audio_data);
  inline std::vector<File::File>::iterator begin() { return files_.begin(); }
  inline std::vector<File::File>::iterator end() { return files_.end(); }
  inline File::File& operator[](int idx) { return files_[idx]; }
  inline std::vector<Error> getErrorList() { return errors_; }
 private:
  class Error {
   public:
    Error(const std::string& filepath, const std::string& error_message);
    inline std::string path() { return filepath_; }
    inline std::string what() { return error_message_; }
   private:
    std::string filepath_;
    std::string error_message_;
  };
  template <class T>
  void readFiles(const std::string& directory, bool read_audio_data, T& it);
  std::vector<Error> errors_;
  std::vector<File::File> files_;
};

}  // namespace Files
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_FILES_H_
