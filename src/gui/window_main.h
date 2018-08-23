#ifndef MP3EDIT_SRC_GUI_WINDOW_MAIN_H_
#define MP3EDIT_SRC_GUI_WINDOW_MAIN_H_

#include <glibmm/dispatcher.h>
#include <gtkmm/builder.h>
#include <gtkmm/window.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/progressbar.h>

#include "mp3edit/src/files.h"
#include "mp3edit/src/gui/treeview_files.h"

namespace Mp3Edit {
namespace Gui {

class WindowMain: public Gtk::Window {
 public:
  WindowMain(BaseObjectType* cobject,
             const Glib::RefPtr<Gtk::Builder>& builder);
 private:
  void openDirDialog();
  void loadEntryDir();
  void onSaveFileBtnPress();
  void onSaveAllFilesBtnPress();
  void onCancelBtnPress();
  void onSearchWebBtnPress();
  void toggleLoadingMode(bool enter_loading_mode);
  void onOperationUpdate();

  Glib::Dispatcher dispatcher_;
  Files::Files files_;
  Files::Files::ProcessingMode processing_mode_;

  Glib::RefPtr<Gtk::Builder> builder_;
  Gtk::CheckButton* checkbox_read_subdir_;
  Gtk::CheckButton* checkbox_read_audio_;
  Gtk::Button* btn_dir_open_;
  Gtk::Button* btn_dir_refresh_;
  Gtk::Entry* entry_dir_;
  TreeViewFiles* treeview_files_;
  Gtk::Entry* entry_song_title_;
  Gtk::Entry* entry_song_artist_;
  Gtk::Entry* entry_song_album_;
  Gtk::Entry* entry_song_track_num_;
  Gtk::Entry* entry_song_track_denum_;
  Gtk::Button* btn_song_search_web_;
  Gtk::Button* btn_song_save_single_;
  Gtk::Button* btn_song_save_all_;
  Gtk::CheckButton* checkbox_rename_file_;
  Gtk::ProgressBar* progressbar_main_;
  Gtk::Button* btn_cancel_action_;
};

}  // namespace Gui
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_GUI_WINDOW_MAIN_H_
