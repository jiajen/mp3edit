#include "mp3edit/src/webservice/musicbrainz.h"

namespace Mp3Edit {
namespace WebService {
namespace MusicBrainz {

namespace {

const char* kWsHeader = "https://musicbrainz.org/search?";
const char* kWsParams = "type=recording&limit=10&method=advanced&query="
                        "(primarytype:album OR primarytype:single)";
const char* kWsFieldSeparator = " AND ";
const char* kWsTitleField = "recording:";
const char* kWsArtistField = "artist:";
const char* kWsAlbumField = "release:";
const char* kWsTrackNumField = "tnum:";
const char* kWsTrackDenumField = "tracks:";

void appendField(const char* field_name, const std::string& field_data,
                 std::string& query) {
  if (field_data.empty()) return;
  query += std::string(kWsFieldSeparator) + field_name + field_data;
}

}  // namespace

std::string generateSearchUrl(const std::string& title_filename,
                              const std::string& artist,
                              const std::string& album,
                              const std::string& track_num,
                              const std::string& track_denum) {
  bool blank_num, blank_denum;
  std::string query;
  appendField(kWsTitleField, title_filename, query);
  appendField(kWsArtistField, artist, query);
  appendField(kWsAlbumField, album, query);
  blank_num = (track_num == "1" && (track_denum.empty() || track_denum == "1"));
  blank_denum = (track_denum == "1");
  appendField(kWsTrackNumField, blank_num ? "" : track_num, query);
  appendField(kWsTrackDenumField, blank_denum ? "" : track_denum, query);
  if (query.empty()) return "";
  return std::string(kWsHeader) + std::string(kWsParams) + query;
}

}  // namespace MusicBrainz
}  // namespace WebService
}  // namespace Mp3Edit
