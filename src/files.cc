#include "mp3edit/src/files.h"

#include <algorithm>
#include <filesystem>

#include "mp3edit/src/gui/window_main.h"

namespace Mp3Edit {
namespace Files {

template <class T>
inline void Files::readFiles(const std::string& directory,
                             bool read_audio_data, T& it) {
  try {
    it = T(directory);
  } catch (const std::filesystem::filesystem_error&) {
    return;
  }
  for (const auto& entry: it) {
    if (!entry.is_regular_file()) continue;
    current_filepath_ = entry.path();
    File::FileType filetype = File::getAudioExtension(current_filepath_);
    if (filetype == File::FileType::kInvalid) continue;
    files_.emplace_back(current_filepath_, filetype, read_audio_data);
    if (!files_.back()) {
      errors_.emplace_back(current_filepath_, files_.back().getErrorMessage());
      files_.pop_back();
    }
  }
}

Files::Error::Error(const std::string& filepath,
                    const std::string& error_message) :
    filepath_(filepath), error_message_(error_message) {}

std::string Files::fileOperationStatus(int& processed_files, int& total_files) {
  std::lock_guard<std::mutex> lock(mutex_);
  processed_files = processed_files_;
  total_files = total_files_;
  return std::filesystem::path(current_filepath_).filename();
}

void Files::readDirectory(const std::string& directory, bool recurse,
                          bool read_audio_data) {
  errors_.clear();
  files_.clear();
  if (recurse) {
    std::filesystem::recursive_directory_iterator it;
    readFiles(directory, read_audio_data, it);
  } else {
    std::filesystem::directory_iterator it;
    readFiles(directory, read_audio_data, it);
  }
}

bool Files::saveFile(int idx, bool rename_file, bool clear_error_message) {
  if (clear_error_message) errors_.clear();
  files_[idx].saveFileChanges(rename_file);
  if (files_[idx]) return true;
  errors_.emplace_back(files_[idx].getFilepath(),
                       files_[idx].getErrorMessage());
  return false;
}

bool Files::saveAllFiles(bool rename_file) {
  errors_.clear();
  for (int i = 0, n = files_.size(); i < n; i++) {
    if (files_[i]) saveFile(i, rename_file, false);
  }
  return errors_.empty();
}

}  // namespace Files
}  // namespace Mp3Edit
