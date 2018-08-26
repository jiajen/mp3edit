#ifndef MP3EDIT_SRC_WEBSERVICE_WIKIPEDIA_H_
#define MP3EDIT_SRC_WEBSERVICE_WIKIPEDIA_H_

#include <string>

namespace Mp3Edit {
namespace WebService {
namespace Wikipedia {

std::string generateSearchUrl(const std::string& title_filename,
                              const std::string& artist,
                              const std::string& album,
                              const std::string&, const std::string&);

}  // Wikipedia
}  // WebService
}  // Mp3Edit

#endif  // MP3EDIT_SRC_WEBSERVICE_WIKIPEDIA_H_
