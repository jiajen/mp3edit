#include "mp3edit/src/gui/window_main.h"

#include <string>
#include <algorithm>
#include <filesystem>
#include <exception>

#include <gtkmm/widget.h>
#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/filechooserdialog.h>

#include "mp3edit/src/webservice/wikipedia.h"

namespace Mp3Edit {
namespace Gui {

namespace {

using WebService::Wikipedia::generateSearchUrl;

int stringToTrack(const std::string& track) {
  try {
    return std::stoi(track);
  } catch (const std::exception&) {
    return -1;
  }
}

}  // namespace

WindowMain::WindowMain(BaseObjectType* cobject,
                       const Glib::RefPtr<Gtk::Builder>& builder)
    : Gtk::Window(cobject), files_(&dispatcher_),
      processing_mode_(Files::Files::ProcessingMode::kReady),
      thread_(nullptr),
      builder_(builder) {
  dispatcher_.connect(
    sigc::mem_fun(*this, &WindowMain::onOperationUpdate));

  builder_->get_widget("entry_dir", entry_dir_);

  entry_dir_->signal_activate().connect(
    sigc::mem_fun(*this, &WindowMain::onDirEntryEnterPress));

  builder_->get_widget("checkbox_read_subdir", checkbox_read_subdir_);

  builder_->get_widget("btn_dir_open", btn_dir_open_);
  btn_dir_open_->signal_clicked().connect(
    sigc::mem_fun(*this, &WindowMain::onOpenDirBtnPress));

  builder_->get_widget("btn_dir_refresh", btn_dir_refresh_);
  btn_dir_refresh_->signal_clicked().connect(
    sigc::mem_fun(*this, &WindowMain::onRefreshDirBtnPress));

  builder_->get_widget_derived("gtk_treeview_files", treeview_files_,
                               this, files_);

  builder_->get_widget("checkbox_read_audio", checkbox_read_audio_);

  builder_->get_widget("entry_song_title", entry_song_title_);
  entry_song_title_->signal_activate().connect(
    sigc::mem_fun(*this, &WindowMain::onMetadataEntryEnterPress));
  builder_->get_widget("entry_song_artist", entry_song_artist_);
  entry_song_artist_->signal_activate().connect(
    sigc::mem_fun(*this, &WindowMain::onMetadataEntryEnterPress));
  builder_->get_widget("entry_song_album", entry_song_album_);
  entry_song_album_->signal_activate().connect(
    sigc::mem_fun(*this, &WindowMain::onMetadataEntryEnterPress));
  builder_->get_widget("entry_song_track_num", entry_song_track_num_);
  entry_song_track_num_->signal_activate().connect(
    sigc::mem_fun(*this, &WindowMain::onMetadataEntryEnterPress));
  builder_->get_widget("entry_song_track_denum", entry_song_track_denum_);
  entry_song_track_denum_->signal_activate().connect(
    sigc::mem_fun(*this, &WindowMain::onMetadataEntryEnterPress));

  builder_->get_widget("checkbox_rename_file", checkbox_rename_file_);

  builder_->get_widget("btn_song_save_single", btn_song_save_single_);
  btn_song_save_single_->signal_clicked().connect(
    sigc::mem_fun(*this, &WindowMain::onSaveFileBtnPress));

  builder_->get_widget("btn_song_search_web", btn_song_search_web_);
  btn_song_search_web_->signal_clicked().connect(
    sigc::mem_fun(*this, &WindowMain::onSearchWebBtnPress));

  builder_->get_widget("btn_song_save_all", btn_song_save_all_);
  btn_song_save_all_->signal_clicked().connect(
    sigc::mem_fun(*this, &WindowMain::onSaveAllFilesBtnPress));

  builder_->get_widget("eventbox_progressbar_main", eventbox_progressbar_main_);
  eventbox_progressbar_main_->signal_button_press_event().connect(
    sigc::mem_fun(*this, &WindowMain::onClickProgressBar));

  builder_->get_widget("progressbar_main", progressbar_main_);
  clearProgressBar();

  builder_->get_widget("btn_cancel_action", btn_cancel_action_);
  btn_cancel_action_->signal_clicked().connect(
    sigc::mem_fun(*this, &WindowMain::onCancelBtnPress));

  builder->get_widget_derived("gtk_dialog_error_dialog",
                              gtk_dialog_error_dialog_, files_.getErrorList());

  signal_delete_event().connect(
    sigc::mem_fun(*this, &WindowMain::onCloseWindow));

  Gtk::Box* box_main;
  Gtk::Box* box_dir_path;
  Gtk::ScrolledWindow* scroller_view;
  Gtk::Grid* grid_main_controls;
  Gtk::Box* box_track_controls;

  builder->get_widget("gtk_box_main", box_main);
  builder->get_widget("gtk_box_dir_path", box_dir_path);
  builder->get_widget("gtk_scroller_treeview_files", scroller_view);
  builder->get_widget("gtk_grid_main_controls", grid_main_controls);
  builder->get_widget("gtk_box_track_controls", box_track_controls);

  std::vector<Gtk::Widget*> widget_order_box_main;
  widget_order_box_main.push_back(box_dir_path);
  widget_order_box_main.push_back(scroller_view);
  widget_order_box_main.push_back(grid_main_controls);
  widget_order_box_main.push_back(scroller_view);
  box_main->set_focus_chain(widget_order_box_main);

  std::vector<Gtk::Widget*> widget_order_box_dir;
  widget_order_box_dir.push_back(entry_dir_);
  box_dir_path->set_focus_chain(widget_order_box_dir);

  std::vector<Gtk::Widget*> widget_order_grid;
  widget_order_grid.push_back(entry_song_title_);
  widget_order_grid.push_back(entry_song_artist_);
  widget_order_grid.push_back(entry_song_album_);
  widget_order_grid.push_back(box_track_controls);
  grid_main_controls->set_focus_chain(widget_order_grid);

  std::vector<Gtk::Widget*> widget_order_box_track;
  widget_order_box_track.push_back(entry_song_track_num_);
  widget_order_box_track.push_back(entry_song_track_denum_);
  box_track_controls->set_focus_chain(widget_order_box_track);
}

void WindowMain::storeEntryData(int pos) {
  files_[pos].updateFields(entry_song_title_->get_text(),
                           entry_song_artist_->get_text(),
                           entry_song_album_->get_text(),
                           stringToTrack(entry_song_track_num_->get_text()),
                           stringToTrack(entry_song_track_denum_->get_text()));
}

void WindowMain::restoreEntryData(int pos) {
  int track_num = (pos == -1) ? -1 : files_[pos].getTrackNum();
  int track_denum = (pos == -1) ? -1 : files_[pos].getTrackDenum();
  entry_song_title_->set_text((pos == -1) ? "" : files_[pos].getTitle());
  entry_song_artist_->set_text((pos == -1) ? "" : files_[pos].getArtist());
  entry_song_album_->set_text((pos == -1) ? "" : files_[pos].getAlbum());
  entry_song_track_num_->set_text(
    (track_num != -1) ? std::to_string(track_num) : "");
  entry_song_track_denum_->set_text(
    (track_denum != -1) ? std::to_string(track_denum) : "");
}

void WindowMain::clearProgressBar() {
  progressbar_main_->set_fraction(0);
  progressbar_main_->set_text("");
}

void WindowMain::onDirEntryEnterPress() {
  preOpLoadEntryDir();
}

void WindowMain::onOpenDirBtnPress() {
  openDirDialog();
}

void WindowMain::onRefreshDirBtnPress() {
  preOpLoadEntryDir();
}

void WindowMain::onMetadataEntryEnterPress() {
  storeEntryDataAndUpdateSelectedRowAndEntry();
}

void WindowMain::onSaveFileBtnPress() {
  if (!storeEntryDataAndUpdateSelectedRowAndEntry()) return;
  preOpSaveFile();
}

void WindowMain::onSearchWebBtnPress() {
  attemptBrowserSearch();
}

void WindowMain::onSaveAllFilesBtnPress() {
  storeEntryDataAndUpdateSelectedRowAndEntry();
  preOpSaveAllFiles();
}

bool WindowMain::onClickProgressBar(GdkEventButton* button_event) {
  if (button_event->type == GDK_BUTTON_PRESS && button_event->button == 1 &&
      !files_.getErrorList().empty()) {
    showErrorDialog();
  }
  return true;
}

void WindowMain::onCancelBtnPress() {
  files_.stopOperation();
}

bool WindowMain::onCloseWindow(bool) {
  if (processing_mode_ == Files::Files::ProcessingMode::kReady) return false;
  files_.stopOperation();
  return true;
}

bool WindowMain::on_key_press_event(GdkEventKey* event) {
  if (processing_mode_ == Files::Files::ProcessingMode::kReady &&
      event->state == GDK_CONTROL_MASK) {
    switch (event->keyval) {
      case GDK_KEY_O:
      case GDK_KEY_o:
        openDirDialog();
        break;
      case GDK_KEY_L:
      case GDK_KEY_l:
        selectDirEntry();
        break;
      case GDK_KEY_Return:
        attemptBrowserSearch();
        break;
      default:
        return Gtk::Window::on_key_press_event(event);
        break;
    }
    return true;
  }
  return Gtk::Window::on_key_press_event(event);
}

void WindowMain::onOperationUpdate() {
  if (processing_mode_ == Files::Files::ProcessingMode::kReady) return;

  int processed_files;
  int total_files;
  std::string processing_file;
  auto mode = files_.fileOperationStatus(processed_files, total_files,
                                         processing_file);

  updateProgressBar(processing_file, processed_files, total_files,
                    mode == Files::Files::ProcessingMode::kReady);

  if (mode != Files::Files::ProcessingMode::kReady) return;
  switch (processing_mode_) {
    case Files::Files::ProcessingMode::kReadMulti:
      postOpLoadEntryDir();
      break;
    case Files::Files::ProcessingMode::kSaveSingle:
      postOpSaveFile();
      break;
    case Files::Files::ProcessingMode::kSaveMulti:
      postOpSaveAllFiles();
      break;
    default:
      break;
  }
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
    preOpLoadEntryDir();
  }
}

bool WindowMain::storeEntryDataAndUpdateSelectedRowAndEntry() {
  int idx = treeview_files_->getSelectedFileIdx();
  if (idx == -1) return false;
  storeEntryData(idx);
  restoreEntryData(idx);
  treeview_files_->restoreSelectedRowData();
  return true;
}

void WindowMain::updateProgressBar(const std::string& filename,
                                   int processed_files, int total_files,
                                   bool done_processing) {
  using std::to_string;
  switch (processing_mode_) {
    case Files::Files::ProcessingMode::kReadMulti:
      if (done_processing) {
        if (total_files == -1) return;
        progressbar_main_->set_fraction(1);
        if (files_.getErrorList().empty()) {
          progressbar_main_->set_text("Loaded " + to_string(processed_files) +
                                      " files.");
        } else {
          int ok_files = processed_files - files_.getErrorList().size();
          progressbar_main_->set_text("Loaded " + to_string(ok_files) +
                                      " out of " + to_string(processed_files) +
                                      " files. Click here for details.");
        }
      } else {
        if (total_files == -1 && processed_files != -1) {
          progressbar_main_->set_text("Finding files - " +
                                      filename + " [" +
                                      to_string(processed_files+1) + "]");
          progressbar_main_->pulse();
        } else if (total_files != -1) {
          if (total_files == 0) total_files = 1;
          progressbar_main_->set_fraction((double)processed_files/total_files);
          progressbar_main_->set_text("Loading " + filename +
                                      " [" + to_string(processed_files+1) +
                                      "/" + to_string(total_files) + "]");
        }
      }
      break;
    case Files::Files::ProcessingMode::kSaveSingle:
      if (done_processing) {
        progressbar_main_->set_fraction(1);
        if (processed_files == 0) {
          progressbar_main_->set_text("Cancelled saving " + filename + ".");
          return;
        }
        if (files_.getErrorList().empty()) {
          progressbar_main_->set_text("Saved " + filename + ".");
        } else {
          progressbar_main_->set_text("Error saving " + filename +
                                      ". Click here for details.");
        }
      } else {
        progressbar_main_->set_fraction(0);
        progressbar_main_->set_text("Saving " + filename + ".");
      }
      break;
    case Files::Files::ProcessingMode::kSaveMulti:
      if (done_processing) {
        progressbar_main_->set_fraction(1);
        if (files_.getErrorList().empty()) {
          progressbar_main_->set_text("Saved " + to_string(processed_files) +
                                      " files.");
        } else {
          int ok_files = processed_files - files_.getErrorList().size();
          progressbar_main_->set_text("Saved " + to_string(ok_files) +
                                      " out of " + to_string(processed_files) +
                                      " files. Click here for details.");
        }
      } else {
        progressbar_main_->set_fraction((double)processed_files/total_files);
        progressbar_main_->set_text("Saving " + filename +
                                    " [" + to_string(processed_files+1) +
                                    "/" + to_string(total_files) + "]");
      }
      break;
    default:
      break;
  }
}

void WindowMain::selectDirEntry() {
  entry_dir_->grab_focus();
}

void WindowMain::showErrorDialog() {
  gtk_dialog_error_dialog_->set_transient_for(*this);
  gtk_dialog_error_dialog_->run();
  gtk_dialog_error_dialog_->hide();
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
  eventbox_progressbar_main_->set_sensitive(!enter_loading_mode);
}

void WindowMain::enterProcessingMode(Files::Files::ProcessingMode mode) {
  const auto kReady = Files::Files::ProcessingMode::kReady;
  if (processing_mode_ == kReady && mode != kReady) {
    toggleLoadingMode(true);
    clearProgressBar();
  } else if (mode == kReady && processing_mode_ != kReady) {
    toggleLoadingMode(false);
    thread_->join();
    delete thread_;
    thread_ = nullptr;
  }
  processing_mode_ = mode;
}

void WindowMain::attemptBrowserSearch() {
  std::string url = generateSearchUrl(getSongTitleOrFilename(),
                                      entry_song_artist_->get_text(),
                                      entry_song_album_->get_text(),
                                      entry_song_track_num_->get_text(),
                                      entry_song_track_denum_->get_text());
  if (url.empty()) return;
  gtk_show_uri_on_window(nullptr, url.c_str(), GDK_CURRENT_TIME, nullptr);
}

std::string WindowMain::getSongTitleOrFilename() {
  if (!entry_song_title_->get_text().empty())
    return entry_song_title_->get_text();
  int idx = treeview_files_->getSelectedFileIdx();
  if (idx == -1) return "";
  return files_[idx].getFilename();
}

void WindowMain::preOpLoadEntryDir() {
  enterProcessingMode(Files::Files::ProcessingMode::kReadMulti);
  files_.setOperation(Files::Files::ProcessingMode::kReadMulti);
  thread_ = new std::thread([this]() {
    files_.readDirectory(entry_dir_->get_text(),
                         checkbox_read_subdir_->get_active(),
                         checkbox_read_audio_->get_active());
  });
}

void WindowMain::postOpLoadEntryDir() {
  treeview_files_->populateTreeView(-1);
  restoreEntryData(-1);
  if (!files_.getErrorList().empty()) showErrorDialog();
  enterProcessingMode(Files::Files::ProcessingMode::kReady);
}

void WindowMain::preOpSaveFile() {
  enterProcessingMode(Files::Files::ProcessingMode::kSaveSingle);
  files_.setOperation(Files::Files::ProcessingMode::kSaveSingle);
  thread_ = new std::thread([this]() {
    files_.saveFile(treeview_files_->getSelectedFileIdx(),
                    checkbox_rename_file_->get_active());
  });
}

void WindowMain::postOpSaveFile() {
  if (!files_.getErrorList().empty()) {
    treeview_files_->removeSelectedRow();
    restoreEntryData(-1);
    showErrorDialog();
  } else if (checkbox_rename_file_->get_active()) {
    treeview_files_->updateSelectedRowFilepath();
  }
  enterProcessingMode(Files::Files::ProcessingMode::kReady);
}

void WindowMain::preOpSaveAllFiles() {
  enterProcessingMode(Files::Files::ProcessingMode::kSaveMulti);
  files_.setOperation(Files::Files::ProcessingMode::kSaveMulti);
  thread_ = new std::thread([this]() {
    files_.saveAllFiles(checkbox_rename_file_->get_active());
  });
}

void WindowMain::postOpSaveAllFiles() {
  if (!files_.getErrorList().empty()) {
    treeview_files_->populateTreeView(treeview_files_->getSelectedFileIdx());
    if (!treeview_files_->getSelectedFileIdx()) restoreEntryData(-1);
    showErrorDialog();
  } else if (checkbox_rename_file_->get_active()) {
    treeview_files_->updateAllRowsFilepath();
  }
  enterProcessingMode(Files::Files::ProcessingMode::kReady);
}

}  // namespace Gui
}  // namespace Mp3Edit
