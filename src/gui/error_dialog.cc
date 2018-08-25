#include "mp3edit/src/gui/error_dialog.h"

#include <gtkmm/clipboard.h>

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
  liststore_->clear();
  for (int i = 0, n = errors_.size(); i < n; i++) {
    Gtk::TreeModel::Row row = *(liststore_->append());
    row[columns_.filepath()] = (std::string)errors_[i].path();
    row[columns_.error()] = errors_[i].what();
  }
  Gtk::MessageDialog::run();
}

bool ErrorDialog::on_key_press_event(GdkEventKey* event) {
  using std::string;
  if (event->state == GDK_CONTROL_MASK &&
      (event->keyval == GDK_KEY_c || event->keyval == GDK_KEY_C)) {
    Gtk::TreeModel::iterator row_it = treeselection_->get_selected();
    if (!row_it) return Gtk::Window::on_key_press_event(event);
    Gtk::Clipboard::get()->set_text((string)((*row_it)[columns_.filepath()]));
    return true;
  }
  return Gtk::Window::on_key_press_event(event);
}

}  // namespace Gui
}  // namespace Mp3Edit
