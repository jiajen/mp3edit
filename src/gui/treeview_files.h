#ifndef MP3EDIT_SRC_GUI_TREEVIEW_FILES_H_
#define MP3EDIT_SRC_GUI_TREEVIEW_FILES_H_

#include <vector>
#include <string>

#include <gtkmm/builder.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treemodelcolumn.h>

#include "mp3edit/src/file.h"

namespace Mp3Edit {
namespace Gui {

class TreeViewFiles: public Gtk::TreeView {
 public:
  TreeViewFiles(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>&,
                std::vector<File::File>& files);
  void populateTreeView();
 private:
  class Columns : public Gtk::TreeModel::ColumnRecord {
   public:
    Columns();
   private:
    Gtk::TreeModelColumn<int> pos_;
    Gtk::TreeModelColumn<std::string> filepath_;
    Gtk::TreeModelColumn<std::string> title_;
    Gtk::TreeModelColumn<std::string> artist_;
    Gtk::TreeModelColumn<std::string> album_;
    Gtk::TreeModelColumn<std::string> track_;
    Gtk::TreeModelColumn<std::string> bitrate_;
    Gtk::TreeModelColumn<std::string> sampling_rate_;
    Gtk::TreeModelColumn<std::string> channel_mode_;
  };

  std::vector<File::File>& files_;

  Glib::RefPtr<Gtk::ListStore> liststore_;
  Columns columns_;
};

}  // namespace Gui
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_GUI_TREEVIEW_FILES_H_
