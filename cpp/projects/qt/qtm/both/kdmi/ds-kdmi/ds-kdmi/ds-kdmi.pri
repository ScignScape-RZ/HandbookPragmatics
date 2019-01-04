
#           Copyright Nathaniel Christen 2019.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

include(../build-group.pri)

INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR

INCLUDEPATH += $$DSMAIN_SRC_GROUP_DIR

DEFINES += CAON_DEBUG
DEFINES += RELAE_LABEL_NODES




CONFIG += no_keywords

DEFINES += USE_KANS
DEFINES += USE_RZNS



HEADERS += \
  $$SRC_DIR/ds-kdmi-model.h \
  $$SRC_DIR/ds-kdmi-model-qobject.h \


SOURCES += \
  $$SRC_DIR/ds-kdmi-model.cpp \
  $$SRC_DIR/ds-kdmi-model-qobject.cpp \


LIBS += -L$$TARGETSDIR  -ldsmain


message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
