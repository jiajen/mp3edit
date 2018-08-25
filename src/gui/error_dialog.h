#ifndef MP3EDIT_SRC_GUI_ERROR_DIALOG_H_
#define MP3EDIT_SRC_GUI_ERROR_DIALOG_H_

#include <vector>

#include <gtkmm/messagedialog.h>

#include "mp3edit/src/files.h"

namespace Mp3Edit {
namespace Gui {

class ErrorDialog: public Gtk::MessageDialog {
 public:
   ErrorDialog(const std::vector<Files::FilesError>& files);
 private:
  const std::vector<Files::FilesError>& files_;
};

}  // namespace Gui
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_GUI_ERROR_DIALOG_H_
