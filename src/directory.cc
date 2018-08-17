#include "mp3edit/src/directory.h"

#include "mp3edit/src/file.h"

#include <algorithm>
#include <filesystem>

namespace Mp3Edit {
namespace Directory {

namespace {

template <class T>
void pushFileIfValid(const T& entry, bool read_audio_data,
                     std::vector<File::File>& files) {
  if (entry.is_regular_file())
    pushFileIfValid((std::string)entry.path(), read_audio_data, files);
}

template <>
void pushFileIfValid<std::string>(const std::string& path, bool read_audio_data,
                                  std::vector<File::File>& files) {
  File::FileType filetype = File::getAudioExtension(path);
  if (filetype != File::FileType::kInvalid)
    files.emplace_back(path, filetype, read_audio_data);
}

}  // namespace

std::vector<File::File> getFiles(const std::string& directory,
                                 bool recurse,
                                 bool read_audio_data) {
  using std::filesystem::directory_iterator;
  using std::filesystem::recursive_directory_iterator;

  std::vector<File::File> files;
  if (recurse) {
    for (const auto& entry: recursive_directory_iterator(directory))
      pushFileIfValid(entry, read_audio_data, files);
  } else {
    for (const auto& entry: directory_iterator(directory))
      pushFileIfValid(entry, read_audio_data, files);
  }

  using File::File;
  sort(files.begin(), files.end(), [](const File& lhs, const File& rhs) {
    return lhs.getFilepath() < rhs.getFilepath();
  });

  return files;
}

}  // namespace Directory
}  // namespace Mp3Edit
