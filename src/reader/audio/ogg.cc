#include "mp3edit/src/reader/audio/ogg.h"
#include "mp3edit/src/reader/utility.h"

namespace Mp3Edit {
namespace ReaderAudio {
namespace Ogg {

namespace {

using Filesystem::readBytes;
using Reader::Utility::lEndianToInt;

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

}  // namespace

bool getAudioProperties(Filesystem::FileStream& file_stream,
                        int seek, int audio_end,
                        File::BitrateType& bitrate_type, int& bitrate,
                        int& sampling_rate, File::ChannelMode& channel_mode) {
  Bytes header;
  readBytes(file_stream, seek+kVorbisIdHeaderPos, kVorbisIdHeaderSize, header);
  int version = lEndianToInt(header.begin() + kVersionPos,
                             header.begin() + kVersionPos + kVersionSize,
                             false);
  if (version != kVersion) return false;
  if (header[kFramingFlagPos] != kFramingFlag) return false;

  int num_channels = lEndianToInt(header.begin() + kNumChannelsPos,
                                  header.begin() + kNumChannelsPos +
                                                   kNumChannelsSize,
                                  false);

  sampling_rate = lEndianToInt(header.begin() + kSamplingRatePos,
                               header.begin() + kSamplingRatePos +
                                                kSamplingRateSize,
                               false);

  int bitrate_max = lEndianToInt(header.begin() + kBitrateMaxPos,
                                 header.begin() + kBitrateMaxPos +
                                                  kBitrateMaxSize,
                                 false);

  int bitrate_nominal = lEndianToInt(header.begin() + kBitrateNominalPos,
                                     header.begin() + kBitrateNominalPos +
                                                      kBitrateNominalSize,
                                     false);

  int bitrate_min = lEndianToInt(header.begin() + kBitrateMinPos,
                                 header.begin() + kBitrateMinPos +
                                                  kBitrateMinSize,
                                 false);
  // TODO convert ints to file types
  return true;
}

}  // namespace Ogg
}  // namespace ReaderAudio
}  // namespace Mp3Edit
