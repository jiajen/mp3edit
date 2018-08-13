#include "mp3edit/src/reader/audio/mp3.h"

namespace Mp3Edit {
namespace ReaderAudio {
namespace Mp3 {

bool getAudioProperties(Filesystem::FileStream& file_stream,
                        int seek, int audio_end,
                        File::BitrateType& bitrate_type, int& bitrate,
                        int& sampling_rate, File::ChannelMode& channel_mode) {
  while (seek < audio_end) {
    // TODO
  }
  // TODO
}

}  // namespace Mp3
}  // namespace ReaderAudio
}  // namespace Mp3Edit
