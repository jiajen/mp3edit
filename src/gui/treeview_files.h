#ifndef MP3EDIT_SRC_GUI_TREEVIEW_FILES_H_
#define MP3EDIT_SRC_GUI_TREEVIEW_FILES_H_

#include <string>

#include <gtkmm/builder.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/entry.h>
#include <gtkmm/treeselection.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/progressbar.h>

#include "mp3edit/src/file.h"
#include "mp3edit/src/files.h"

namespace Mp3Edit {
namespace Gui {

class WindowMain;

class TreeViewFiles: public Gtk::TreeView {
 public:
  TreeViewFiles(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>&,
                WindowMain* parent_window, Files::Files& files);
  void populateTreeView();
 private:
  class Columns : public Gtk::TreeModel::ColumnRecord {
    typedef Gtk::TreeModelColumn<std::string> Column;
   public:
    Columns();
    inline Gtk::TreeModelColumn<int>& pos() { return pos_; }
    inline Column& title() { return title_; }
    inline Column& artist() { return artist_; }
    inline Column& album() { return album_; }
    inline Column& track() { return track_; }
    inline Column& bitrate() { return bitrate_; }
    inline Column& samplingRate() { return sampling_rate_; }
    inline Column& channelMode() { return channel_mode_; }
    inline Column& filepath() { return filepath_; }
   private:
    Gtk::TreeModelColumn<int> pos_;
    Column title_;
    Column artist_;
    Column album_;
    Column track_;
    Column bitrate_;
    Column sampling_rate_;
    Column channel_mode_;
    Column filepath_;
  };

  void onRowDataEdit(const Glib::ustring&, const Glib::ustring&);

  void getEntryData(std::string& title, std::string& artist, std::string& album,
                    int& track_num, int& track_denum);

  void onRowSelect();

  Files::Files& files_;

  Glib::RefPtr<Gtk::ListStore> liststore_;
  Glib::RefPtr<Gtk::TreeSelection> treeselection_;
  Columns columns_;

  WindowMain* parent_window_;
};

}  // namespace Gui
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_GUI_TREEVIEW_FILES_H_
