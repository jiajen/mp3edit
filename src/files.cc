#include "mp3edit/src/files.h"

#include <algorithm>
#include <filesystem>

#include "mp3edit/src/gui/window_main.h"

namespace Mp3Edit {
namespace Files {

namespace {

class DirectoryEntry {
 public:
  DirectoryEntry(const std::string& path, File::FileType& filetype)
      : filetype_(filetype), path_(path) {}
  inline File::FileType getFiletype() const { return filetype_; }
  inline const std::string& getPath() const { return path_; }
 private:
  File::FileType filetype_;
  std::string path_;
};

}  // namespace

template <class T>
inline void Files::readFiles(const std::string& directory,
                             bool read_audio_data, T& it) {
  try {
    it = T(directory);
  } catch (const std::filesystem::filesystem_error&) {
    return;
  }

  std::vector<DirectoryEntry> dir_entries;
  beginProgress(0);
  for (const auto& entry: it) {
    if (!entry.is_regular_file()) continue;
    std::string filepath = entry.path();
    File::FileType filetype = File::getAudioExtension(filepath);
    if (filetype == File::FileType::kInvalid) continue;
    if (!updateProgress(filepath, 0)) return;
    dir_entries.emplace_back(filepath, filetype);
  }

  beginProgress(dir_entries.size());
  for (int i = 0, n = dir_entries.size(); i < n; i++) {
    if (!updateProgress(dir_entries[i].getPath(), i+1)) return;
    files_.emplace_back(dir_entries[i].getPath(), dir_entries[i].getFiletype(),
                        read_audio_data);
    if (!files_.back()) {
      errors_.emplace_back(dir_entries[i].getPath(),
                           files_.back().getErrorMessage());
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

Files::Files(Gui::WindowMain* parent_window): parent_window_(parent_window) {}

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

void Files::saveFile(int idx, bool rename_file, bool is_single_file) {
  if (is_single_file) {
    errors_.clear();
    beginProgress(1);
    updateProgress(files_[idx].getFilepath(), 0);
  }
  files_[idx].saveFileChanges(rename_file);
  if (!files_[idx]) {
    errors_.emplace_back(files_[idx].getFilepath(),
                         files_[idx].getErrorMessage());
  }
  if (is_single_file) updateProgress(files_[idx].getFilepath(), 1);
}

void Files::saveAllFiles(bool rename_file) {
  errors_.clear();

  int total_files = 0;
  for (const auto& file: files_) {
    if (file) total_files++;
  }
  beginProgress(total_files);

  int processing_file_idx = 0;
  for (int i = 0, n = files_.size(); i < n; i++) {
    if (files_[i]) {
      if (!updateProgress(files_[i].getFilepath(), ++processing_file_idx))
        return;
      saveFile(i, rename_file, false);
    }
  }
}

void Files::stopOperation() {
  std::lock_guard<std::mutex> lock(mutex_);
  stop_processing_ = true;
}

void Files::beginProgress(int total_files) {
  std::lock_guard<std::mutex> lock(mutex_);
  stop_processing_ = false;
  total_files_ = total_files;
}

bool Files::updateProgress(const std::string& filepath, int processing_file) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (stop_processing_) {
    processed_files_ = processing_file-1;
    return false;
  }
  current_filepath_ = filepath;
  processed_files_ = processing_file;
  parent_window_->notifyProgressChange();
  return true;
}

}  // namespace Files
}  // namespace Mp3Edit
