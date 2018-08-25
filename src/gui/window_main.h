#ifndef MP3EDIT_SRC_GUI_WINDOW_MAIN_H_
#define MP3EDIT_SRC_GUI_WINDOW_MAIN_H_

#include <thread>

#include <glibmm/dispatcher.h>
#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/window.h>
#include <gtkmm/entry.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/button.h>
#include <gtkmm/progressbar.h>

#include "mp3edit/src/files.h"
#include "mp3edit/src/gui/treeview_files.h"

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
 private:
  void onDirEntryEnterPress();
  void onOpenDirBtnPress();
  void onRefreshDirBtnPress();
  void onMetadataEntryEnterPress();
  void onSaveFileBtnPress();
  void onSearchWebBtnPress();
  void onSaveAllFilesBtnPress();
  void onCancelBtnPress();

  void onOperationUpdate();

  void openDirDialog();
  bool storeEntryDataAndUpdateSelectedRowAndEntry();
  void toggleLoadingMode(bool enter_loading_mode);
  void enterProcessingMode(Files::Files::ProcessingMode processing_mode);

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
  Gtk::ProgressBar* progressbar_main_;
  Gtk::Button* btn_cancel_action_;
};

}  // namespace Gui
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_GUI_WINDOW_MAIN_H_
