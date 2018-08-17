#include "mp3edit/src/gui/window_main.h"

#include <glib.h>
#include <gtkmm/filechooserdialog.h>

#include "mp3edit/src/directory.h"

namespace Mp3Edit {
namespace Gui {

using File::File;

WindowMain::WindowMain(BaseObjectType* cobject,
                       const Glib::RefPtr<Gtk::Builder>& builder)
    : Gtk::Window(cobject), builder_(builder) {
  builder_->get_widget("checkbox_read_subdir", checkbox_read_subdir_);
  builder_->get_widget("checkbox_read_audio", checkbox_read_audio_);

  builder_->get_widget("btn_dir_open", btn_dir_open_);
  btn_dir_open_->signal_clicked().connect(
    sigc::mem_fun(*this, &WindowMain::openDirDialog));

  builder_->get_widget("btn_dir_refresh", btn_dir_refresh_);
  btn_dir_refresh_->signal_clicked().connect(
    sigc::mem_fun(*this, &WindowMain::loadEntryDir));

  builder_->get_widget("entry_dir", entry_dir_);
}

void WindowMain::openDirDialog() {
  Gtk::FileChooserDialog dir_chooser("Select a directory",
                                     Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
  dir_chooser.set_transient_for(*this);

  dir_chooser.add_button("Open", Gtk::RESPONSE_OK);
  dir_chooser.add_button("Cancel", Gtk::RESPONSE_CANCEL);
  dir_chooser.set_filename(g_get_home_dir());

  if (dir_chooser.run() == Gtk::RESPONSE_OK) {
    entry_dir_->set_text(dir_chooser.get_filename());
    loadEntryDir();
  }
}

void WindowMain::loadEntryDir() {
  files_ = Directory::getFiles(entry_dir_->get_text(),
                               checkbox_read_audio_->get_active());
  // TODO update gui
  // TODO pass value of checkbox (read audio data) to getFiles
}

}  // namespace Gui
}  // namespace Mp3Edit
