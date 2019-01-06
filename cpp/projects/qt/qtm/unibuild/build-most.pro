
TEMPLATE = subdirs


SUBDIRS = \
  dataset/dsmain/dsmain \
  dataset/dsmain/data-model-console \
  dataset/ro-info/ro-info \
  dataset/ro-info/ro-info-console \
  dataset/config/config-dialog \


SUBDIRS += \
  kdmi/ds-kdmi/ds-kdmi \
  kdmi/ds-kdmi/ds-kdmi-console \
  kdmi/kdmi-bridge/kdmi-bridge \
  kdmi/kdmi-bridge/kdmi-bridge-console \


SUBDIRS += \
  dataset/pdf-pull/pdf-pull \
  dataset/pdf-pull/pdf-pull-console \


SUBDIRS += \
  external/xpdf/xpdf \
  external/xpdf/xpdf-console \
  kauvir/kauvir-kcm/kauvir-type-system \
  kauvir/kcm-runtime-eval/kcm-scopes \
  kauvir/kauvir-kcm/kauvir-code-model \
  kauvir/kauvir-runtime/kcm-command-package \
  phaon/phaon/PhaonLib \
  kauvir/kauvir-runtime/kcm-command-runtime \
  kauvir/kcm-runtime-eval/kcm-direct-eval \
  tests-kph/kauvir-phaon/kauvir-phaon \


SUBDIRS += \
  qscign/ScignStage/ScignStage-ling \
  qscign/LexPair/lexpair \
  qscign/LexPair/lexpair-console \


SUBDIRS += \
  dataset/application-model/application-model \
  dataset/application-model/application-model-test-dialog \
  dataset/application-model/amtd-console \
  dataset/config/config-dialog-console \


SUBDIRS += \
  phaon/phaon/phaon-console \
  tests-kph/kauvir-phaon/kauvir-phaon-console \


SUBDIRS += \
  dataset/dsmain/_run__dsmain-console \


SUBDIRS += \
  tests-kph/kauvir-phaon/kph-tcp-console \
  phaon/phaon-xml/phaon-xml \
  phaon/phaon-xml/phaon-xml-console \


CONFIG += ordered
