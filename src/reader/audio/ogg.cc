#include "mp3edit/src/reader/audio/ogg.h"

namespace Mp3Edit {
namespace ReaderAudio {
namespace Ogg {

namespace {

const int kVorbisIdHeaderPos = 35;
const int kVorbisIdHeaderSize = 23;

const int kVersion = 0;
const int kVersionPos = 0;
const int kVersionSize = 4;
const int kNumChannelsPos = 4;
const int kNumChannelsSize = 1;
const int kSamplingRatePos = 5;
const int kSamplingRateSize = 4;
const int kBitrateMaxPos = 9;
const int kBitrateMaxSize = 4;
const int kBitrateNominalPos = 13;
const int kBitrateNominalSize = 4;
const int kBitrateMinPos = 17;
const int kBitrateMinSize = 4;

const unsigned char kFramingFlag = '\x01';
const int kFramingFlagPos = 22;
const int kFramingFlagSize = 1;

}  // namespace

bool getAudioProperties(Filesystem::FileStream& file_stream,
                        int seek, int audio_end,
                        File::BitrateType& bitrate_type, int& bitrate,
                        int& sampling_rate, File::ChannelMode& channel_mode) {
  // TODO
  return true;
}

}  // namespace Ogg
}  // namespace ReaderAudio
}  // namespace Mp3Edit
