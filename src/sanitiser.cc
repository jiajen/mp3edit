#include "mp3edit/src/sanitiser.h"

namespace Mp3Edit {
namespace Sanitiser {

namespace {

const char kMinValidChar = 32;
const char kMaxValidChar = 126;
const char* kInvalidFilenameChars = "<>:\"/\\|?*";
const char kMinValidNumericChar = 48;
const char kMaxValidNumericChar = 57;

}  // namespace

bool sanitiseIntegerString(std::string& str) {
  bool changed = false;
  std::string valid_str;
  for (const char& c: str) {
    if (c >= kMinValidNumericChar && c <= kMaxValidNumericChar) {
      valid_str.push_back(c);
    } else {
      changed = true;
    }
  }
  str = valid_str;
  return changed;
}

std::string toValidFilename(const std::string& str) {
  std::string filename, sanitised_filename = str;
  sanitiseString(sanitised_filename);

  size_t strt_idx = 0, nxt_idx, size = sanitised_filename.length();
  do {
    nxt_idx = sanitised_filename.find_first_of(kInvalidFilenameChars, strt_idx);
    if (nxt_idx == std::string::npos) nxt_idx = size;
    if (nxt_idx - strt_idx > 0)
      filename.append(sanitised_filename.data() + strt_idx,
                      sanitised_filename.data() + nxt_idx);
    strt_idx = nxt_idx + 1;
  } while (nxt_idx != size);

  return filename;
}

bool sanitiseTrack(int& track_num, int& track_denum) {
  bool changed = false;
  if (track_num != -1 && track_num < 1) {
    track_num = -1;
    changed = true;
  }
  if (track_denum != -1 && track_denum < 1) {
    track_denum = -1;
    changed = true;
  }
  if (track_num == -1 && track_denum != -1) {
    track_denum = -1;
    changed = true;
  }
  if (track_num != -1 && track_denum != -1 && track_num > track_denum) {
    track_denum = -1;
    changed = true;
  }
  return changed;
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
}  // namespace Mp3Edit
