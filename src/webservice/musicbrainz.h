#ifndef MP3EDIT_WEBSERVICE_MUSICBRAINZ_H_
#define MP3EDIT_WEBSERVICE_MUSICBRAINZ_H_

#include <string>

namespace Mp3Edit {
namespace WebService {
namespace MusicBrainz {

std::string generateSearchUrl(const std::string& title_filename,
                              const std::string& artist,
                              const std::string& album,
                              const std::string& track_num,
                              const std::string& track_denum);

}  // namespace MusicBrainz
}  // namespace WebService
}  // namespace Mp3Edit

#endif  // MP3EDIT_WEBSERVICE_MUSICBRAINZ_H_
