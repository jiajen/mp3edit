#ifndef MP3EDIT_GUI_ERROR_DIALOG_H_
#define MP3EDIT_GUI_ERROR_DIALOG_H_

#include <string>
#include <vector>

#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeselection.h>

#include "mp3edit/src/files.h"

namespace Mp3Edit {
namespace Gui {

class ErrorDialog: public Gtk::MessageDialog {
 public:
  ErrorDialog(BaseObjectType* cobject,
               const Glib::RefPtr<Gtk::Builder>& builder,
               const std::vector<Files::FilesError>& files);
  void run();

 private:
  class Columns : public Gtk::TreeModel::ColumnRecord {
    typedef Gtk::TreeModelColumn<std::string> Column;

   public:
    Columns();
    inline Column& filepath() { return filepath_; }
    inline Column& error() { return error_; }

   private:
    Column filepath_;
    Column error_;
  };

  bool on_key_press_event(GdkEventKey* event) override;

  const std::vector<Files::FilesError>& errors_;

  Glib::RefPtr<Gtk::Builder> builder_;
  Gtk::TreeView* treeview_;
  Glib::RefPtr<Gtk::ListStore> liststore_;
  Glib::RefPtr<Gtk::TreeSelection> treeselection_;
  Columns columns_;
};

}  // namespace Gui
}  // namespace Mp3Edit

#endif  // MP3EDIT_GUI_ERROR_DIALOG_H_
