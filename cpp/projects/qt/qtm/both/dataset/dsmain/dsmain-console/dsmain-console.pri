
#           Copyright Nathaniel Christen 2018.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

include(../build-group.pri)

QT += widgets

TEMPLATE = app

INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR

INCLUDEPATH += $$SCIGNSTAGE_SRC_GROUP_DIR
INCLUDEPATH += $$KAUVIR_KCM_SRC_GROUP_DIR
INCLUDEPATH += $$PHAON_SRC_GROUP_DIR
INCLUDEPATH += $$QHYP_SRC_GROUP_DIR
INCLUDEPATH += $$KCM_RUNTIME_EVAL_SRC_GROUP_DIR
INCLUDEPATH += $$KAUVIR_RUNTIME_SRC_GROUP_DIR

INCLUDEPATH += $$LEXPAIR_SRC_GROUP_DIR

DEFINES += CAON_DEBUG
DEFINES += RELAE_LABEL_NODES


DEFINES += DATA_FOLDER=\\\"$$DATA_ROOT_DIR\\\"
DEFINES += SCREENSHOTS_FOLDER=\\\"$$ROOT_DIR/screenshots\\\"


DEFINES += USE_KANS
DEFINES += USE_RZNS
DEFINES += USE_QSNS


HEADERS += \
  $$SRC_DIR/test-functions.h \


SOURCES += \
  $$SRC_DIR/main.cpp \
  $$SRC_DIR/test-functions.cpp \


LIBS += -L$$TARGETSDIR -ldsmain -lScignStage-ling \

LIBS += -L$$TARGETSDIR -llexpair

exists($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/external--xpdf--xpdf) \#/
{
 message(DEFINE\'ing USING_XPDF)
 DEFINES += USING_XPDF
}

exists($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/tests-kph--kauvir-phaon--kauvir-phaon) \#/
{
 message(DEFINE\'ing USING_KPH)
 DEFINES += USING_KPH
 LIBS += -L$$TARGETSDIR -lkcm-direct-eval -lkcm-scopes  -lkauvir-phaon -lPhaonLib -lkauvir-code-model
}

exists($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/kauvir--kauvir-kcm--kcm-lisp-bridge) \#/
{
 message(DEFINE\'ing USING_ECL)
 include(../../../../find-ecl-sexp.pri)
 LIBS += -L$$ECL_DIR
 message($$ECL_DIR)
}

message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
