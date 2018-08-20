#ifndef MP3EDIT_SRC_FILES_H_
#define MP3EDIT_SRC_FILES_H_

#include <string>
#include <vector>

#include "mp3edit/src/file.h"

namespace Mp3Edit {
namespace Files {

// TODO
class Files {
 public:
 private:
};

std::vector<File::File> getFiles(const std::string& directory,
                                 bool recurse,
                                 bool read_audio_data);

}  // namespace Files
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_FILES_H_
