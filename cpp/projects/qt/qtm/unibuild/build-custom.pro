
TEMPLATE = subdirs

#__CHOICE_SUBDIRS__#

SUBDIRS = \
  dataset/ro-info/ro-info \
  dataset/config/config-dialog \
  dataset/dsmain/dsmain \
  dataset/pdf-pull/pdf-pull \
  dataset/pdf-pull/pdf-pull-console \
  dataset/ro-info/ro-info-console \
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
  phaon/phaon/phaon-console \
  tests-kph/kauvir-phaon/kauvir-phaon-console \
  tests-kph/kauvir-phaon/kph-tcp-console \
  phaon/phaon-xml/phaon-xml \
  phaon/phaon-xml/phaon-xml-console \
  qscign/ScignStage/ScignStage-ling \
  qscign/LexPair/lexpair \
  dataset/application-model/application-model \
  dataset/application-model/application-model-test-dialog \
  dataset/application-model/amtd-console \
  dataset/dsmain/_run__dsmain-console \


#__END_INSERT__#


# e.g.
#SUBDIRS = \
#  dataset/dsmain/dsmain \
#  dataset/application-model/application-model \
#  external/xpdf/xpdf \
#  dataset/dsmain/_run__dsmain-console \

CONFIG += ordered
