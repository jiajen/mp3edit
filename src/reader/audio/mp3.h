#ifndef MP3EDIT_READER_AUDIO_MP3_H_
#define MP3EDIT_READER_AUDIO_MP3_H_

#include "mp3edit/src/filesystem.h"
#include "mp3edit/src/file.h"

namespace Mp3Edit {
namespace ReaderAudio {
namespace Mp3 {

// This changes audio_end to the end of the last frame if needed.
bool getAudioProperties(Filesystem::FileStream& file_stream,
                        int seek, int& audio_end,
                        File::BitrateType& bitrate_type, int& bitrate,
                        int& sampling_rate, File::ChannelMode& channel_mode);

}  // namespace Mp3
}  // namespace ReaderAudio
}  // namespace Mp3Edit

#endif  // MP3EDIT_READER_AUDIO_MP3_H_
