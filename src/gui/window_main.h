#ifndef MP3EDIT_SRC_GUI_WINDOW_MAIN_H_
#define MP3EDIT_SRC_GUI_WINDOW_MAIN_H_

#include <vector>

#include <gtkmm/builder.h>
#include <gtkmm/window.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>

#include "mp3edit/src/file.h"
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

  std::vector<File::File> files_;

  Glib::RefPtr<Gtk::Builder> builder_;
  Gtk::CheckButton* checkbox_read_subdir_;
  Gtk::CheckButton* checkbox_read_audio_;
  Gtk::Button* btn_dir_open_;
  Gtk::Button* btn_dir_refresh_;
  Gtk::Entry* entry_dir_;
  TreeViewFiles* treeview_files_;
};

}  // namespace Gui
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_GUI_WINDOW_MAIN_H_
