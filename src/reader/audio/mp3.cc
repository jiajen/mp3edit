#include "mp3edit/src/reader/audio/mp3.h"

namespace Mp3Edit {
namespace ReaderAudio {
namespace Mp3 {

namespace {

using Filesystem::readBytes;

const int kFrameHeaderLength = 4;

}  // namespace

bool getAudioProperties(Filesystem::FileStream& file_stream,
                        int seek, int audio_end,
                        File::BitrateType& bitrate_type, int& bitrate,
                        int& sampling_rate, File::ChannelMode& channel_mode) {
  Bytes header;
  while (seek < audio_end) {
    readBytes(file_stream, seek, kFrameHeaderLength, header);
    // TODO read each frame
  }

  // TODO compile bitrate
  return (seek == audio_end);
}

}  // namespace Mp3
}  // namespace ReaderAudio
}  // namespace Mp3Edit
