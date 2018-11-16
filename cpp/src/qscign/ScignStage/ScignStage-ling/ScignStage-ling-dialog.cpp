
//          Copyright Nathaniel Christen 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include "scignstage-ling-dialog.h"

#include "styles.h"




#include <QApplication>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QButtonGroup>
#include <QScrollArea>
#include <QFileDialog>
#include <QTabWidget>
#include <QSplitter>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QGroupBox>

#include <QDirIterator>

#include <QPlainTextEdit>
#include <QTextStream>

#include <QTreeWidget>

#include <QtMultimedia/QMediaPlayer>

#include <QPainter>
#include <QPushButton>
#include <QLabel>

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QTableWidget>
#include <QGraphicsPixmapItem>

#include <QMessageBox>
#include <QDebug>
#include <QClipboard>

#include <QProcess>

#include <QGraphicsView>
#include <QScrollArea>
#include <QTcpServer>
#include <QNetworkAccessManager>

#include <QHeaderView>

#include <QMenu>
#include <QAction>

#include <QListWidget>

#include "dsmain/language-sample.h"
#include "dsmain/language-sample-group.h"

#include "xpdf-bridge.h"


#include "PhaonLib/phaon-runner.h"

#include "kauvir-phaon/kph-command-package.h"

#include "kauvir-code-model/kcm-channel-group.h"
#include "kauvir-code-model/kauvir-code-model.h"

#include "dsmain/dataset.h"

#include "textio.h"

USING_KANS(TextIO)

USING_KANS(Phaon)

//USING_QSNS(ScignStage)


