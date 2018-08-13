#ifndef MP3EDIT_SRC_FILE_H_
#define MP3EDIT_SRC_FILE_H_

#include <string>

#include "mp3edit/src/bytes.h"
#include "mp3edit/src/filesystem.h"

namespace Mp3Edit {
namespace File {

enum class FileType {
  kMp3 = 0,
  kFlac = 1,
  kOgg = 2,
  kInvalid = 3, // kInvalid must be last
};

enum class BitrateType {
  kConstant = 0,
  kVbr = 1,
  kLossless = 2,
  kInvalid = 3,
};

enum class ChannelMode {
  kStereo = 0,
  kJointStereo = 1,
  kDualChannel = 2,
  kMono = 3,
  kInvalid = 4,
};

FileType getAudioExtension(const std::string& filename);

class File {
 public:
  File(const std::string& filepath, FileType filetype, bool read_audio_data);
  inline std::string getTitle() const { return title_; }
  inline std::string getArtist() const { return artist_; }
  inline std::string getAlbum() const { return album_; }
  inline int getTrackNum() const { return track_num_; }
  inline int getTrackDenum() const { return track_denum_; }
  inline std::string getFilepath() const { return filepath_; }
  void saveFileChanges(bool rename_file);
 private:
  void readMetaData(Filesystem::FileStream& file_stream);
  // Sets is_valid_ to false if file appears invalid
  void readAudioData(Filesystem::FileStream& file_stream);

  // Assumes file_stream points to filepath_.
  // Responsible for closing the stream.
  bool writeFile(const Bytes& audio_raw, const Bytes& metadata_front,
                 const Bytes& metadata_back, const std::string& new_filename);

  std::string title_;
  std::string artist_;
  std::string album_;
  int track_num_;
  int track_denum_;

  std::string filepath_;
  FileType filetype_;
  int filesize_;
  int audio_start_;
  int audio_end_;

  BitrateType bitrate_type_;
  int bitrate_;
  int sampling_rate_;
  ChannelMode channel_mode_;

  // For ogg and flac metadata generation.
  // Some files have stray tags outside the container.
  // This points to the start of the container.
  // This can =/= to audio_start_ as the container can contain metadata.
  int file_container_start_seek_;

  // True if valid audio (e.g. reading of properties appeared valid)
  bool is_valid_;
};

}  // namespace File
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_FILE_H_
