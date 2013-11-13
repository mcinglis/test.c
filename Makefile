
# -----
# Variables
# -----

testc = .
dep_ext = .dep.mk

include Module.mk

tests_src = $(wildcard tests/*.c)
tests_obj = $(tests_src:.c=.o)
tests_dep = $(tests_obj:.o=$(dep_ext))
tests_main = tests/main

examples_src = $(wildcard examples/*.c)
examples_obj = $(examples_src:.c=.o)
examples_dep = $(examples_obj:.o=$(dep_ext))
examples_bin = $(basename $(examples_src))


# -----
# Preprocessor and compiler flags
# -----

# For the tests and examples:
CPPFLAGS += -I.

CFLAGS += -std=c11 -g -Wall -Wextra -Wpedantic \
          -Wshadow -Wcast-qual -Wcast-align \
          -Wformat=2 -Wno-unused-parameter -Wwrite-strings \
          -Wstrict-prototypes -Wold-style-definition \
          -Wredundant-decls -Wmissing-include-dirs -Wswitch-default \
          -Wcast-align -Wnested-externs -Wno-missing-field-initializers

ifeq ($(CC),gcc)
    CFLAGS += -Og -fstack-protector-strong -Wjump-misses-init -Wlogical-op
endif
ifeq ($(CC),clang)
    CFLAGS += -O0
endif


# -----
# Targets
# -----

.PHONY: all
all: $(testc_obj) tests examples

.PHONY: tests
tests: $(tests_main)
$(tests_main): $(tests_obj) $(testc_obj)

.PHONY: test
test: $(tests_main)
	@./$(tests_main)

.PHONY: examples
examples: $(examples_bin)
$(examples_bin): $(testc_obj)

.PHONY: clean
clean:
	-rm -f $(testc_dep) $(testc_obj)
	-rm -f $(tests_dep) $(tests_obj) $(tests_main)
	-rm -f $(examples_dep) $(examples_obj) $(examples_bin)


# -----
# Automatic dependencies
# -----

# Have the compiler output dependency files with make targets for each
# of the object files. The `MT` option specifies the dependency file
# itself as a target, so that it's regenerated when it should be.
%$(dep_ext): %.c
	$(CC) $(CPPFLAGS) -MM -MP -MT '$(@:$(dep_ext)=.o) $@' $< > $@

# Include each of those dependency files; Make will run the rule above
# to generate each dependency file (if it needs to).
-include $(testc_dep) $(tests_dep) $(examples_dep)

