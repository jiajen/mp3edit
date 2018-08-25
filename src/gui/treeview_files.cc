#include "mp3edit/src/gui/treeview_files.h"

#include <gtkmm/cellrenderer.h>
#include <gtkmm/cellrenderertext.h>

#include "mp3edit/src/file.h"
#include "mp3edit/src/gui/window_main.h"

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

void appendColumn(TreeViewFiles* tree_view, const char* column_title,
                  Gtk::TreeModelColumn<std::string>& column) {
  tree_view->get_column(tree_view->append_column(
    column_title, column)-1)->set_sort_column(column);
}

void appendColumnEditable(TreeViewFiles* tree_view, const char* column_title,
                          Gtk::TreeModelColumn<std::string>& column,
                          void (TreeViewFiles::* function_ptr)(
                            const Glib::ustring&, const Glib::ustring&)) {
  int idx = tree_view->append_column_editable(column_title, column)-1;
  tree_view->get_column(idx)->set_sort_column(column);

  Gtk::CellRenderer* renderer = tree_view->get_column_cell_renderer(idx);
  Gtk::CellRendererText* renderer_text = (Gtk::CellRendererText*)(renderer);
  renderer_text->signal_edited().connect(
    sigc::mem_fun(*tree_view, function_ptr));
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
                             WindowMain* parent_window, Files::Files& files)
    : Gtk::TreeView(cobject), files_(files), parent_window_(parent_window) {
  liststore_ = Gtk::ListStore::create(columns_);
  this->set_model(liststore_);

  treeselection_ = this->get_selection();
  enableRowSignal();

  appendColumnEditable(this, kTrack, columns_.track(),
    &TreeViewFiles::onRowDataEdit);
  appendColumnEditable(this, kTitle, columns_.title(),
    &TreeViewFiles::onRowDataEdit);
  appendColumnEditable(this, kArtist, columns_.artist(),
    &TreeViewFiles::onRowDataEdit);
  appendColumnEditable(this, kAlbum, columns_.album(),
    &TreeViewFiles::onRowDataEdit);
  appendColumn(this, kBitrate, columns_.bitrate());
  appendColumn(this, kSamplingRate, columns_.samplingRate());
  appendColumn(this, kChannelMode, columns_.channelMode());
  appendColumn(this, kFilepath, columns_.filepath());

  populateTreeView(-1);
}

void TreeViewFiles::populateTreeView(int selected_file_idx) {
  if (signal_row_change_.connected()) {
    runWithoutSignal(&TreeViewFiles::populateTreeView, selected_file_idx);
    return;
  }

  unSelectRowIfSelected();
  liststore_->clear();
  for (int i = 0, n = files_.size(); i < n; i++) {
    if (!files_[i]) continue;
    Gtk::TreeModel::Row row = *(liststore_->append());
    row[columns_.pos()] = i;
    row[columns_.title()] = files_[i].getTitle();
    row[columns_.artist()] = files_[i].getArtist();
    row[columns_.album()] = files_[i].getAlbum();
    row[columns_.track()] = files_[i].getTrack();
    row[columns_.bitrate()] = files_[i].getBitrate();
    row[columns_.samplingRate()] = files_[i].getSamplingRate();
    row[columns_.channelMode()] = files_[i].getChannelMode();
    row[columns_.filepath()] = files_[i].getFilepath();
    if (i == selected_file_idx) treeselection_->select(row);
  }
}

void TreeViewFiles::restoreSelectedRowData() {
  restoreRowData(*treeselection_->get_selected());
}

int TreeViewFiles::getSelectedFileIdx() {
  Gtk::TreeModel::iterator row_ptr = treeselection_->get_selected();
  return (row_ptr) ? (*row_ptr)[columns_.pos()] : -1;
}

void TreeViewFiles::updateSelectedRowFilepath() {
  Gtk::TreeModel::Row row = *treeselection_->get_selected();
  row[columns_.filepath()] = files_[row[columns_.pos()]].getFilepath();
}

void TreeViewFiles::updateAllRowsFilepath() {
  for (Gtk::TreeModel::Row row: liststore_->children())
    row[columns_.filepath()] = files_[row[columns_.pos()]].getFilepath();
}

void TreeViewFiles::removeSelectedRow() {
  Gtk::TreeModel::iterator row_ptr = treeselection_->get_selected();
  unSelectRowIfSelected();
  liststore_->erase(row_ptr);
}

void TreeViewFiles::onRowSelect() {
  parent_window_->clearProgressBar();
  if (last_selected_row_ptr_) {
    parent_window_->storeEntryData((*last_selected_row_ptr_)[columns_.pos()]);
    restoreRowData(*last_selected_row_ptr_);
  }
  Gtk::TreeModel::iterator row_ptr = treeselection_->get_selected();
  parent_window_->restoreEntryData((row_ptr) ? (*row_ptr)[columns_.pos()] : -1);
  last_selected_row_ptr_ = row_ptr;
}

void TreeViewFiles::onRowDataEdit(const Glib::ustring&, const Glib::ustring&) {
  Gtk::TreeModel::Row row = *treeselection_->get_selected();
  storeRowData(row);
  restoreRowData(row);
  parent_window_->restoreEntryData(row[columns_.pos()]);
}

void TreeViewFiles::unSelectRowIfSelected() {
  if (signal_row_change_.connected()) {
    runWithoutSignal(&TreeViewFiles::unSelectRowIfSelected);
    return;
  }

  Gtk::TreeModel::iterator row_ptr = treeselection_->get_selected();
  if (row_ptr) treeselection_->unselect(row_ptr);
  last_selected_row_ptr_ = Gtk::TreeModel::iterator();
}

void TreeViewFiles::storeRowData(const Gtk::TreeModel::Row& row) {
  int idx = row[columns_.pos()];
  files_[idx].updateFields(row[columns_.title()], row[columns_.artist()],
                           row[columns_.album()], row[columns_.track()]);
}

void TreeViewFiles::restoreRowData(const Gtk::TreeModel::Row& row) {
  int idx = row[columns_.pos()];
  row[columns_.title()] = files_[idx].getTitle();
  row[columns_.artist()] = files_[idx].getArtist();
  row[columns_.album()] = files_[idx].getAlbum();
  row[columns_.track()] = files_[idx].getTrack();
}

void TreeViewFiles::enableRowSignal() {
  signal_row_change_ = treeselection_->signal_changed().connect(
                         sigc::mem_fun(*this, &TreeViewFiles::onRowSelect));
}

void TreeViewFiles::runWithoutSignal(void (TreeViewFiles::* function_ptr)()) {
  signal_row_change_.disconnect();
  ((*this).*function_ptr)();
  enableRowSignal();
}

void TreeViewFiles::runWithoutSignal(void (TreeViewFiles::* function_ptr)(int),
                                     int val) {
  signal_row_change_.disconnect();
  ((*this).*function_ptr)(val);
  enableRowSignal();
}

}  // namespace Gui
}  // namespace Mp3Edit
