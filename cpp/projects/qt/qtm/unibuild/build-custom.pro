
TEMPLATE = subdirs

#__CHOICE_SUBDIRS__#

SUBDIRS = \
  dataset/ro-info/ro-info \
  dataset/config/config-dialog \
  dataset/dsmain/dsmain \
  dataset/pdf-pull/pdf-pull \
  kdmi/ds-kdmi/ds-kdmi \
  kdmi/ds-kdmi/ds-kdmi-console \
  kdmi/kdmi-bridge/kdmi-bridge \
  kdmi/kdmi-bridge/kdmi-bridge-console \
  dataset/pdf-pull/pdf-pull-console \
  dataset/ro-info/ro-info-console \
  external/xpdf/xpdf \
  qscign/ScignStage/ScignStage-ling \
  qscign/LexPair/lexpair \
  dataset/application-model/application-model \
  dataset/dsmain/_run__dsmain-console \


#__END_INSERT__#


# e.g.
#SUBDIRS = \
#  dataset/dsmain/dsmain \
#  dataset/application-model/application-model \
#  external/xpdf/xpdf \
#  dataset/dsmain/_run__dsmain-console \

CONFIG += ordered
