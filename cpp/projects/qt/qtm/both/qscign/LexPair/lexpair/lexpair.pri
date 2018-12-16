
#           Copyright Nathaniel Christen 2018.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)


QT += widgets


INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR


DEFINES += CAON_DEBUG
DEFINES += RELAE_LABEL_NODES



CONFIG += no_keywords

DEFINES += USE_KANS
DEFINES += USE_RZNS


HEADERS += \
  $$SRC_DIR/lexpair-dialog.h \
  $$SRC_DIR/lexpair-sxpr.h \


SOURCES += \
  $$SRC_DIR/lexpair-dialog.cpp \
  $$SRC_DIR/lexpair-sxpr.cpp \




message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
