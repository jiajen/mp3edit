#include "mp3edit/src/webservice/wikipedia.h"

namespace Mp3Edit {
namespace WebService {
namespace Wikipedia {

namespace {

const char* kWsHeader = "https://en.wikipedia.org/w/index.php?search=";
const char* kWsTitleField = " (song)";
const char* kWsAlbumField = " (album)";
const char* kWsTitleArtistPreField = " (";
const char* kWsTitleArtistPostField = " song)";
const char* kWsAlbumArtistPreField = " (";
const char* kWsAlbumArtistPostField = " album)";

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
  } else {
    return kWsHeader + title_filename + kWsTitleField;
  }
}

}  // Wikipedia
}  // WebService
}  // Mp3Edit
