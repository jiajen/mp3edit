#ifndef MP3EDIT_SRC_FILES_H_
#define MP3EDIT_SRC_FILES_H_

#include <string>
#include <vector>
#include <mutex>

#include <glibmm/dispatcher.h>

#include "mp3edit/src/file.h"

namespace Mp3Edit {

namespace Files {

class Files {
 private:
  class Error;
 public:
  Files(Glib::Dispatcher* dispatcher);
  inline File::File& operator[](int idx) { return files_[idx]; }
  inline int size() const { return files_.size(); }
  inline const std::vector<Error>& getErrorList() const { return errors_; }
  void readDirectory(const std::string& directory, bool recurse,
                     bool read_audio_data);
  void saveFile(int idx, bool rename_file, bool is_single_file = true);
  // Only save files that are valid as invalidated files (due to save errors)
  // will still exist in the vector and in order.
  void saveAllFiles(bool rename_file);

  enum class ProcessingMode {
    kReady = 0,
    kReadMulti = 1,
    kSaveSingle = 2,
    kSaveMulti = 3,
  };
  ProcessingMode fileOperationStatus(int& processed_files, int& total_files,
                                     std::string& processing_file);
  // This must be called by the GUI thread before the start of an operation.
  void setOperation(ProcessingMode processing_mode);
  void stopOperation();
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

  // Mutexed functions
  void beginProgress(int total_files);
  // Check if operation has been stopped.
  bool updateProgress(const std::string& filepath, int processed_files_n);

  std::vector<Error> errors_;
  std::vector<File::File> files_;

  // Multi-threading
  std::mutex mutex_;
  Glib::Dispatcher* dispatcher_;
  std::string current_filepath_;
  int processed_files_;
  int total_files_;
  bool stop_processing_;
  ProcessingMode processing_mode_;
};

}  // namespace Files
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_FILES_H_
