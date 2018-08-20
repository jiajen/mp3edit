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

}  // namespace Files
}  // namespace Mp3Edit
