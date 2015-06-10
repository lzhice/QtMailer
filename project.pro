TEMPLATE = subdirs
SUBDIRS = src examples

CONFIG += ordered
src.file        = src/QtMailer.pro
examples.file   = examples/examples.pro
examples.depends = src
