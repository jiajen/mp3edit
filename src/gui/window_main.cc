#include "mp3edit/src/gui/window_main.h"

#include "mp3edit/src/directory.h"

namespace Mp3Edit {
namespace Gui {

using File::File;

WindowMain::WindowMain(BaseObjectType* cobject,
                       const Glib::RefPtr<Gtk::Builder>& builder)
    : Gtk::Window(cobject), builder_(builder) {
  builder_->get_widget("btn_dir_open", btn_dir_open_);
  btn_dir_open_->signal_clicked().connect(
    sigc::mem_fun(*this, &WindowMain::openDirDialog));
  builder_->get_widget("btn_dir_refresh", btn_dir_refresh_);
  btn_dir_refresh_->signal_clicked().connect(
    sigc::mem_fun(*this, &WindowMain::loadEntryDir));

  builder_->get_widget("entry_dir", entry_dir_);
}

void WindowMain::openDirDialog() {
  // TODO
}

void WindowMain::loadEntryDir() {
  std::vector<File> files = Directory::getFiles(entry_dir_->get_text(), false);
  // TODO update gui
  // TODO pass value of checkbox (read audio data) to getFiles
}

}  // namespace Gui
}  // namespace Mp3Edit
