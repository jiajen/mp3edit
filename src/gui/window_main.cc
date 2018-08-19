#include "mp3edit/src/gui/window_main.h"

#include <glib.h>
#include <gtkmm/filechooserdialog.h>

#include "mp3edit/src/directory.h"

namespace Mp3Edit {
namespace Gui {

using File::File;

WindowMain::WindowMain(BaseObjectType* cobject,
                       const Glib::RefPtr<Gtk::Builder>& builder)
    : Gtk::Window(cobject), builder_(builder) {
  builder_->get_widget("checkbox_read_subdir", checkbox_read_subdir_);
  builder_->get_widget("checkbox_read_audio", checkbox_read_audio_);

  builder_->get_widget("btn_dir_open", btn_dir_open_);
  btn_dir_open_->signal_clicked().connect(
    sigc::mem_fun(*this, &WindowMain::openDirDialog));

  builder_->get_widget("btn_dir_refresh", btn_dir_refresh_);
  btn_dir_refresh_->signal_clicked().connect(
    sigc::mem_fun(*this, &WindowMain::loadEntryDir));

  builder_->get_widget("entry_dir", entry_dir_);

  builder_->get_widget("entry_song_title", entry_song_title_);
  builder_->get_widget("entry_song_artist", entry_song_artist_);
  builder_->get_widget("entry_song_album", entry_song_album_);
  builder_->get_widget("entry_song_track_num", entry_song_track_num_);
  builder_->get_widget("entry_song_track_denum", entry_song_track_denum_);

  builder_->get_widget_derived("gtk_treeview_files", treeview_files_, files_,
                               entry_song_title_, entry_song_artist_,
                               entry_song_album_, entry_song_track_num_,
                               entry_song_track_denum_);
}

void WindowMain::openDirDialog() {
  Gtk::FileChooserDialog dir_chooser("Select a directory",
                                     Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
  dir_chooser.set_transient_for(*this);

  dir_chooser.add_button("Open", Gtk::RESPONSE_OK);
  dir_chooser.add_button("Cancel", Gtk::RESPONSE_CANCEL);
  dir_chooser.set_filename(g_get_home_dir());

  if (dir_chooser.run() == Gtk::RESPONSE_OK) {
    entry_dir_->set_text(dir_chooser.get_filename());
    loadEntryDir();
  }
}

void WindowMain::loadEntryDir() {
  files_ = Directory::getFiles(entry_dir_->get_text(),
                               checkbox_read_subdir_->get_active(),
                               checkbox_read_audio_->get_active());
  treeview_files_->populateTreeView();
}

}  // namespace Gui
}  // namespace Mp3Edit
