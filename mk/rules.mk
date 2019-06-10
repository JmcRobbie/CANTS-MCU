
########################################################################
### Do not change anything below unless you know what you are doing! ###
########################################################################

# Toolchain paths
CC := $(COMPILER_PATH)picosky-gcc
OBJCOPY := $(COMPILER_PATH)picosky-objcopy
OBJDUMP := $(COMPILER_PATH)picosky-objdump
SIZE := $(COMPILER_PATH)picosky-size
ifeq ($(OS),Windows_NT)
	# use toolchain provided programs
	RM := $(COMPILER_PATH)picosky-rm
	MKDIR := $(COMPILER_PATH)picosky-mkdir
else
	# use native programs on Linux
	RM := rm
	MKDIR := mkdir
endif

include mk/sections.mk

# List source files and include folders
SRCC := $(wildcard $(addsuffix *.c,$(SRCDIRS)))
SRCS := $(wildcard $(addsuffix *.S,$(SRCDIRS)))
INC := $(addprefix -I,$(INCDIRS))

# Change ".c" or ".S" file extensions to ".o"
OBJ := $(SRCC:%.c=%.o) $(SRCS:%.S=%.o) $(patsubst %.c,%.o,$(FILES:%.S=%.o))

CFLAGS := -mcore=$(CORETYPE) -std=c99 -Wall -Wextra -Wpedantic -Wshadow \
		  -ffunction-sections -fdata-sections $(EXTRA_C_FLAGS) \
		  -D USER_TEXT_ORIGIN=$(USER_TEXT_ORIGIN) \
		  -D USER_TEXT_LENGTH=$(USER_TEXT_LENGTH) \
		  -D SUPERVISOR_TEXT_ORIGIN=$(SUPERVISOR_TEXT_ORIGIN) \
		  -D SUPERVISOR_TEXT_LENGTH=$(SUPERVISOR_TEXT_LENGTH) \
		  -D DATA_ORIGIN=$(DATA_ORIGIN) \
		  -D DATA_LENGTH=$(DATA_LENGTH) \
		  -D EXT_DATA_ORIGIN=$(DATA_EXT_ORIGIN) \
		  -D EXT_DATA_LENGTH=$(DATA_EXT_LENGTH)
		  
LDFLAGS := -mcore=$(CORETYPE) -Wl,--gc-sections $(EXTRA_LD_FLAGS) \
		-Wl,--defsym,__USER_TEXT_ORIGIN__=$(USER_TEXT_ORIGIN) \
		-Wl,--defsym,__USER_TEXT_LENGTH__=$(USER_TEXT_LENGTH) \
		-Wl,--defsym,__SUPERVISOR_TEXT_ORIGIN__=$(SUPERVISOR_TEXT_ORIGIN) \
		-Wl,--defsym,__SUPERVISOR_TEXT_LENGTH__=$(SUPERVISOR_TEXT_LENGTH) \
		-Wl,--defsym,__DATA_ORIGIN__=$(DATA_ORIGIN) \
		-Wl,--defsym,__DATA_LENGTH__=$(DATA_LENGTH) \
		-Wl,--defsym,__EXT_DATA_ORIGIN__=$(DATA_EXT_ORIGIN) \
		-Wl,--defsym,__EXT_DATA_LENGTH__=$(DATA_EXT_LENGTH)

ifeq ($(TARGET),Debug)
	CFLAGS += -Og -ggdb3 -DDEBUG
else
	CFLAGS += -Os -flto
endif

OBJDIR := $(OUTPUT_PATH)/$(TARGET)/obj
DEPDIR := $(OUTPUT_PATH)/$(TARGET)/dep
EXEDIR := $(OUTPUT_PATH)/$(TARGET)
_OBJ := $(addprefix $(OBJDIR)/,$(OBJ))
.PRECIOUS: $(_OBJ)

# disable built-in rules (speed optimization)
MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

# This must be first target in case if make is run without specified target
all: chktarget $(EXEDIR)/$(PROJECT).elf postbin $(EXEDIR)/$(PROJECT).asm
	@$(SIZE) -A $(EXEDIR)/$(PROJECT).elf
	@$(OBJDUMP) -P mem-usage $(EXEDIR)/$(PROJECT).elf

clean: chktarget
	@$(RM) -rf $(OUTPUT_PATH)/$(TARGET)
	
rebuild: clean all
	
chktarget:
ifneq ($(TARGET),Debug)
ifneq ($(TARGET),Release)
	$(error "Only Release and Debug targets are known!")
endif
endif

# include dependencies
-include $(addprefix $(DEPDIR)/,${OBJ:.o=.d})

$(OBJDIR)/%.o: %.c Makefile
	@echo [CC] $<
	@$(MKDIR) -p $(@D)
	@$(MKDIR) -p $(dir $(DEPDIR)/$*.d)
	@$(CC) -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d $(CFLAGS) $(INC) -c -o $@ $<

$(OBJDIR)/%.o: %.S Makefile
	@echo [AS] $<
	@$(MKDIR) -p $(@D)
	@$(MKDIR) -p $(dir $(DEPDIR)/$*.d)
	@$(CC) -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d $(CFLAGS) $(INC) -c -o $@ $<

$(EXEDIR)/%.elf: $(_OBJ)
	@echo [LD] $@
	@$(CC) $(LDFLAGS) -Wl,-Map=$(EXEDIR)/$(PROJECT).map,--cref -o $@ $(_OBJ)

%.bin: %.elf
	@echo [OBJCOPY] $@
	@$(OBJCOPY) --gap-fill 0 -O binary $< $@

%.asm: %.elf
	@echo [OBJDUMP] $@
	@$(OBJDUMP) -S --disassemble $< > $@
	
postbin: $(EXEDIR)/$(PROJECT).bin
	@echo [Post BIN steps]
	$(call POST_BIN_steps,$(EXEDIR)/$(PROJECT))

.PHONY: all clean rebuild chktarget postbin
