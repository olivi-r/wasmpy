# get assemblies
dirs=_internal control parametric variable memory numeric tail-call
opcodes_paths=wasmpy/arch/x86/lib/opcodes.cpp wasmpy/arch/x86_64/lib/opcodes.cpp
x86_binaries=$(foreach i,$(dirs),$(foreach j,$(wildcard wasmpy/arch/x86/$(i)/*.s),$(patsubst %.s,%,$(j))))
x86_64_binaries=$(foreach i,$(dirs),$(foreach j,$(wildcard wasmpy/arch/x86_64/$(i)/*.s),$(patsubst %.s,%,$(j))))
binaries=$(x86_binaries) $(x86_64_binaries)

# set defaults
ifeq ($(origin LDFLAGS),undefined)
LDFLAGS=--oformat binary -e 0
endif

ifeq ($(origin X86_ASFLAGS),undefined)
X86_ASFLAGS=--32 $(ASFLAGS)
endif

ifeq ($(origin X86_LDFLAGS),undefined)
X86_LDFLAGS=-melf_i386 $(LDFLAGS)
endif

ifeq ($(origin X86_64_ASFLAGS),undefined)
X86_64_ASFLAGS=$(ASFLAGS)
endif

ifeq ($(origin X86_64_LDFLAGS),undefined)
X86_64_LDFLAGS=$(LDFLAGS)
endif

.PHONY: all clean x86 x86_64

# main commands
all: $(binaries)

clean:
	rm -f $(binaries) $(foreach i,$(binaries),$(patsubst %,%.o,$(i))) $(opcodes_paths)

x86: $(x86_binaries)

x86_64: $(x86_64_binaries)

# generate rules for assembly sources
define X86_BIN_FROM_SRC
$(1): $(patsubst %,%.s,$(1))
	$(AS) $(X86_ASFLAGS) -o $(patsubst %,%.o,$(1)) $(patsubst %,%.s,$(1))
	$(LD) $(X86_LDFLAGS) -o $(1) $(patsubst %,%.o,$(1))

endef

define X86_64_BIN_FROM_SRC
$(1): $(patsubst %,%.s,$(1))
	$(AS) $(X86_64_ASFLAGS) -o $(patsubst %,%.o,$(1)) $(patsubst %,%.s,$(1))
	$(LD) $(X86_64_LDFLAGS) -o $(1) $(patsubst %,%.o,$(1))

endef

$(eval $(foreach i,$(x86_binaries),$(call X86_BIN_FROM_SRC,$(i))))
$(eval $(foreach i,$(x86_64_binaries),$(call X86_64_BIN_FROM_SRC,$(i))))
