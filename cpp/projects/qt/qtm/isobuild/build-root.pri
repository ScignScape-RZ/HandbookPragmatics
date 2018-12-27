
BUILD_DIR_CODE = $$OUT_PWD
BUILD_DIR_CODE ~= s!.*/(build|release)-($$PROJECT_NAME)-(.*)-(Debug|Release)!\3

CHOICE_CODE = isobuild-choices

WHICH_BUILD_DIR_CODE = isobuild

include(../choices/$${WHICH_BUILD_DIR_CODE}_choices.pri)

include(../build-root-both.pri)
