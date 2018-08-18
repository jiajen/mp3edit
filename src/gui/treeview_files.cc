#include "mp3edit/src/gui/treeview_files.h"

namespace Mp3Edit {
namespace Gui {

TreeViewFiles::TreeViewFiles(BaseObjectType* cobject,
                             const Glib::RefPtr<Gtk::Builder>&,
                             std::vector<File::File>& files)
    : Gtk::TreeView(cobject), files_(files) {}

void TreeViewFiles::populateTreeView() {
  // TODO fill
}

}  // namespace Gui
}  // namespace Mp3Edit
