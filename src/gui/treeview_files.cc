#include "mp3edit/src/gui/treeview_files.h"

#include <algorithm>

#include <gtkmm/cellrenderer.h>
#include <gtkmm/cellrenderertext.h>

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

void appendColumnEditable(TreeViewFiles* tree_view,
                          const char* column_title,
                          Gtk::TreeModelColumn<std::string>& column,
                          void (TreeViewFiles::*function_ptr)(
                            const Glib::ustring&, const Glib::ustring&)) {
  int idx = tree_view->append_column_editable(column_title, column)-1;
  tree_view->get_column(idx)->set_sort_column(column);

  Gtk::CellRenderer* renderer = tree_view->get_column_cell_renderer(idx);
  Gtk::CellRendererText* renderer_text = (Gtk::CellRendererText*)(renderer);
  renderer_text->signal_edited().connect(
    sigc::mem_fun(*tree_view, function_ptr));
}

void addSignalsToFieldEntry(TreeViewFiles* tree_view,
                            void (TreeViewFiles::*fx_changed)(),
                            void (TreeViewFiles::*fx_activated)(),
                            Gtk::Entry* entry) {
  entry->signal_changed().connect(sigc::mem_fun(*tree_view, fx_changed));
  entry->signal_activate().connect(sigc::mem_fun(*tree_view, fx_activated));
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
                             Files::Files& files,
                             Gtk::Entry* entry_title, Gtk::Entry* entry_artist,
                             Gtk::Entry* entry_album,
                             Gtk::Entry* entry_track_num,
                             Gtk::Entry* entry_track_denum)
    : Gtk::TreeView(cobject), files_(files), entry_song_title_(entry_title),
      entry_song_artist_(entry_artist), entry_song_album_(entry_album),
      entry_song_track_num_(entry_track_num),
      entry_song_track_denum_(entry_track_denum),
      disable_signals_(true) {
  treeselection_ = this->get_selection();

  addSignalsToFieldEntry(this, &TreeViewFiles::onEditTypeEntry,
                         &TreeViewFiles::onEntryEnterPress, entry_title);
  addSignalsToFieldEntry(this, &TreeViewFiles::onEditTypeEntry,
                         &TreeViewFiles::onEntryEnterPress, entry_artist);
  addSignalsToFieldEntry(this, &TreeViewFiles::onEditTypeEntry,
                         &TreeViewFiles::onEntryEnterPress, entry_album);
  addSignalsToFieldEntry(this, &TreeViewFiles::onEditTypeEntry,
                         &TreeViewFiles::onEntryEnterPress, entry_track_num);
  addSignalsToFieldEntry(this, &TreeViewFiles::onEditTypeEntry,
                         &TreeViewFiles::onEntryEnterPress, entry_track_denum);

  liststore_ = Gtk::ListStore::create(columns_);
  this->set_model(liststore_);

  appendColumnEditable(this, kTrack, columns_.track(),
                       &TreeViewFiles::onEditTypeRow);
  appendColumnEditable(this, kTitle, columns_.title(),
                       &TreeViewFiles::onEditTypeRow);
  appendColumnEditable(this, kArtist, columns_.artist(),
                       &TreeViewFiles::onEditTypeRow);
  appendColumnEditable(this, kAlbum, columns_.album(),
                       &TreeViewFiles::onEditTypeRow);
  appendColumn(this, kBitrate, columns_.bitrate());
  appendColumn(this, kSamplingRate, columns_.samplingRate());
  appendColumn(this, kChannelMode, columns_.channelMode());
  appendColumn(this, kFilepath, columns_.filepath());

  treeselection_->signal_changed().connect(
    sigc::mem_fun(*this, &TreeViewFiles::onRowSelect));

  edit_type_ = EditType::kUnedited;
}

void TreeViewFiles::onEditTypeRow(const Glib::ustring&,
                                  const Glib::ustring&) {
  if (disable_signals_ || !current_row_) return;
  edit_type_ = EditType::kRow;
  storeCurrentEditsInFileMem();
  updateCurrentRowFromFileMem();
  updateEntryFromFileMem();
}

void TreeViewFiles::onEditTypeEntry() {
  if (disable_signals_ || !current_row_) return;
  edit_type_ = EditType::kEntry;
}

void TreeViewFiles::onEntryEnterPress() {
  storeAndUpdateEntryData();
}

void TreeViewFiles::storeAndUpdateEntryData() {
  storeCurrentEditsInFileMem();
  updateCurrentRowFromFileMem();
  updateEntryFromFileMem();
}

void TreeViewFiles::populateTreeView() {
  disable_signals_ = true;
  current_row_ = Gtk::TreeModel::iterator();
  edit_type_ = EditType::kUnedited;
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
  entry_song_title_->set_text(Glib::ustring());
  entry_song_artist_->set_text(Glib::ustring());
  entry_song_album_->set_text(Glib::ustring());
  entry_song_track_num_->set_text(Glib::ustring());
  entry_song_track_denum_->set_text(Glib::ustring());
  disable_signals_ = false;
}

