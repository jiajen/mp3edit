#include "mp3edit/src/gui/error_dialog.h"

namespace Mp3Edit {
namespace Gui {

namespace {
  const char* kDialogTitle = "Status";
}  // namespace

ErrorDialog::ErrorDialog(const std::vector<Files::FilesError>& files)
    : Gtk::MessageDialog(kDialogTitle, false,
                         Gtk::MessageType::MESSAGE_INFO,
                         Gtk::ButtonsType::BUTTONS_OK, false), files_(files) {}

}  // namespace Gui
}  // namespace Mp3Edit
