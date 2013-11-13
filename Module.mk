# Module.mk

# To use Test.c in your project, define the following variables:
# - `testc`: the path to the Test.c directory
# - `dep_ext`: the file extension you're using for the
#   automatically-generated Make target files (via the `-M` preprocessor
#   option); you can ignore this if you don't want it.
# and then include this file.

testc_src = $(wildcard $(testc)/*.c)
testc_obj = $(testc_src:.c=.o)
testc_dep = $(testc_obj:.o=$(dep_ext))

CPPFLAGS += -I$(testc)/submodules

