#include "mp3edit/src/files.h"

#include <algorithm>
#include <filesystem>

namespace Mp3Edit {
namespace Files {

namespace {

template <class T>
inline std::vector<File::File> getFiles(const std::string& directory,
                                        bool read_audio_data, T& it) {
  std::vector<File::File> files;
  try {
    it = T(directory);
  } catch (const std::filesystem::filesystem_error&) {
    return files;
  }
  for (const auto& entry: it) {
    if (!entry.is_regular_file()) continue;
    std::string path = entry.path();
    File::FileType filetype = File::getAudioExtension(path);
    if (filetype == File::FileType::kInvalid) continue;
    files.emplace_back(path, filetype, read_audio_data);
    if (!files.back()) files.pop_back();
  }
  return files;
}

}  // namespace

Files::Files() {
  // TODO
}

Files::Files(const std::string& directory, bool recurse, bool read_audio_data) {
  // TODO
  if (recurse) {
    std::filesystem::recursive_directory_iterator it;
    files_ = getFiles(directory, read_audio_data, it);
  } else {
    std::filesystem::directory_iterator it;
    files_ = getFiles(directory, read_audio_data, it);
  }
}

}  // namespace Files
}  // namespace Mp3Edit