ScignStage_Ling_Dialog::ScignStage_Ling_Dialog(XPDF_Bridge* xpdf_bridge,
  Dataset& ds,
  QWidget* parent)
  : QDialog(parent), xpdf_bridge_(xpdf_bridge),
    last_sample_(nullptr),
    last_highlight_(nullptr), xpdf_process_(nullptr), tcp_server_(nullptr),
    phr_(nullptr), phr_init_function_(nullptr), screenshot_function_(nullptr),
    current_tcp_msecs_(0), xpdf_port_(0),
    current_index_(-1), max_index_(0),
    current_volume_(50), current_group_index_(-1),
    current_open_group_(nullptr), no_auto_expand_(nullptr)
{
 // // setup RZW

 samples_ = &ds.samples();
 groups_ = &ds.groups();

 button_box_ = new QDialogButtonBox(this);

 button_ok_ = new QPushButton("OK");
 button_proceed_ = new QPushButton("Proceed");
 button_cancel_ = new QPushButton("Cancel");

 button_ok_->setDefault(false);
 button_ok_->setAutoDefault(false);

 button_proceed_->setDefault(false);
 button_proceed_->setAutoDefault(false);

 button_cancel_->setDefault(true);

 button_ok_->setEnabled(false);

 // // unless this is being embedded ...
 button_proceed_->setEnabled(false);
 button_cancel_->setText("Close");

 button_box_->addButton(button_ok_, QDialogButtonBox::AcceptRole);
 button_box_->addButton(button_proceed_, QDialogButtonBox::ApplyRole);
 button_box_->addButton(button_cancel_, QDialogButtonBox::RejectRole);

 button_ok_->setStyleSheet(basic_button_style_sheet_());
 button_proceed_->setStyleSheet(basic_button_style_sheet_());
 button_cancel_->setStyleSheet(basic_button_style_sheet_());


 connect(button_proceed_, SIGNAL(clicked()), this, SLOT(proceed()));
 connect(button_box_, SIGNAL(accepted()), this, SLOT(accept()));
 connect(button_box_, SIGNAL(rejected()), this, SLOT(cancel()));

 main_layout_ = new QVBoxLayout;


 top_buttons_layout_ = new QHBoxLayout;

 take_screenshot_button_ = new QPushButton("Screenshot", this);

 activate_tcp_button_ = new QPushButton("Activate TCP", this);

 take_screenshot_button_->setStyleSheet(colorful_button_style_sheet_());
 activate_tcp_button_->setStyleSheet(colorful_button_style_sheet_());

 connect(take_screenshot_button_, SIGNAL(clicked()),
   this, SLOT(handle_take_screenshot_requested()));

 connect(activate_tcp_button_, SIGNAL(clicked()),
   this, SLOT(activate_tcp_requested()));

 top_buttons_layout_->addStretch();

 top_buttons_layout_->addWidget(activate_tcp_button_);

 top_buttons_layout_->addWidget(take_screenshot_button_);

 main_layout_->addLayout(top_buttons_layout_);

 middle_layout_ = new QHBoxLayout;

 // //   Foreground

 main_tree_widget_ = new QTreeWidget(this);

 int r = 0;

 QStringList headers {
  "Text",
  "Label",
  "Page",
  "Chapter"
 };


 main_tree_widget_->setColumnCount(4);
 main_tree_widget_->setHeaderLabels(headers);

 main_tree_widget_->setColumnWidth(0, 500);
 main_tree_widget_->setColumnWidth(1, 40);
 main_tree_widget_->setColumnWidth(2, 35);
 main_tree_widget_->setColumnWidth(3, 15);


 for(Language_Sample_Group* group : *groups_)
 {
  QString mt = group->get_main_text();
  if(mt.isEmpty())
    continue;

  QStringList qsl; // = group->all_sample_text();

  qsl.push_back(mt);
  qsl.push_back(QString::number(group->first()->index()));
  qsl.push_back(QString::number(group->first()->page()));
  qsl.push_back(QString::number(group->first()->chapter()));

  QTreeWidgetItem* twi = new QTreeWidgetItem((QTreeWidget*) nullptr,
    qsl);

  twi_by_group_[group] = twi;

  for(Language_Sample* samp: *group)
  {
   QStringList qsl; // = group->all_sample_text();
   qsl.push_back(samp->text());

   QString si;
   QString sbi = samp->sub_index();

   if(sbi.endsWith('\''))
   {
    si = "(none)";
   }
   else
   {
    sbi.replace('/', '\'');
    si = QString("%1%2").arg(samp->index())
      .arg(sbi);
   }

   qsl.push_back(si);
   qsl.push_back(QString::number(samp->chapter()));
   qsl.push_back(QString::number(samp->page()));
   QTreeWidgetItem* stwi = new QTreeWidgetItem((QTreeWidget*) nullptr,
     qsl);
   twi->addChild(stwi);
  }


  main_tree_widget_->addTopLevelItem(twi);
  //items.append(twi);
 }

 middle_layout_->addWidget(main_tree_widget_);

 connect(main_tree_widget_, &QTableWidget::customContextMenuRequested, [this](const QPoint& qp)
 {
  qDebug() << qp;

  QWidget* qw = QApplication::widgetAt(main_tree_widget_->mapToGlobal(qp));

  if(qw)
  {
//   if(qw->parent() == main_frame_)
//   {
//    int i = main_grid_layout_->indexOf(qw);
//    if(i != -1)
//    {
//     int r, c, rs, cs;
//     main_grid_layout_->getItemPosition(i, &r, &c, &rs, &cs);
//     run_message_by_grid_position(qp, r, c);

//    }
//   }
  }

 });

 main_layout_->addLayout(middle_layout_);

 nav_panel_ = new NAV_Ling1D_Panel(0, 100, 50, this);

 connect(nav_panel_, SIGNAL(sample_up_requested()),
   this, SLOT(handle_sample_up()));

 connect(nav_panel_, SIGNAL(sample_down_requested()),
   this, SLOT(handle_sample_down()));

 connect(nav_panel_, SIGNAL(peer_up_requested()),
   this, SLOT(handle_peer_up()));

 connect(nav_panel_, SIGNAL(peer_down_requested()),
   this, SLOT(handle_peer_down()));

 connect(nav_panel_, SIGNAL(sample_first_requested()),
   this, SLOT(handle_sample_first()));

 connect(nav_panel_, SIGNAL(sample_replay_requested()),
   this, SLOT(handle_sample_replay()));


 connect(nav_panel_, SIGNAL(volume_change_requested(int)),
   this, SLOT(handle_volume_change_requested(int)));

 connect(nav_panel_, &NAV_Ling1D_Panel::auto_expand_changed, [this](bool b)
 {
  no_auto_expand_ = b?nullptr:nav_panel_;
 });

 main_layout_->addWidget(nav_panel_);



 main_layout_->addWidget(button_box_);

 setLayout(main_layout_);

#ifdef USING_XPDF
 // // xpdf connections ...
 if(xpdf_bridge_)
 {
  connect(xpdf_bridge_, SIGNAL(xpdf_is_ready()),
    this, SLOT(handle_xpdf_is_ready()));
 }
#endif // USING_XPDF

}



