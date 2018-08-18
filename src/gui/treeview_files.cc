#include "mp3edit/src/gui/treeview_files.h"

#include <gtkmm/liststore.h>

namespace Mp3Edit {
namespace Gui {

namespace {

const char* kTitle = "Title";
const char* kArtist = "Artist";
const char* kAlbum = "Album";
const char* kTrack = "Track";
const char* kBitrate = "Bitrate";
const char* kSamplingRate = "Sampling Rate";
const char* kChannelMode = "Channel Mode";
const char* kFilepath = "Path";

}  // namespace

TreeViewFiles::Columns::Columns() {
  add(title_);
  add(artist_);
  add(album_);
  add(track_);
  add(bitrate_);
  add(sampling_rate_);
  add(channel_mode_);
  add(filepath_);
  add(pos_);
}

TreeViewFiles::TreeViewFiles(BaseObjectType* cobject,
                             const Glib::RefPtr<Gtk::Builder>&,
                             std::vector<File::File>& files)
    : Gtk::TreeView(cobject), files_(files) {
  liststore_ = Gtk::ListStore::create(columns_);
  this->set_model(liststore_);
  this->append_column(kTitle, columns_.title());
  this->append_column(kArtist, columns_.artist());
  this->append_column(kAlbum, columns_.album());
  this->append_column(kTrack, columns_.track());
  this->append_column(kBitrate, columns_.bitrate());
  this->append_column(kSamplingRate, columns_.samplingRate());
  this->append_column(kChannelMode, columns_.channelMode());
  this->append_column(kFilepath, columns_.filepath());

}

void TreeViewFiles::populateTreeView() {
  // TODO fill
}

}  // namespace Gui
}  // namespace Mp3Edit
