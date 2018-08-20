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

#include "mp3edit/src/file.h"
#include "mp3edit/src/files.h"

namespace Mp3Edit {
namespace Gui {

class TreeViewFiles: public Gtk::TreeView {
 public:
  TreeViewFiles(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>&,
                Files::Files& files, Gtk::Entry* entry_title,
                Gtk::Entry* entry_artist, Gtk::Entry* entry_album,
                Gtk::Entry* entry_track_num, Gtk::Entry* entry_track_denum);
  void populateTreeView();
  void saveSelectedFile(bool rename_file);
  void saveAllFiles(bool rename_file);
 private:
  enum class EditType {
    kRow = 0,
    kEntry = 1,
    kUnedited = 2,
  };
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

  void saveSelectedFile(Gtk::TreeModel::Row& row, bool rename_file,
                        bool is_single_file);
  void onEditTypeRow(const Glib::ustring&,
                     const Glib::ustring&);
  void onEditTypeEntry();
  void onEntryEnterPress();
  void storeAndUpdateEntryData();

  void getRowData(const Gtk::TreeModel::Row& row,
                  std::string& title, std::string& artist,
                  std::string& album, std::string& track);

  void getEntryData(std::string& title, std::string& artist, std::string& album,
                    int& track_num, int& track_denum);

  void storeCurrentEditsInFileMem();

  void updateCurrentRowFromFileMem();
  void updateEntryFromFileMem();
  void onRowSelect();

  Files::Files& files_;

  Glib::RefPtr<Gtk::ListStore> liststore_;
  Glib::RefPtr<Gtk::TreeSelection> treeselection_;
  Columns columns_;

  Gtk::Entry* entry_song_title_;
  Gtk::Entry* entry_song_artist_;
  Gtk::Entry* entry_song_album_;
  Gtk::Entry* entry_song_track_num_;
  Gtk::Entry* entry_song_track_denum_;

  // To remember what was edited
  EditType edit_type_;
  Gtk::TreeModel::iterator current_row_;

  // UI
  bool disable_signals_;
};

}  // namespace Gui
}  // namespace Mp3Edit

#endif  // MP3EDIT_SRC_GUI_TREEVIEW_FILES_H_
