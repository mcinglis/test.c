
testc_src = $(wildcard $(testc)/*.c)
testc_obj = $(testc_src:.c=.o)
testc_dep = $(testc_obj:.o=$(dep_ext))

