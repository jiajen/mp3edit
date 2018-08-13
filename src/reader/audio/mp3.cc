#include "mp3edit/src/reader/audio/mp3.h"

namespace Mp3Edit {
namespace ReaderAudio {
namespace Mp3 {

bool getAudioProperties(Filesystem::FileStream& file_stream,
                        int seek, int filesize,
                        File::BitrateType& bitrate_type, int& bitrate,
                        int& sampling_rate, File::ChannelMode& channel_mode) {
  // TODO
}

}  // namespace Mp3
}  // namespace ReaderAudio
}  // namespace Mp3Edit
