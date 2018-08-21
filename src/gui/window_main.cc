#include "mp3edit/src/gui/window_main.h"

#include <filesystem>

#include <glib.h>
#include <gtkmm/filechooserdialog.h>

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
  entry_dir_->signal_activate().connect(
    sigc::mem_fun(*this, &WindowMain::loadEntryDir));

  builder_->get_widget("entry_song_title", entry_song_title_);
  builder_->get_widget("entry_song_artist", entry_song_artist_);
  builder_->get_widget("entry_song_album", entry_song_album_);
  builder_->get_widget("entry_song_track_num", entry_song_track_num_);
  builder_->get_widget("entry_song_track_denum", entry_song_track_denum_);

  builder_->get_widget("progressbar_main", progressbar_main_);
  progressbar_main_->set_show_text(true);
  progressbar_main_->set_text("");
  builder_->get_widget("btn_cancel_action", btn_cancel_action_);
  btn_cancel_action_->signal_clicked().connect(
    sigc::mem_fun(*this, &WindowMain::onCancelBtnPress));

  builder_->get_widget_derived("gtk_treeview_files", treeview_files_, files_,
                               entry_song_title_, entry_song_artist_,
                               entry_song_album_, entry_song_track_num_,
                               entry_song_track_denum_, progressbar_main_);

  builder_->get_widget("btn_song_search_web", btn_song_search_web_);
  btn_song_search_web_->signal_clicked().connect(
    sigc::mem_fun(*this, &WindowMain::onSearchWebBtnPress));

  builder_->get_widget("btn_song_save_single", btn_song_save_single_);
  btn_song_save_single_->signal_clicked().connect(
    sigc::mem_fun(*this, &WindowMain::onSaveFileBtnPress));

  builder_->get_widget("btn_song_save_all", btn_song_save_all_);
  btn_song_save_all_->signal_clicked().connect(
    sigc::mem_fun(*this, &WindowMain::onSaveAllFilesBtnPress));

  builder_->get_widget("checkbox_rename_file", checkbox_rename_file_);

}

void WindowMain::toggleLoadingMode(bool enter_loading_mode) {
  checkbox_read_subdir_->set_sensitive(!enter_loading_mode);
  checkbox_read_audio_->set_sensitive(!enter_loading_mode);
  btn_dir_open_->set_sensitive(!enter_loading_mode);
  btn_dir_refresh_->set_sensitive(!enter_loading_mode);
  entry_dir_->set_sensitive(!enter_loading_mode);
  treeview_files_->set_sensitive(!enter_loading_mode);
  entry_song_title_->set_sensitive(!enter_loading_mode);
  entry_song_artist_->set_sensitive(!enter_loading_mode);
  entry_song_album_->set_sensitive(!enter_loading_mode);
  entry_song_track_num_->set_sensitive(!enter_loading_mode);
  entry_song_track_denum_->set_sensitive(!enter_loading_mode);
  btn_song_save_single_->set_sensitive(!enter_loading_mode);
  btn_song_save_all_->set_sensitive(!enter_loading_mode);
  btn_song_search_web_->set_sensitive(!enter_loading_mode);
  checkbox_rename_file_->set_sensitive(!enter_loading_mode);
}

void WindowMain::openDirDialog() {
  Gtk::FileChooserDialog dir_chooser("Select a directory",
                                     Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
  dir_chooser.set_transient_for(*this);

  dir_chooser.add_button("Open", Gtk::RESPONSE_OK);
  dir_chooser.add_button("Cancel", Gtk::RESPONSE_CANCEL);

  if (std::filesystem::is_directory((std::string)entry_dir_->get_text())) {
    dir_chooser.set_filename(entry_dir_->get_text());
  } else {
    dir_chooser.set_filename(g_get_home_dir());
  }

  if (dir_chooser.run() == Gtk::RESPONSE_OK) {
    entry_dir_->set_text(dir_chooser.get_filename());
    loadEntryDir();
  }
}

void WindowMain::loadEntryDir() {
  files_ = Files::Files(entry_dir_->get_text(),
                        checkbox_read_subdir_->get_active(),
                        checkbox_read_audio_->get_active());
  treeview_files_->populateTreeView();
}

void WindowMain::onSaveFileBtnPress() {
  treeview_files_->saveSelectedFile(checkbox_rename_file_->get_active());
}

void WindowMain::onSaveAllFilesBtnPress() {
  treeview_files_->saveAllFiles(checkbox_rename_file_->get_active());
}

void WindowMain::onSearchWebBtnPress() {
  // TODO
}

void WindowMain::onCancelBtnPress() {
  // TODO
}

}  // namespace Gui
}  // namespace Mp3Edit
