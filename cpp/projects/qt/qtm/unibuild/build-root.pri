
BUILD_DIR_CODE = $$OUT_PWD

BUILD_DIR_CODE ~= s!.*/(build|release)-build-(\w+)-.*!UNIBUILD-\2

CHOICE_CODE = $$BUILD_DIR_CODE-choices

WHICH_BUILD_DIR_CODE = $$BUILD_DIR_CODE

include(../_choices/$${WHICH_BUILD_DIR_CODE}_choices.pri)

include(../build-root-both.pri)
