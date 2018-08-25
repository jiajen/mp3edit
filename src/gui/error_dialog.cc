#include "mp3edit/src/gui/error_dialog.h"

namespace Mp3Edit {
namespace Gui {

namespace {

const char* kFilepath = "Filepath";
const char* kError = "Error";

void appendColumn(Gtk::TreeView* tree_view, const char* column_title,
                  Gtk::TreeModelColumn<std::string>& column) {
  tree_view->get_column(tree_view->append_column(
    column_title, column)-1)->set_sort_column(column);
}

}  // namespace

ErrorDialog::Columns::Columns() {
  add(filepath_);
  add(error_);
}

ErrorDialog::ErrorDialog(BaseObjectType* cobject,
                         const Glib::RefPtr<Gtk::Builder>& builder,
                         const std::vector<Files::FilesError>& errors)
    : Gtk::MessageDialog(cobject), errors_(errors), builder_(builder) {
  builder_->get_widget("gtk_treeview_error_dialog", treeview_);
  liststore_ = Gtk::ListStore::create(columns_);
  treeview_->set_model(liststore_);
  treeselection_ = treeview_->get_selection();
  appendColumn(treeview_, kFilepath, columns_.filepath());
  appendColumn(treeview_, kError, columns_.error());
}

void ErrorDialog::run() {
  // TODO fill treeview
  Gtk::MessageDialog::run();
}

}  // namespace Gui
}  // namespace Mp3Edit
