#include "mp3edit/src/gui/treeview_files.h"

namespace Mp3Edit {
namespace Gui {

TreeViewFiles::TreeViewFiles(BaseObjectType* cobject,
                             const Glib::RefPtr<Gtk::Builder>&)
    : Gtk::TreeView(cobject) {}

}  // namespace Gui
}  // namespace Mp3Edit
