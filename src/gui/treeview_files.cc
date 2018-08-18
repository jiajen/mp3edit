#include "mp3edit/src/gui/treeview_files.h"

#include <gtkmm/liststore.h>

namespace Mp3Edit {
namespace Gui {

TreeViewFiles::Columns::Columns() {
  add(pos_);
  add(filepath_);
  add(title_);
  add(artist_);
  add(album_);
  add(track_);
  add(bitrate_);
  add(sampling_rate_);
  add(channel_mode_);
}

TreeViewFiles::TreeViewFiles(BaseObjectType* cobject,
                             const Glib::RefPtr<Gtk::Builder>&,
                             std::vector<File::File>& files)
    : Gtk::TreeView(cobject), files_(files) {
  liststore_ = Gtk::ListStore::create(columns_);
}

void TreeViewFiles::populateTreeView() {
  // TODO fill
}

}  // namespace Gui
}  // namespace Mp3Edit
