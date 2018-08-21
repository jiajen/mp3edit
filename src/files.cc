#include "mp3edit/src/files.h"

#include <algorithm>
#include <filesystem>

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
    std::string path = entry.path();
    File::FileType filetype = File::getAudioExtension(path);
    if (filetype == File::FileType::kInvalid) continue;
    files_.emplace_back(path, filetype, read_audio_data);
    if (!files_.back()) {
      errors_.emplace_back(path, files_.back().getErrorMessage());
      files_.pop_back();
    }
  }
}

Files::Error::Error(const std::string& filepath,
                    const std::string& error_message) :
    filepath_(filepath), error_message_(error_message) {}

Files::Files() {}

Files::Files(const std::string& directory, bool recurse, bool read_audio_data) {
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
