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
  add(pos_);
  add(title_);
  add(artist_);
  add(album_);
  add(track_);
  add(bitrate_);
  add(sampling_rate_);
  add(channel_mode_);
  add(filepath_);
}

TreeViewFiles::TreeViewFiles(BaseObjectType* cobject,
                             const Glib::RefPtr<Gtk::Builder>&,
                             std::vector<File::File>& files,
                             Gtk::Entry* entry_title, Gtk::Entry* entry_artist,
                             Gtk::Entry* entry_album,
                             Gtk::Entry* entry_track_num,
                             Gtk::Entry* entry_track_denum)
    : Gtk::TreeView(cobject), files_(files), entry_song_title_(entry_title),
      entry_song_artist_(entry_artist), entry_song_album_(entry_album),
      entry_song_track_num_(entry_track_num),
      entry_song_track_denum_(entry_track_denum) {
  treeselection_ = this->get_selection();
  treeselection_->signal_changed().connect(
    sigc::mem_fun(*this, &TreeViewFiles::onRowSelect));

  liststore_ = Gtk::ListStore::create(columns_);
  this->set_model(liststore_);

  appendColumnEditable(this, kTrack, columns_.track());
  appendColumnEditable(this, kTitle, columns_.title());
  appendColumnEditable(this, kArtist, columns_.artist());
  appendColumnEditable(this, kAlbum, columns_.album());
  appendColumn(this, kBitrate, columns_.bitrate());
  appendColumn(this, kSamplingRate, columns_.samplingRate());
  appendColumn(this, kChannelMode, columns_.channelMode());
  appendColumn(this, kFilepath, columns_.filepath());
}

void TreeViewFiles::populateTreeView() {
  liststore_->clear();
  int n = 0;
  for (const File::File& file: files_) {
    Gtk::TreeModel::Row row = *(liststore_->append());
    row[columns_.pos()] = n++;
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

void TreeViewFiles::onRowSelect() {
  Gtk::TreeModel::iterator it = treeselection_->get_selected();
  if (!it) return;
  Gtk::TreeModel::Row row = *it;
  entry_song_title_->set_text((Glib::ustring)row[columns_.title()]);
  entry_song_artist_->set_text((Glib::ustring)row[columns_.artist()]);
  entry_song_album_->set_text((Glib::ustring)row[columns_.album()]);
  // TODO handle num and denum by separating
}

}  // namespace Gui
}  // namespace Mp3Edit
