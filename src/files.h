#ifndef MP3EDIT_SRC_FILES_H_
#define MP3EDIT_SRC_FILES_H_

#include <string>
#include <vector>
#include <mutex>

#include "mp3edit/src/file.h"

namespace Mp3Edit {

// WindowMain is used to callback its member function when a threaded operation
// is running.
namespace Gui {
  class WindowMain;
}  // namespace Gui

namespace Files {

class Files {
 private:
  class Error;
 public:
  inline File::File& operator[](int idx) { return files_[idx]; }
  inline int size() const { return files_.size(); }
  inline const std::vector<Error>& getErrorList() const { return errors_; }
  void readDirectory(const std::string& directory, bool recurse,
                     bool read_audio_data);
  bool saveFile(int idx, bool rename_file, bool clear_error_message = true);
  // Only save files that are valid as invalidated files (due to save errors)
  // will still exist in the vector and in order.
  bool saveAllFiles(bool rename_file);
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

  // Multi-threading
  std::mutex mutex_;
  Gui::WindowMain* parent_window_;
  std::string current_filepath_;
  int processed_files_;
  int total_files_;
  bool stop_processing_;
};

}  // namespace Files
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_FILES_H_
