#ifndef MP3EDIT_SRC_DIRECTORY_H_
#define MP3EDIT_SRC_DIRECTORY_H_

#include <string>
#include <vector>

#include "mp3edit/src/file.h"

namespace Mp3Edit {
namespace Directory {

std::vector<File::File> getFiles(const std::string& directory,
                                 bool read_audio_data);

}  // namespace Directory
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_DIRECTORY_H_
