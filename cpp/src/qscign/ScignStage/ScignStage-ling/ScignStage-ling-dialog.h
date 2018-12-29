
//          Copyright Nathaniel Christen 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef SCIGNSTAGE_LING_DIALOG__H
#define SCIGNSTAGE_LING_DIALOG__H

#include <QObject>

#include <QMetaType>

#include <QList>
#include <QStack>


#include <QPoint>

#include <QDialog>
#include <QTableWidget>

#include <functional>

#include "accessors.h"
#include "qsns.h"

#include "nav-protocols/nav-ling-1d-panel.h"

#include <functional>

#include "kans.h"

KANS_CLASS_DECLARE(DSM ,Language_Sample)
KANS_CLASS_DECLARE(DSM ,Language_Sample_Group)
KANS_CLASS_DECLARE(DSM, Dataset)

USING_KANS(DSM)

class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QTabWidget;
class QTextEdit;
class QFrame;
class QHBoxLayout;
class QVBoxLayout;
class QSlider;
class QPlainTextEdit;
class QBoxLayout;
class QButtonGroup;
class QGroupBox;
class QScrollArea;
class QGridLayout;
class QMediaPlayer;
class QProcess;
class QTcpServer;
class QTreeWidget;
class QTreeWidgetItem;
class QAbstractButton;
class QSplitter;
class QComboBox;

class ScignStage_Clickable_Label;

class XPDF_Bridge;

KANS_CLASS_DECLARE(PhaonLib ,Phaon_Runner)
USING_KANS(PhaonLib)


QSNS_(ScignStage)
_QSNS(ScignStage)
//?namespace QScign { namespace ScignStage {



class ScignStage_Ling_Dialog : public QDialog
{
 Q_OBJECT

 QHBoxLayout* minimize_layout_;

 QDialogButtonBox* button_box_;
 QPushButton* button_ok_;
 QPushButton* button_cancel_;
 QPushButton* button_proceed_;


 QHBoxLayout* middle_layout_;
 QVBoxLayout* main_layout_;


 // //  "Pseudo" Toolbar ...
 //QHBoxLayout* top_buttons_layout_;

 QPushButton* launch_config_button_;
 QPushButton* activate_tcp_button_;
 QPushButton* take_screenshot_button_;

 QHBoxLayout* filters_layout_;
 QGridLayout* filter_forms_grid_layout_;
 QGridLayout* filter_issues_grid_layout_;

 QGridLayout* quasi_toolbar_layout_;
 QHBoxLayout* config_layout_;

 QButtonGroup* filter_forms_button_group_;
 QButtonGroup* filter_issues_button_group_;

 QGroupBox* filter_forms_group_box_;
 QGroupBox* filter_issues_group_box_;

 QStringList issues_;
 QStringList forms_;

// QScrollArea* tree_scroll_area_;
// QFrame* main_frame_;

 QTreeWidget* main_tree_widget_;
// QVBoxLayout* main_tree_layout_;

 //QGridLayout* main_grid_layout_;

 NAV_Ling1D_Panel* nav_panel_;

 XPDF_Bridge* xpdf_bridge_;

 QVector<Language_Sample*>* samples_;
 QVector<Language_Sample_Group*>* groups_;

 //QSet<QString>* temp_filters_;

 QSet<QString> current_filters_;

 QHBoxLayout* full_sentence_layout_;
 QSplitter* full_sentence_splitter_;
 QLabel* full_sentence_pre_label_;
 QLabel* full_sentence_post_label_;
 QPlainTextEdit* full_sentence_plain_text_edit_;

 QVBoxLayout* show_original_version_layout_;
 QPushButton* show_original_version_button_;
 QGroupBox* show_original_version_group_box_;


 QFrame* select_chapter_frame_;
 QLabel* select_chapter_label_;
 QComboBox* select_chapter_combo_box_;
 QHBoxLayout* select_chapter_layout_;

 QStack<QMenu*> popped_up_menus_;

 void show_full_sentence(Language_Sample_Group* g);
 void show_full_sentence(Language_Sample* samp);

 void chapter_to_string(QString& result, bool wl);
 void group_to_string(QString& result, bool wl);

 void save_to_user_select_file(QString text);

 QMap<Language_Sample*, QPair<QLabel*, int> > sample_to_label_map_;

 Language_Sample* current_sample_;

 int current_index_;

 int max_index_;

 QWidget* last_highlight_;

 int current_volume_;

 QProcess* xpdf_process_;

