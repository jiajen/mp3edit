#include "main.h"

#include <glibmm/refptr.h>
#include <gtkmm/application.h>
#include <gtkmm/builder.h>

#include "mp3edit/src/gui/window_main.h"

namespace {

const char* kApplicationId = "me.jiajen.mp3edit";
const char* kApplicationGladeFile = "mp3edit.glade";

}  // namespace

int main() {
  Mp3Edit::Gui::WindowMain* window_main;
  Glib::RefPtr<Gtk::Builder> builder;
  auto app = Gtk::Application::create(kApplicationId);
  try {
    builder = Gtk::Builder::create_from_file(kApplicationGladeFile);
  } catch (const Glib::Exception&) {
    g_critical("UI File not found.");
    return 1;
  }
  builder->get_widget_derived("gtk_window_main", window_main);
  app->run(*window_main);
  return 0;
}