void ScignStage_Ling_Dialog::handle_sample_down()
{
 if(current_open_group_)
 {
  twi_by_group_[current_open_group_]->setExpanded(false);
 }
 while(true)
 {
  if(current_group_index_ == -1)
    current_group_index_ = 0;
  else if(current_group_index_ == groups_->size() - 1)
    current_group_index_ = 0;
  else
    ++current_group_index_;

  Language_Sample_Group* g  = groups_->at(current_group_index_);

  if(QTreeWidgetItem* twi = twi_by_group_.value(g))
  {
   current_open_group_ = g;

   // ensure last subitem is visible
   QTreeWidgetItem* stwi = twi->child(twi->childCount() - 1);
   main_tree_widget_->scrollToItem(stwi);
   main_tree_widget_->scrollToItem(twi);
   twi->setExpanded(!(bool)no_auto_expand_);
   break;
  }
 }
}

void ScignStage_Ling_Dialog::handle_sample_up()
{
 if(current_open_group_)
 {
  twi_by_group_[current_open_group_]->setExpanded(false);
 }
 while(true)
 {
  if(current_group_index_ == -1)
    current_group_index_ = groups_->size() - 1;
  else if(current_group_index_ == 0)
    current_group_index_ = groups_->size() - 1;
  else
    --current_group_index_;

  Language_Sample_Group* g  = groups_->at(current_group_index_);

  if(QTreeWidgetItem* twi = twi_by_group_.value(g))
  {
   current_open_group_ = g;

   // ensure last subitem is visible
   QTreeWidgetItem* stwi = twi->child(twi->childCount() - 1);
   main_tree_widget_->scrollToItem(stwi);
   main_tree_widget_->scrollToItem(twi);
   twi->setExpanded(!(bool)no_auto_expand_);
   break;
  }
 }
}

void ScignStage_Ling_Dialog::handle_sample_first()
{
 current_group_index_ = groups_->size() - 1;
 handle_sample_down();
}



void ScignStage_Ling_Dialog::handle_take_screenshot_requested()
{
 Q_EMIT(take_screenshot_requested());
 if(screenshot_function_)
   screenshot_function_();
}

void ScignStage_Ling_Dialog::check_phr()
{
#ifdef USING_KPH
 if(!phr_)
 {
  phr_ = new Phaon_Runner;
  if(phr_init_function_)
    phr_init_function_(*phr_);
 }
#endif
}

// // KAI
void ScignStage_Ling_Dialog::test_msgbox(QString msg)
{
 QString m = QString("Received Message: %1").arg(msg);
 QMessageBox::information(this, "Test OK", m);
}



void ScignStage_Ling_Dialog::save_to_user_select_file(QString text)
{
 QString f = QFileDialog::getSaveFileName();
 if(!f.isEmpty())
 {
  save_file(f, text);
 }
}

void ScignStage_Ling_Dialog::run_about_context_menu(const QPoint& p, int col,
  std::function<void()> about_fn,
  std::function<void()> copy_fn,  std::function<void()> save_fn)
{
 QMenu* qm = new QMenu(this);
 qm->addAction("About ...", about_fn);
 qm->addAction("Copy Data to Clipboard", copy_fn);
 qm->addAction("Save Data to File", save_fn);
 QPoint g = mapToGlobal(p);
 qm->popup(g);
}

void ScignStage_Ling_Dialog::run_sample_context_menu(const QPoint& p,
   std::function<void()> play_fn,
   std::function<void()> copy_fn)
{
 QMenu* qm = new QMenu(this);
 qm->addAction("Play ...", play_fn);
 qm->addAction("Copy Path to Clipboard", copy_fn);
 QPoint g = main_tree_widget_->mapToGlobal(p);
 qm->popup(g);
}


