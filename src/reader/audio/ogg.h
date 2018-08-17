#ifndef MP3EDIT_SRC_READER_AUDIO_OGG_H_
#define MP3EDIT_SRC_READER_AUDIO_OGG_H_

#include "mp3edit/src/filesystem.h"
#include "mp3edit/src/file.h"

namespace Mp3Edit {
namespace ReaderAudio {
namespace Ogg {

bool getAudioProperties(Filesystem::FileStream& file_stream,
                        int seek, int,
                        File::BitrateType& bitrate_type, int& bitrate,
                        int& sampling_rate, File::ChannelMode& channel_mode);

}  // namespace Ogg
}  // namespace ReaderAudio
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_READER_AUDIO_OGG_H_
