
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

CPPFLAGS += -I. -I./packages


objects = test.o
examples_src = $(wildcard examples/*.c)
examples_bin = $(basename $(examples_src))
deprules = $(objects:.o=.d) $(examples_src:.c=.d)


all: submodules $(objects) $(examples_bin)
.PHONY: all


submodules: .submodule.make
.PHONY: submodules
.submodule.make: .git/HEAD
	git submodule init
	git submodule update
	touch .submodule.make


$(examples_bin): $(objects)


# Have the compiler output dependency files with make targets for each
# of the object files. The `MT` option specifies the dependency file
# itself as a target, so that it's regenerated when it should be.
%.d: %.c
	$(CC) -MM -MP -MT '$(@:.d=.o) $@' $(CPPFLAGS) $< > $@

# Include each of those dependency files; Make will run the rule above
# to generate each dependency file (if it needs to).
-include $(deprules)


clean:
	-rm -f $(deprules)
	-rm -f $(objects)
	-rm -f $(examples_bin)
.PHONY: clean

