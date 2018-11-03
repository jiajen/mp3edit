#include "mp3edit/src/main.h"

#include <glibmm/refptr.h>
#include <gtkmm/application.h>
#include <gtkmm/builder.h>

#include "mp3edit/src/gui/window_main.h"

namespace {

const char* kApplicationId = "me.jiajen.mp3edit";

#ifndef RELEASE_BUILD
const char* kApplicationGladeFile = "mp3edit.glade";
#endif  // RELEASE_BUILD

}  // namespace

int main() {
  Mp3Edit::Gui::WindowMain* window_main;
  Glib::RefPtr<Gtk::Builder> builder;
  auto app = Gtk::Application::create(kApplicationId);
  try {
#ifdef RELEASE_BUILD
    builder = Gtk::Builder::create_from_string(kApplicationGladeContent);
#else
    builder = Gtk::Builder::create_from_file(kApplicationGladeFile);
#endif  // RELEASE_BUILD
  } catch (const Glib::Exception&) {
    g_critical("Unable to generate UI.");
    return 1;
  }
  builder->get_widget_derived("gtk_window_main", window_main);
  app->run(*window_main);
  return 0;
}
