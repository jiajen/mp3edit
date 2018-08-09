#ifndef MP3EDIT_SRC_SANITISER_H_
#define MP3EDIT_SRC_SANITISER_H_

#include <string>

namespace Mp3Edit {
namespace Sanitiser {

bool sanitiseIntegerString(std::string& str);

// Returns a filename string that is valid in both linux and windows.
std::string toValidFilename(const std::string& str);

// Returns true if string was changed.
bool sanitiseString(std::string& str);

}  // namespace Sanitiser
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_SANITISER_H_
