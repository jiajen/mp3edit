#ifndef MP3EDIT_SRC_WEBSERVICE_MUSICBRAINZ_H_
#define MP3EDIT_SRC_WEBSERVICE_MUSICBRAINZ_H_

#include <string>

namespace Mp3Edit {
namespace WebService {
namespace MusicBrainz {

std::string generateSearchUrl(const std::string& title_filename,
                              const std::string& artist,
                              const std::string& album,
                              const std::string& track_num,
                              const std::string& track_denum);

}  // MusicBrainz
}  // WebService
}  // Mp3Edit

#endif  // MP3EDIT_SRC_WEBSERVICE_MUSICBRAINZ_H_