void ScignStage_Ling_Dialog::run_message_by_grid_position(const QPoint& p, int r, int c)
{
// static QMap<QPair<int, int>, void(ScignStage_Ling_Dialog::*)(const QPoint&, int col)>
//   static_map {{
//   {{0, 1}, &run_chapter_message},
//  }};

// auto it = static_map.find({r, c});
// if(it != static_map.end())
// {
//  (this->**it)(p, c);
//  return;
// }

// if(c == 0)
// {
//  run_sample_context_menu(p, [this, r]
//  {
//   current_index_ = r - 2;
//   play_file_at_current_index();
//  },[this, r]
//  {
//   QString f = files_[r - 2];
//   QString path = SAMPLES_FOLDER "/" + f;

//   QClipboard* clipboard = QApplication::clipboard();
//   clipboard->setText(path);
//  });
// }
}

void ScignStage_Ling_Dialog::send_xpdf_msg(QString msg)
{
 if(xpdf_bridge_)
   xpdf_bridge_->take_message(msg);
}

void ScignStage_Ling_Dialog::open_pdf_file(QString name, int page)
{
#ifdef USING_XPDF
 check_launch_xpdf([this, name, page]()
 {
  send_xpdf_msg(QString("open:%1;%2").arg(name).arg(page));
 },[this, name, page]()
 {
  held_xpdf_msg_ = QString("open:%1;%2").arg(name).arg(page);
 });
#else
 QMessageBox::information(this, "XPDF Needed",
   "You need to build the customized XPDF library "
   "to view PDF files from this application.  See "
   "build-order.txt for more information."
 );
#endif
}

void ScignStage_Ling_Dialog::run_kph(const QByteArray& qba)
{
#ifdef USING_KPH
 check_phr();

 KPH_Command_Package khp;
 khp.absorb_data(qba);

 Kauvir_Code_Model& kcm = phr_->get_kcm();

 KCM_Channel_Group kcg(kcm.channel_names());

 khp.init_channel_group(kcm, kcg);
 phr_->run(kcg);
#endif
}

void ScignStage_Ling_Dialog::run_msg(QString msg, QByteArray qba)
{
 qDebug() << QString("received: %1").arg(msg);

 if(msg == "kph")
 {
  run_kph(qba);
 }
}

bool ScignStage_Ling_Dialog::xpdf_is_ready()
{
 if(xpdf_bridge_)
   return xpdf_bridge_->is_ready();
 return false;
}

void ScignStage_Ling_Dialog::handle_xpdf_is_ready()
{
 if(!held_xpdf_msg_.isEmpty())
 {
  send_xpdf_msg(held_xpdf_msg_);
  held_xpdf_msg_.clear();
 }
}

void ScignStage_Ling_Dialog::check_launch_xpdf(std::function<void()> fn,
  std::function<void()> waitfn)
{
 if(xpdf_is_ready())
 {
  fn();
  return;
 }

 if(xpdf_bridge_)
 {
  xpdf_bridge_->init();
  waitfn();
  return;
 }
}


