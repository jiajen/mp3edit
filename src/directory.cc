#include "mp3edit/src/directory.h"

#include "mp3edit/src/file.h"

#include <algorithm>
#include <filesystem>

namespace Mp3Edit {
namespace Directory {

std::vector<File::File> getFiles(const std::string& directory,
                                 bool read_audio_data) {
  std::vector<File::File> files;
  for (const auto& entry: std::filesystem::directory_iterator(directory)) {
    if (!entry.is_regular_file()) continue;
    std::string path = (std::string)entry.path();
    File::FileType filetype = File::getAudioExtension(path);
    if (filetype != File::FileType::kInvalid)
      files.emplace_back(path, filetype, read_audio_data);
  }

  using File::File;
  sort(files.begin(), files.end(), [](const File& lhs, const File& rhs) {
    return lhs.getFilepath() < rhs.getFilepath();
  });

  return files;
}

}  // namespace Directory
}  // namespace Mp3Edit
