#ifndef MP3EDIT_GUI_WINDOW_MAIN_H_
#define MP3EDIT_GUI_WINDOW_MAIN_H_

#include <string>
#include <thread>

#include <glibmm/dispatcher.h>
#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/window.h>
#include <gtkmm/entry.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/button.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/progressbar.h>

#include "mp3edit/src/files.h"
#include "mp3edit/src/gui/treeview_files.h"
#include "mp3edit/src/gui/error_dialog.h"

namespace Mp3Edit {
namespace Gui {

class WindowMain: public Gtk::Window {
 public:
  WindowMain(BaseObjectType* cobject,
             const Glib::RefPtr<Gtk::Builder>& builder);
  // Called by treeview when row is changed.
  void storeEntryData(int pos);
  // Called by self to update entry with file data and also
  // by treeview when row data is edited.
  void restoreEntryData(int pos);
  void clearProgressBar();

 private:
  void onDirEntryEnterPress();
  void onOpenDirBtnPress();
  void onRefreshDirBtnPress();
  void onMetadataEntryEnterPress();
  void onSaveFileBtnPress();
  void onSearchWebBtnPress();
  void onSaveAllFilesBtnPress();
  bool onClickProgressBar(GdkEventButton* button_event);
  void onCancelBtnPress();
  bool onCloseWindow(bool);
  bool on_key_press_event(GdkEventKey* event) override;

  void onOperationUpdate();

  void openDirDialog();
  bool storeEntryDataAndUpdateSelectedRowAndEntry();
  void updateProgressBar(const std::string& filename, int processed_files,
                         int total_files, bool done_processing);
  void selectDirEntry();
  void showErrorDialog();
  void toggleLoadingMode(bool enter_loading_mode);
  void enterProcessingMode(Files::Files::ProcessingMode processing_mode);
  void attemptBrowserSearch();
  std::string getSongTitleOrFilename();

  void preOpLoadEntryDir();
  void postOpLoadEntryDir();
  void preOpSaveFile();
  void postOpSaveFile();
  void preOpSaveAllFiles();
  void postOpSaveAllFiles();

  Glib::Dispatcher dispatcher_;
  Files::Files files_;
  Files::Files::ProcessingMode processing_mode_;
  std::thread* thread_;

  Glib::RefPtr<Gtk::Builder> builder_;
  Gtk::Entry* entry_dir_;
  Gtk::CheckButton* checkbox_read_subdir_;
  Gtk::Button* btn_dir_open_;
  Gtk::Button* btn_dir_refresh_;
  TreeViewFiles* treeview_files_;
  Gtk::CheckButton* checkbox_read_audio_;
  Gtk::Entry* entry_song_title_;
  Gtk::Entry* entry_song_artist_;
  Gtk::Entry* entry_song_album_;
  Gtk::Entry* entry_song_track_num_;
  Gtk::Entry* entry_song_track_denum_;
  Gtk::CheckButton* checkbox_rename_file_;
  Gtk::Button* btn_song_save_single_;
  Gtk::Button* btn_song_search_web_;
  Gtk::Button* btn_song_save_all_;
  Gtk::EventBox* eventbox_progressbar_main_;
  Gtk::ProgressBar* progressbar_main_;
  Gtk::Button* btn_cancel_action_;
  ErrorDialog* gtk_dialog_error_dialog_;
};

}  // namespace Gui
}  // namespace Mp3Edit

#endif  // MP3EDIT_GUI_WINDOW_MAIN_H_
