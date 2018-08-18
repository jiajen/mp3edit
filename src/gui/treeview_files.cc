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

inline void appendColumn(TreeViewFiles* tree_view, const char* column_title,
                         Gtk::TreeModelColumn<std::string>& column) {
  tree_view->get_column(tree_view->append_column(
    column_title, column)-1)->set_sort_column(column);
}

inline void appendColumnEditable(TreeViewFiles* tree_view,
                                 const char* column_title,
                                 Gtk::TreeModelColumn<std::string>& column) {
  tree_view->get_column(tree_view->append_column_editable(
    column_title, column)-1)->set_sort_column(column);
}

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
  appendColumnEditable(this, kTitle, columns_.title());
  appendColumnEditable(this, kArtist, columns_.artist());
  appendColumnEditable(this, kAlbum, columns_.album());
  appendColumnEditable(this, kTrack, columns_.track());
  appendColumn(this, kBitrate, columns_.bitrate());
  appendColumn(this, kSamplingRate, columns_.samplingRate());
  appendColumn(this, kChannelMode, columns_.channelMode());
  appendColumn(this, kFilepath, columns_.filepath());
}

void TreeViewFiles::populateTreeView() {
  for (const File::File& file: files_) {
    Gtk::TreeModel::Row row = *(liststore_->append());
    row[columns_.title()] = file.getTitle();
    row[columns_.artist()] = file.getArtist();
    row[columns_.album()] = file.getAlbum();
    row[columns_.track()] = file.getTrack();
    row[columns_.bitrate()] = file.getBitrate();
    row[columns_.samplingRate()] = file.getSamplingRate();
    row[columns_.channelMode()] = file.getChannelMode();
    row[columns_.filepath()] = file.getFilepath();
  }
}

}  // namespace Gui
}  // namespace Mp3Edit
