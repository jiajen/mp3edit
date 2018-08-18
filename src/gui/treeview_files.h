#ifndef MP3EDIT_SRC_GUI_TREEVIEW_FILES_H_
#define MP3EDIT_SRC_GUI_TREEVIEW_FILES_H_

#include <vector>

#include <gtkmm/builder.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>

#include "mp3edit/src/file.h"

namespace Mp3Edit {
namespace Gui {

class TreeViewFiles: public Gtk::TreeView {
 public:
  TreeViewFiles(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>&,
                std::vector<File::File>& files);
  void populateTreeView();
 private:
  class Columns;

  std::vector<File::File>& files_;

  Glib::RefPtr<Gtk::ListStore> liststore_;
  Columns columns_;
};

}  // namespace Gui
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_GUI_TREEVIEW_FILES_H_
