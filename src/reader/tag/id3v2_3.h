#ifndef MP3EDIT_SRC_READER_TAG_ID3V2_3_H_
#define MP3EDIT_SRC_READER_TAG_ID3V2_3_H_

#include "mp3edit/src/bytes.h"
#include "mp3edit/src/filesystem.h"
#include "mp3edit/src/reader/tag/id3v2.h"

namespace Mp3Edit {
namespace ReaderTag {
namespace Id3v2_3 {

class Tag {
 public:
  Tag(Filesystem::FileStream &file_stream, int seek_start, int seek_end);
  Bytes generateTagHeader();
  inline std::string getTitle() const { return title_; }
  inline std::string getArtist() const { return artist_; }
  inline std::string getAlbum() const { return album_; }
  inline int getTrackNum() const { return track_num_; }
  inline int getTrackDenum() const { return track_denum_; }
 private:
  std::string title_;
  std::string artist_;
  std::string album_artist_;
  std::string album_;
  int track_num_;
  int track_denum_;
};

}  // Id3v2_3
}  // ReaderTag
}  // Mp3Edit

#endif  // MP3EDIT_SRC_READER_TAG_ID3V2_3_H_