void ScignStage_Ling_Dialog::activate_tcp_requested()
{
#ifdef USING_KPH
 QString waiting_message;

 if(tcp_server_)
 {
  waiting_message = QString("TCP is already started, using IP: %1\nport: %2\n\n")
    .arg(tcp_server_->serverAddress().toString()).arg(tcp_server_->serverPort());
  QMessageBox::information(this, "Already Activated", waiting_message);
  return;
 }
 tcp_server_ = new QTcpServer();
 QMap<qintptr, QByteArray>* temps = new QMap<qintptr, QByteArray>();

 int port = 18261; // // r z 1

 if (!tcp_server_->listen(QHostAddress::LocalHost, port))
 {
  QMessageBox::critical(this, "TCP Initialization Failed",
                         QString("Could not use port: %1.")
                         .arg(tcp_server_->errorString()));
 }
 else
 {
  waiting_message = QString("Using IP: %1\nport: %2\n\n")
     .arg(tcp_server_->serverAddress().toString()).arg(tcp_server_->serverPort());

  QMessageBox::information(this, "TCP is Started",
                            QString(waiting_message));
 }

 QObject::connect(tcp_server_, &QTcpServer::newConnection, [this, temps]
 {
  QTcpSocket* clientConnection = tcp_server_->nextPendingConnection();
  QObject::connect(clientConnection, &QAbstractSocket::disconnected,
    clientConnection, &QObject::deleteLater);
  clientConnection->write("OK");
  QObject::connect(clientConnection, &QTcpSocket::readyRead, [clientConnection, this, temps]
  {
   qintptr sd = clientConnection->socketDescriptor();
   QByteArray received;
   while(clientConnection->bytesAvailable())
   {
    received.append(clientConnection->readAll());
   }
   if(received.endsWith("<//>"))
   {
    received.chop(4);
    QByteArray qba = (*temps)[sd];
    qba.append(received);
    temps->remove(sd);

    int index = qba.indexOf("<<>>");

    if(index != -1)
    {
     int i1 = qba.indexOf('@', index);
     int i2 = qba.indexOf(':', i1);
     QString msg = QString::fromLatin1(qba.mid(index + 4, i1 - index - 4));
     QByteArray ms = qba.mid(i1 + 1, i2 - i1 - 2);
     quint64 msecs = ms.toLongLong();
     if(msecs != current_tcp_msecs_)
     {
      current_tcp_msecs_ = msecs;
      run_msg( msg, qba.mid(i2 + 1) );
     }
    }
    clientConnection->write("END");
   }
   else
   {
    (*temps)[sd] += received;
   }
  });
 });
#else
 QMessageBox::information(this, "Kauvir/Phaon Needed",
   QString(
     "To use TCP for testing/demoing \"Kernel Application Interface\" "
     "functions you need to build several additional libraries "
     "(see the build-order.txt file for Qt project files and %1, line %2).")
     .arg(__FILE__).arg(__LINE__)
 );
#endif
}


ScignStage_Ling_Dialog::~ScignStage_Ling_Dialog()
{
 //  //  Child widgets should delete automatically ...
}


void ScignStage_Ling_Dialog::cancel()
{
 Q_EMIT(rejected());
 Q_EMIT(canceled(this));
 Q_EMIT(rejected());
 close();
}

void ScignStage_Ling_Dialog::accept()
{
 Q_EMIT(accepted(this));
}


//main_frame_ = new QFrame(this);

//main_frame_->setMinimumHeight(250);

//main_frame_->setMinimumWidth(300);

//main_frame_->setContextMenuPolicy(Qt::CustomContextMenu);

//main_grid_layout_ = new QGridLayout;
//main_tree_layout_ = new QVBoxLayout;
// int cc = 1;
// for(QString h : headers)
// {
//  QLabel* lbl = new QLabel(h, this);
//  main_grid_layout_->addWidget(lbl, 0, cc);
//  ++cc;
// }

// for(Language_Sample* samp : *samples_)
// {
//  QLabel* lbl = new QLabel(samp->text(), this);
//  sample_to_label_map_[samp] = {lbl, r};

//  main_grid_layout_->addWidget(lbl, r + 1, 0);

//  main_grid_layout_->addWidget(new QLabel(
//    QString::number(samp->index()) + samp->sub_index()),
//    r + 1, 1);

//  main_grid_layout_->addWidget(new QLabel(
//    QString::number(samp->chapter())),
//    r + 1, 2);

//  main_grid_layout_->addWidget(new QLabel(
//    QString::number(samp->page())),
//    r + 1, 3);

//  ++r;
// }

// QList<QTreeWidgetItem *> items;
// int i = 0;

// for(int i = 0; i < 6; ++i)
// {
//  QStringList qsl;
//  for(int j = 0; j < 3; ++j)
//  {
//   QString qs = QString("item: %1 %2").arg(i).arg(j);
//   qsl.push_back(qs);
//  }
//  QTreeWidgetItem* twi = new QTreeWidgetItem((QTreeWidget*) nullptr,
//   qsl);
//  main_tree_widget_->addTopLevelItem(twi);
// }
// main_tree_widget_->insertTopLevelItems(0, items);
// main_tree_widget_->insertTopLevelItems(1, items);

 //main_tree_layout_->addLayout(main_grid_layout_);

// main_tree_layout_->addWidget(main_tree_widget_);

// main_frame_->setLayout(main_tree_layout_);

// tree_scroll_area_ = new QScrollArea(this);
// tree_scroll_area_->setWidget(main_frame_);

// tree_scroll_area_->setMaximumHeight(200);
