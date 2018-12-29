
TEMPLATE = subdirs


SUBDIRS = \
  dataset/dsmain/dsmain \

SUBDIRS += \
  dataset/pdf-pull/pdf-pull \

SUBDIRS += \
  external/xpdf/xpdf \

SUBDIRS += \
  qscign/ScignStage/ScignStage-ling \

SUBDIRS += \
  dataset/application-model/application-model \
  dataset/config/config-dialog \

SUBDIRS += \
  qscign/LexPair/lexpair \
  dataset/dsmain/_run__dsmain-console \


CONFIG += ordered
