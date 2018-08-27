#include "mp3edit/src/reader/audio/mp3.h"

#include <cstring>

namespace Mp3Edit {
namespace ReaderAudio {
namespace Mp3 {

namespace {

using Filesystem::readBytes;

const int kFrameHeaderLength = 4;

const int kSyncPos = 0;
const int kSyncSize = 2;
const char* kSyncBitMask = "\xFF\xE0";

// Used to remember uninitialised values and invalid values.
const int kInvalidValue = -1;
const int kUnsetValue = -2;

const int kVersionPos = 1;
const char kVersionBitMask = '\x18';
const int kVersionShift = 3;
enum class MpegVersion {
  kUnset = kUnsetValue,
  kInvalid = kInvalidValue,
  kV2_5 = 0,
  kReserved = 1,
  kV2 = 2,
  kV1 = 3,
};

const int kLayerPos = 1;
const char kLayerBitMask = '\x06';
const int kLayerShift = 1;
enum class Layer {
  kUnset = kUnsetValue,
  kInvalid = kInvalidValue,
  kReserved = 0,
  kIII = 1,
  kII = 2,
  kI = 3,
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
const int kSamplingTable[][4] = { // [Version][Sampling]
  {11025, 12000, 8000, -1},
  {-1, -1, -1, -1},
  {22050, 24000, 16000, -1},
  {44100, 48000, 32000, -1}
};

const int kPaddingPos = 2;
const char kPaddingBitMask = '\x02';

const int kChannelPos = 3;
const char kChannelBitMask = '\xC0';
const int kChannelShift = 6;
enum class ChannelMode {
  kUnset = kUnsetValue,
  kInvalid = kInvalidValue,
  kStereo = 0,
  kJointStereo = 1,
  kDualChannel = 2,
  kMono = 3,
};

const int kCrcPos = 1;
const char kCrcBitMask = '\x01';
const int kCrcLength = 2;

const char* kXingHeaderVbr = "Xing";
const char* kXingHeaderCbr = "Info";
const int kXingHeaderPreambleLength = 4;

const int kSideInfoLengthV1Stereo = 32;
const int kSideInfoLengthV1Mono = 17;
const int kSideInfoLengthV2Stereo = 17;
const int kSideInfoLengthV2Mono = 9;

// Layer II only allows certain combinations of channel mode and bitrates.
bool checkIsValidBitrate(int bitrate, Layer layer, ChannelMode channel_mode) {
  if (layer != Layer::kII) return true;
  if (channel_mode == ChannelMode::kMono) {
    if (bitrate > 192) return false;
  } else {
    if (bitrate == 80 || (bitrate >= 32 && bitrate <= 56)) return false;
  }
  return true;
}

bool checkValidSync(const Bytes& header) {
  for (int i = 0; i < kSyncSize; i++) {
    if ((header[kSyncPos+i]&kSyncBitMask[i]) != (unsigned char)kSyncBitMask[i])
      return false;
  }
  return true;
}

inline int extractVal(const Bytes& header, int pos, char bitmask, int shift) {
  return ((header[pos]&bitmask) >> shift);
}

template<class T>
bool getVal(T val_new, T& val) {
  if (val_new != val) {
    if (val == T::kUnset) {
      val = (T)val_new;
    } else {
      return false;
    }
  }
  return true;
}

bool getVersion(const Bytes& header, MpegVersion& version) {
  MpegVersion val = (MpegVersion)extractVal(header, kVersionPos,
                                            kVersionBitMask, kVersionShift);
  if (val == MpegVersion::kReserved) return false;
  return getVal(val, version);
}

bool getLayer(const Bytes& header, Layer& layer) {
  Layer val = (Layer)extractVal(header, kLayerPos,
                                kLayerBitMask, kLayerShift);
  if (val == Layer::kReserved) return false;
  return getVal(val, layer);
}

bool getBitrate(const Bytes& header, MpegVersion version, Layer layer,
                ChannelMode channel_mode, int& bitrate, long long& bitrate_sum,
                File::BitrateType& bitrate_type) {
  using File::BitrateType;

  int idx = extractVal(header, kBitratePos, kBitrateBitMask, kBitrateShift);
  const int* idx_table = kBitrateRefTable[(int)version][(int)layer];
  int frame_bitrate = *(idx_table + idx);
  if (frame_bitrate == -1) return false;
  if (!checkIsValidBitrate(frame_bitrate, layer, channel_mode)) return false;

  if (bitrate == kUnsetValue) {
    bitrate_type = BitrateType::kConstant;
  } else if (bitrate_type != BitrateType::kVbr && frame_bitrate != bitrate) {
    bitrate_type = BitrateType::kVbr;
  }

  bitrate = frame_bitrate;
  bitrate_sum += frame_bitrate;
  return true;
}

bool getSampling(const Bytes& header, MpegVersion version, int& sampling_rate) {
  int idx = extractVal(header, kSamplingPos,
                       kSamplingBitMask, kSamplingShift);
  int frame_sampling_rate = kSamplingTable[(int)version][idx];

  if (frame_sampling_rate == -1) return false;
  if (sampling_rate == kUnsetValue) {
    sampling_rate = frame_sampling_rate;
  } else if (sampling_rate != frame_sampling_rate) {
    return false;
  }
  return true;
}

bool hasPadding(const Bytes& header) {
  return ((header[kPaddingPos]&kPaddingBitMask) > 0);
}

bool getChannelMode(const Bytes& header, ChannelMode& channel_mode) {
  ChannelMode val = (ChannelMode)extractVal(header, kChannelPos,
                                            kChannelBitMask, kChannelShift);
  return getVal(val, channel_mode);
}

// Returns true if Xing frame was found.
bool skipXingFrame(const Bytes& header, Filesystem::FileStream& file_stream,
                   MpegVersion version, ChannelMode channel_mode, Layer layer,
                   int seek) {
  const int len1 = kSideInfoLengthV1Stereo;
  const int len2 = kSideInfoLengthV1Mono;
  const int len3 = kSideInfoLengthV2Mono;

  if (layer == Layer::kReserved || layer == Layer::kII) return false;
  int offset;
  if (version == MpegVersion::kV1) {
    offset = (channel_mode == ChannelMode::kMono) ? len2 : len1;
  } else if (version == MpegVersion::kV2 || version == MpegVersion::kV2_5) {
    offset = (channel_mode == ChannelMode::kMono) ? len3 : len2;
  } else {
    return false;
  }
  offset += kFrameHeaderLength;

  if ((header[kCrcPos]&kCrcBitMask) == 0) offset += kCrcLength;

  Bytes xing_header;
  readBytes(file_stream, seek+offset, kXingHeaderPreambleLength, xing_header);
  const char* header_ptr = (const char*)xing_header.data();
  return (strncmp(header_ptr, kXingHeaderCbr, kXingHeaderPreambleLength) == 0 ||
          strncmp(header_ptr, kXingHeaderVbr, kXingHeaderPreambleLength) == 0);
}

}  // namespace

bool getAudioProperties(Filesystem::FileStream& file_stream,
                        int seek, int& audio_end,
                        File::BitrateType& bitrate_type, int& bitrate,
                        int& sampling_rate, File::ChannelMode& channel_mode) {
  MpegVersion version = MpegVersion::kUnset;
  Layer layer = Layer::kUnset;
  sampling_rate = kUnsetValue;
  bitrate = kUnsetValue;
  ChannelMode channel_mode_read = ChannelMode::kUnset;

  Bytes header;
  bitrate_type = File::BitrateType::kInvalid;
  int n_frames = 0;
  long long bitrate_sum = 0;
  bool is_first_frame = true;
  for (int size; seek < audio_end; seek += size) {
    readBytes(file_stream, seek, kFrameHeaderLength, header);
    if (!checkValidSync(header)) {
      if (n_frames > 1) {  // If false sync, n_frames is likely to be 1.
        audio_end = seek;
        break;
      } else {
        return false;
      }
    }
    if (!getVersion(header, version)) return false;
    if (!getLayer(header, layer)) return false;
    if (!getSampling(header, version, sampling_rate)) return false;
    if (!getChannelMode(header, channel_mode_read)) return false;
    if (!getBitrate(header, version, layer, channel_mode_read,
                    bitrate, bitrate_sum, bitrate_type))
      return false;
    size = (144000 * bitrate) / sampling_rate;
    if (hasPadding(header)) size++;
    n_frames++;
    if (is_first_frame) {
      is_first_frame = false;
      if (skipXingFrame(header, file_stream, version,
                        channel_mode_read, layer, seek)) {
        version = MpegVersion::kUnset;
        layer = Layer::kUnset;
        sampling_rate = kUnsetValue;
        bitrate = kUnsetValue;
        channel_mode_read = ChannelMode::kUnset;
        bitrate_type = File::BitrateType::kInvalid;
        bitrate_sum = 0;
        n_frames = 0;
      }
    }
  }

  if (bitrate_type == File::BitrateType::kVbr)
    bitrate = (bitrate_sum)/n_frames;

  switch (channel_mode_read) {
    case ChannelMode::kStereo:
      channel_mode = File::ChannelMode::kStereo;
      break;
    case ChannelMode::kJointStereo:
      channel_mode = File::ChannelMode::kJointStereo;
      break;
    case ChannelMode::kDualChannel:
      channel_mode = File::ChannelMode::kDualChannel;
      break;
    case ChannelMode::kMono:
      channel_mode = File::ChannelMode::kMono;
      break;
    default:  // kUnset
      return false;
      break;
  }
  if (version == MpegVersion::kUnset) return false;
  if (layer == Layer::kUnset) return false;
  if (bitrate == kUnsetValue) return false;
  if (sampling_rate == kUnsetValue) return false;

  return (seek == audio_end);
}

}  // namespace Mp3
}  // namespace ReaderAudio
}  // namespace Mp3Edit