void TreeViewFiles::saveSelectedFile(bool rename_file) {
  if (!current_row_) return;
  storeAndUpdateEntryData();
  Gtk::TreeModel::Row row = *current_row_;
  saveSelectedFile(row, rename_file, true);
}

void TreeViewFiles::saveSelectedFile(Gtk::TreeModel::Row& row,
                                     bool rename_file, bool is_single_file) {
  if (is_single_file) files_.clearErrorLog();
  int pos = row[columns_.pos()];
  if (!files_.saveFile(pos, rename_file)) {
    bool disable_signals_state = disable_signals_;
    disable_signals_ = true;
    if (row == *current_row_) {
      treeselection_->unselect(current_row_);
      current_row_ = Gtk::TreeModel::iterator();
      entry_song_title_->set_text(Glib::ustring());
      entry_song_artist_->set_text(Glib::ustring());
      entry_song_album_->set_text(Glib::ustring());
      entry_song_track_num_->set_text(Glib::ustring());
      entry_song_track_denum_->set_text(Glib::ustring());
    }
    liststore_->erase(row);
    // TODO show error if is_single_file.
    disable_signals_ = disable_signals_state;
    return;
  }
  row[columns_.filepath()] = files_[pos].getFilepath();
}

void TreeViewFiles::saveAllFiles(bool rename_file) {
  files_.clearErrorLog();
  if (current_row_) storeAndUpdateEntryData();
  Gtk::TreeModel::Children children = liststore_->children();
  for (auto it = children.begin(); it != children.end(); it++) {
    Gtk::TreeModel::Row row = *it;
    saveSelectedFile(row, rename_file, false);
  }
  if (!files_.getErrorList().empty()) {
    // TODO show error.
  }
}

void TreeViewFiles::getRowData(const Gtk::TreeModel::Row& row,
                               std::string& title, std::string& artist,
                               std::string& album, std::string& track) {
  title = row[columns_.title()];
  artist = row[columns_.artist()];
  album = row[columns_.album()];
  track = row[columns_.track()];
}

void TreeViewFiles::getEntryData(std::string& title, std::string& artist,
                                 std::string& album, int& track_num,
                                 int& track_denum) {
  title = entry_song_title_->get_text();
  artist = entry_song_artist_->get_text();
  album = entry_song_album_->get_text();
  try {
    track_num = std::stoi(entry_song_track_num_->get_text());
  } catch (const std::exception&) {
    track_num = -1;
  }
  try {
    track_denum = std::stoi(entry_song_track_denum_->get_text());
  } catch (const std::exception&) {
    track_denum = -1;
  }
}

void TreeViewFiles::storeCurrentEditsInFileMem() {
  if (!current_row_ || edit_type_ == EditType::kUnedited) return;
  int idx = (*current_row_)[columns_.pos()];
  std::string title;
  std::string artist;
  std::string album;
  std::string track;
  int track_num;
  int track_denum;
  switch (edit_type_) {
    case EditType::kRow:
      getRowData(*current_row_, title, artist, album, track);
      files_[idx].updateFields(title, artist, album, track);
      break;
    case EditType::kEntry:
      getEntryData(title, artist, album, track_num, track_denum);
      files_[idx].updateFields(title, artist, album, track_num, track_denum);
      break;
    default:
      break;
  }
  edit_type_ = EditType::kUnedited;
}

void TreeViewFiles::updateCurrentRowFromFileMem() {
  if (!current_row_) return;
  Gtk::TreeModel::Row row = *current_row_;
  int idx = row[columns_.pos()];
  row[columns_.title()] = files_[idx].getTitle();
  row[columns_.artist()] = files_[idx].getArtist();
  row[columns_.album()] = files_[idx].getAlbum();
  row[columns_.track()] = files_[idx].getTrack();
}

void TreeViewFiles::updateEntryFromFileMem() {
  if (!current_row_) return;
  using std::to_string;
  bool disable_signals_state_ = disable_signals_;
  disable_signals_ = true;

  int idx = (*current_row_)[columns_.pos()];
  int track_num = files_[idx].getTrackNum();
  int track_denum = files_[idx].getTrackDenum();
  entry_song_title_->set_text((Glib::ustring)files_[idx].getTitle());
  entry_song_artist_->set_text((Glib::ustring)files_[idx].getArtist());
  entry_song_album_->set_text((Glib::ustring)files_[idx].getAlbum());
  if (track_num != -1) {
    entry_song_track_num_->set_text((Glib::ustring)to_string(track_num));
  } else {
    entry_song_track_num_->set_text(Glib::ustring());
  }
  if (track_denum != -1) {
    entry_song_track_denum_->set_text((Glib::ustring)to_string(track_denum));
  } else {
    entry_song_track_denum_->set_text(Glib::ustring());
  }

  disable_signals_ = disable_signals_state_;
}

void TreeViewFiles::onRowSelect() {
  if (disable_signals_) return;
  storeCurrentEditsInFileMem();
  updateCurrentRowFromFileMem();
  current_row_ = treeselection_->get_selected();
  updateEntryFromFileMem();
}

}  // namespace Gui
}  // namespace Mp3Edit
