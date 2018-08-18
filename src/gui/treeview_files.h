#ifndef MP3EDIT_SRC_GUI_TREEVIEW_FILES_H_
#define MP3EDIT_SRC_GUI_TREEVIEW_FILES_H_

#include <gtkmm/builder.h>
#include <gtkmm/treeview.h>

namespace Mp3Edit {
namespace Gui {

class TreeViewFiles: public Gtk::TreeView {
 public:
  TreeViewFiles(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>&);
 private:
};

}  // namespace Gui
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_GUI_TREEVIEW_FILES_H_
