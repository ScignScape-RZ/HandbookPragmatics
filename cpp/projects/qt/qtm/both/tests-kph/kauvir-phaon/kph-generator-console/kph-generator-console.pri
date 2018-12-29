
#           Copyright Nathaniel Christen 2019.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

PROJECT_NAME = kph-generator-console

include(../build-group.pri)

TEMPLATE = app

INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR

INCLUDEPATH += $$PHAON_SRC_GROUP_DIR
INCLUDEPATH += $$QHYP_SRC_GROUP_DIR
INCLUDEPATH += $$KAUVIR_PHAON_SRC_GROUP_DIR
INCLUDEPATH += $$KAUVIR_KCM_SRC_GROUP_DIR
INCLUDEPATH += $$KCM_RUNTIME_EVAL_SRC_GROUP_DIR
INCLUDEPATH += $$KAUVIR_RUNTIME_SRC_GROUP_DIR


CONFIG += no_keywords


DEFINES += USE_KANS

DEFINES += DEFAULT_KPH_FOLDER=\\\"$$CPP_ROOT_DIR/kph\\\"

HEADERS += \
  $$SRC_DIR/test-functions.h \


SOURCES += \
  $$SRC_DIR/main.cpp \
  $$SRC_DIR/test-functions.cpp \

LIBS += -L$$TARGETSDIR -lkph-generator

LIBS += -L$$TARGETSDIR -lPhaonLib -lkauvir-code-model -lkauvir-type-system \
   -lkcm-command-package -lkcm-direct-eval -lkcm-scopes -lkauvir-phaon


contains(CHOICE_FEATURES, "kcm_ecl") \#/
{
 message(DEFINE\'ing USING_ECL)
 include(../../../../find-ecl-sexp.pri)
}

contains(CHOICE_FEATURES, "iso-choice") \#/
{
 exists($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/kauvir--kauvir-kcm--kcm-lisp-bridge) \#/
 {
  message(DEFINE\'ing USING_ECL)
  include(../../../../find-ecl-sexp.pri)
 }
}


message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
