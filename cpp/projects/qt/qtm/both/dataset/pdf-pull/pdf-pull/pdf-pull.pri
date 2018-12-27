
#           Copyright Nathaniel Christen 2019.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

include(../build-group.pri)

INCLUDEPATH += $$SRC_DIR

INCLUDEPATH += $$DSMAIN_SRC_GROUP_DIR

QT += widgets

DEFINES += CAON_DEBUG
DEFINES += RELAE_LABEL_NODES


CONFIG += no_keywords

DEFINES += USE_KANS
DEFINES += USE_RZNS

DEFINES += DEFAULT_TEMP_DATASET_FILE=\\\"$$ROOT_DIR/data/temp/data.txt\\\"

HEADERS += \
  $$SRC_DIR/pdf-pull.h \
  $$SRC_DIR/qconsole.h \
  $$SRC_DIR/qlispconsole.h \
  $$SRC_DIR/qlispconsole-dialog.h \

SOURCES += \
  $$SRC_DIR/pdf-pull.cpp \
  $$SRC_DIR/qlispconsole.cpp \
  $$SRC_DIR/qlispconsole-dialog.cpp \
  $$SRC_DIR/qconsole.cpp \


LIBS += -L$$TARGETSDIR -ldsmain

message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
