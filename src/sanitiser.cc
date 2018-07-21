#include "mp3edit/src/sanitiser.h"

namespace Mp3Edit {
namespace Id3 {
namespace Sanitiser {

namespace {

const char kMaxValidChar = 126;
const char kMinValidChar = 32;
const char* kInvalidFilenameChars = "<>:\"/\\|?*";

}  // namespace

std::string toValidFilename(const std::string& str) {
  std::string filename, sanitised_filename = str;
  sanitiseString(sanitised_filename);

  size_t strt_idx = 0, nxt_idx, size = str.length();
  do {
    nxt_idx = sanitised_filename.find_first_of(kInvalidFilenameChars, strt_idx);
    if (nxt_idx == std::string::npos) nxt_idx = size;
    if (nxt_idx - strt_idx > 0) filename.append(strt_idx, nxt_idx - strt_idx);
    strt_idx = nxt_idx + 1;
  } while (nxt_idx != size);

  return filename;
}

bool sanitiseString(std::string& str) {
  std::string valid_str, trimmed_str;
  valid_str.reserve(str.length());
  trimmed_str.reserve(str.length());

  for (const char& c: str) {
    if (c >= kMinValidChar && c <= kMaxValidChar) valid_str.push_back(c);
  }

  for (const char& c: valid_str) {
    if (c != ' ' || (!trimmed_str.empty() && trimmed_str.back() != ' '))
      trimmed_str.push_back(c);
  }
  if (!trimmed_str.empty() && trimmed_str.back() == ' ') trimmed_str.pop_back();

  bool changed = (trimmed_str != str);
  str = trimmed_str;
  return changed;
}

}  // namespace Sanitiser
}  // namespace Id3
}  // namespace Mp3Edit
