#ifndef MP3EDIT_GUI_TREEVIEW_FILES_H_
#define MP3EDIT_GUI_TREEVIEW_FILES_H_

#include <string>

#include <sigc++/connection.h>
#include <glibmm/refptr.h>
#include <gtkmm/builder.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeselection.h>

#include "mp3edit/src/files.h"

namespace Mp3Edit {
namespace Gui {

class WindowMain;

class TreeViewFiles: public Gtk::TreeView {
 public:
  TreeViewFiles(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>&,
                WindowMain* parent_window, Files::Files& files);
  void populateTreeView(int selected_file_idx);
  void restoreSelectedRowData();
  int getSelectedFileIdx();
  void updateSelectedRowFilepath();
  void updateAllRowsFilepath();
  void removeSelectedRow();

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

  void onRowSelect();
  void onRowDataEdit(const Glib::ustring&, const Glib::ustring&);

  void unSelectRowIfSelected();
  void storeRowData(const Gtk::TreeModel::Row& row);
  void restoreRowData(const Gtk::TreeModel::Row& row);
  void enableRowSignal();

  void runWithoutSignal(void (TreeViewFiles::* function_ptr)());
  void runWithoutSignal(void (TreeViewFiles::* function_ptr)(int), int val);

  Files::Files& files_;
  Gtk::TreeModel::iterator last_selected_row_ptr_;
  sigc::connection signal_row_change_;

  Glib::RefPtr<Gtk::ListStore> liststore_;
  Glib::RefPtr<Gtk::TreeSelection> treeselection_;
  Columns columns_;

  WindowMain* parent_window_;
};

}  // namespace Gui
}  // namespace Mp3Edit

#endif  // MP3EDIT_GUI_TREEVIEW_FILES_H_
