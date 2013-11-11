
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

CPPFLAGS += -Wall -I. -I./packages


objects = common.o assertion.o assertion-private.o test.o

tests_src = $(wildcard tests/*.c)
tests_obj = $(tests_src:.c=.o)
tests_bin = tests/main

examples_src = $(wildcard examples/*.c)
examples_bin = $(basename $(examples_src))

deprules = $(objects:.o=.dep.mk) \
	   $(tests_src:.c=.dep.mk) \
	   $(examples_src:.c=.dep.mk)


.PHONY: all
all: .submodules.make $(objects) examples tests

# Make submodules only on the first `make all`.
.submodules.make:
	git submodule init
	git submodule update
	@touch $@


# `tests` builds the tests, and `test` runs the tests.
.PHONY: test tests
test: tests
	./tests/main
tests: $(tests_bin)
$(tests_bin): $(tests_obj) $(objects)


.PHONY: examples
examples: $(examples_bin)
$(examples_bin): $(objects)


# Have the compiler output dependency files with make targets for each
# of the object files. The `MT` option specifies the dependency file
# itself as a target, so that it's regenerated when it should be.
%.dep.mk: %.c
	$(CC) -MM -MP -MT '$(<:.c=.o) $@' $(CPPFLAGS) $< > $@

# Include each of those dependency files; Make will run the rule above
# to generate each dependency file (if it needs to).
-include $(deprules)


.PHONY: clean clean-dep clean-obj clean-bin
clean: clean-dep clean-obj clean-bin
clean-dep:
	-rm -f $(deprules)
clean-obj:
	-rm -f $(deprules:.dep.mk=.o)
clean-bin:
	-rm -f $(tests_bin) $(examples_bin)