 quint64 current_tcp_msecs_;

 void* application_model_;

 //int xpdf_port_;
 QTcpServer* tcp_server_;

 QString held_xpdf_msg_;

 Phaon_Runner* phr_;

 std::function<void(Phaon_Runner&)> phr_init_function_;
 std::function<void()> screenshot_function_;
 std::function<void()> launch_config_function_;

 std::function<void(QString)> launch_lexpair_dialog_function_;

 int current_group_index_;
 Language_Sample_Group* current_open_group_;

 QMap<Language_Sample_Group*, QTreeWidgetItem*> twi_by_group_;

 void* no_auto_expand_;
 int current_peer_index_;

 int current_chapter_number_;
 QMap<int, QPair<int, int>> chapter_groups_first_last_;

 bool xpdf_is_ready();
 void check_phr();

 void run_msg(QString msg, QByteArray qba);
 void run_kph(const QByteArray& qba);


 void open_pdf_file(QString name, int page);

 void check_launch_xpdf(std::function<void()> fn,
   std::function<void()> waitfn);
 void send_xpdf_msg(QString msg);

 QString load_about_file(QString name);

 bool ask_pdf_proceed(QString name);

 void set_group_foreground(QTreeWidgetItem* twi);
 void clear_group_foreground(QTreeWidgetItem* twi);

 void set_full_sentence_from_child(QTreeWidgetItem* twi);
 void set_child_group_foreground(QTreeWidgetItem* twi);
 void clear_child_group_foreground(QTreeWidgetItem* twi);

 void run_chapter_message(const QPoint& p, int col);
 void run_group_message(const QPoint& p, int col);

 void check_expand(QTreeWidgetItem* twi);
 void expand(QTreeWidgetItem* twi);

 void handle_user_expand(QTreeWidgetItem* twi);

 void highlight(QTreeWidgetItem* twi);

 void highlight(QTreeWidgetItem* twi,
   Language_Sample_Group* g, int* index = nullptr);

 void highlight(QTreeWidgetItem* twi,
   int* index)
 {
  highlight(twi, nullptr, index);
 }

 void run_group_context_menu(const QPoint& p, int page, QString text,
   QStringList texts,
   std::function<void(int)> pdf_fn,
   std::function<void(QString)> copy_fn,
   std::function<void(QString)> launch_fn,
   std::function<void(QStringList)> copies_fn,
   std::function<void()> highlight_fn);

 void run_sample_context_menu(const QPoint& p, int page, QString text,
   std::function<void(int)> pdf_fn,
   std::function<void(QString)> copy_fn,
   std::function<void(QString)> launch_fn);

 void run_message_by_grid_position(const QPoint& p, int r, int c);

public:



 ScignStage_Ling_Dialog(XPDF_Bridge* xpdf_bridge,
   Dataset& ds, QWidget* parent = nullptr);

 ~ScignStage_Ling_Dialog();

 ACCESSORS__SET(std::function<void(Phaon_Runner&)>, phr_init_function)
 ACCESSORS__SET(std::function<void()> ,screenshot_function)
 ACCESSORS__SET(std::function<void(QString)> ,launch_lexpair_dialog_function)
 ACCESSORS__SET(std::function<void()> ,launch_config_function)

 ACCESSORS(void* ,application_model)

 // //  Kernel Application Interface
 void test_msgbox(QString msg);
 void expand_sample(int index);
 void launch_lexpair_dialog(QString s);

 void find_sample_down(Language_Sample_Group* start,
   QSet<QString>* temp_filters);

 void find_sample_up(Language_Sample_Group* start,
   QSet<QString>* temp_filters);


Q_SIGNALS:
 void canceled(QDialog*);
 void accepted(QDialog*);
 void take_screenshot_requested();
 void launch_config_requested();

public Q_SLOTS:

 void checked_label_change(QAbstractButton* qab, bool checked);

 void handle_xpdf_is_ready();
 void handle_take_screenshot_requested();
 void handle_launch_config_requested();

 void accept();
 void cancel();

 void activate_tcp_requested();

 void handle_sample_down();
 void handle_sample_up();

 void handle_sample_first();

 void handle_filtered_down();
 void handle_filtered_up();

 void handle_peer_down();
 void handle_peer_up();

 void handle_chapter_down();
 void handle_chapter_up();

 void handle_chapter_start();
 void handle_chapter_end();

};

//_QSNS(ScignStage)


#endif  // SCIGNSTAGE_AUDIO_DIALOG__H


