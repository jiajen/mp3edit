#include "mp3edit/src/webservice/wikipedia.h"

namespace Mp3Edit {
namespace WebService {
namespace Wikipedia {

namespace {

const char* kWsHeader = "https://en.wikipedia.org/w/index.php?search=";

#define TITLE "song"
#define ALBUM "album"
#define TYPE_PRE " \\("
#define TYPE_POST "\\)"
#define REGEX_PRE "( \\(("
#define REGEX_POSTPRE " |)"
#define REGEX_POSTPOST "\\)|)"
const char* kWsTitleField = TYPE_PRE TITLE TYPE_POST;
const char* kWsAlbumField = TYPE_PRE ALBUM TYPE_POST;
const char* kWsTitleArtistPreField = REGEX_PRE;
const char* kWsTitleArtistPostField = REGEX_POSTPRE TITLE REGEX_POSTPOST;
const char* kWsAlbumArtistPreField = REGEX_PRE;
const char* kWsAlbumArtistPostField = REGEX_POSTPRE ALBUM REGEX_POSTPOST;
#undef REGEX_POSTPOST
#undef REGEX_POSTPRE
#undef REGEX_PRE
#undef TYPE_POST
#undef TYPE_PRE
#undef ALBUM
#undef TITLE

}  // namespace

std::string generateSearchUrl(const std::string& title_filename,
                              const std::string& artist,
                              const std::string& album,
                              const std::string&, const std::string&) {
  if (!album.empty()) {
    if (!artist.empty()) {
      return kWsHeader + album + kWsAlbumArtistPreField + artist +
                                 kWsAlbumArtistPostField;
    } else {
      return kWsHeader + album + kWsAlbumField;
    }
  } else if (!artist.empty()) {
    return kWsHeader + title_filename + kWsTitleArtistPreField + artist +
                                        kWsTitleArtistPostField;
  } else if (!title_filename.empty()) {
    return kWsHeader + title_filename + kWsTitleField;
  } else {
    return std::string();
  }
}

}  // Wikipedia
}  // WebService
}  // Mp3Edit
