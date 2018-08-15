#include "mp3edit/src/reader/audio/mp3.h"

namespace Mp3Edit {
namespace ReaderAudio {
namespace Mp3 {

namespace {

using Filesystem::readBytes;

const int kFrameHeaderLength = 4;

const int kSyncPos = 0;
const int kSyncSize = 2;
const char* kSyncBitMask = "\xFF\xE0";

const int kVersionPos = 1;
const char kVersionBitMask = '\x18';
const int kVersionShift = 3;
enum class MpegVersion {
  kV2_5 = 0,
  kReserved = 1,
  kV2 = 2,
  kV1 = 3,
  kUnset = 4,
  kInvalid = 5,
};

const int kLayerPos = 1;
const char kLayerBitMask = '\x06';
const int kLayerShift = 1;
enum class Layer {
  kReserved = 0,
  kIII = 1,
  kII = 2,
  kI = 3,
  kUnset = 4,
  kInvalid = 5,
};

const int kBitratePos = 2;
const char kBitrateBitMask = '\xF0';
const int kBitrateShift = 4;
const int kBitrateTable[][16] = {
  {-1, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, -1},
  {-1, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, -1},
  {-1, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, -1},
  {-1, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, -1},
  {-1, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, -1}
};
const int* kBitrateRefTable[][4] = { // [Version][Layer]
  {0,kBitrateTable[4],kBitrateTable[4],kBitrateTable[3]},
  {0,0,0,0},
  {0,kBitrateTable[4],kBitrateTable[4],kBitrateTable[3]},
  {0,kBitrateTable[2],kBitrateTable[1],kBitrateTable[0]}
};

const int kSamplingPos = 2;
const char kSamplingBitMask = '\x0C';
const int kSamplingShift = 2;
const int kFrequencyTable[][4] = { // [Version][Frequency]
  {11025, 12000, 8000, -1},
  {-1, -1, -1, -1},
  {22050, 24000, 16000, -1},
  {44100, 48000, 32000, -1}
};

const int kPaddingPos = 2;
const char kPaddingBitMask = '\x02';
const int kPaddingShift = 1;

const int kChannelPos = 3;
const char kChannelBitMask = '\xC0';
const int kChannelShift = 6;
enum class ChannelMode {
  kStereo = 0,
  kJointStereo = 1,
  kDualChannel = 2,
  kMono = 3,
  kUnset = 4,
  kInvalid = 5,
};

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
