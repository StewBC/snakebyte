###############################################################################
### Generic Makefile for cc65 projects - full version with abstract options ###
### V1.1.1 (w) 2010 - 2011 Oliver Schmidt & Patryk "Silver Dream !" Łogiewa ###
###############################################################################

###############################################################################
### In order to override defaults - values can be assigned to the variables ###
###############################################################################

# Name of the final, single-file executable.
# Default: name of the current dir with target name appended
PROGRAM :=

# Space or comma separated list of cc65 supported targets to build for.
# Default: c64 (lowercase!)
TARGETS :=

# Space or comma separated list of SKUs to build for.
# Default: Same as TARGETS
SKUS :=

# Path to the directory containing C and ASM sources.
# Default: src
SRCDIR :=

# Path to the directory where object files are to be stored (inside respective target subdirectories).
# Default: obj
OBJDIR :=

# Additional C compiler flags and options.
# Default: none
CFLAGS  =

# Additional assembler flags and options.
# Default: none
ASFLAGS =

# Additional linker flags and options.
# Default: none
LDFLAGS =

# Command used to run the emulator.
# Default: depending on target platform. For default (c64) target: x64 -kernal kernal -VICIIdsize -autoload
EMUCMD := open $(PROGRAM)

# Optional commands used before starting the emulation process, and after finishing it.
# Default: none
#PREEMUCMD := osascript -e "tell application \"System Events\" to set isRunning to (name of processes) contains \"X11.bin\"" -e "if isRunning is true then tell application \"X11\" to activate"
#PREEMUCMD := osascript -e "tell application \"X11\" to activate"
#POSTEMUCMD := osascript -e "tell application \"System Events\" to tell process \"X11\" to set visible to false"
PREEMUCMD :=
POSTEMUCMD :=

# On Windows machines VICE emulators may not be available in the PATH by default.
# In such case, please set the variable below to point to directory containing
# VICE emulators. 
#VICE_HOME := "C:\Program Files\WinVICE-2.2-x86\"
VICE_HOME :=

# Options state file name. You should not need to change this, but for those
# rare cases when you feel you really need to name it differently - here you are
STATEFILE := Makefile.options

####  DO NOT EDIT BELOW THIS LINE, UNLESS YOU REALLY KNOW WHAT YOU ARE DOING!  ####

###################################################################################
### Mapping abstract options to the actual compiler, assembler and linker flags ###
### Predefined compiler, assembler and linker flags, used with abstract options ###
### valid for 2.14.x. Consult the documentation of your cc65 version before use ###
###################################################################################

# Compiler flags used to tell the compiler to optimise for SPEED
define _optspeed_
  CFLAGS += -Oris
endef

# Compiler flags used to tell the compiler to optimise for SIZE
define _optsize_
  CFLAGS += -Or
endef

# Compiler and assembler flags for generating listings
define _listing_
  CFLAGS += --listing $$(@:.o=.lst)
  ASFLAGS += --listing $$(@:.o=.lst)
  REMOVES += $(addsuffix .lst,$(basename $(OBJECTS)))
endef

# Linker flags for generating map file
define _mapfile_
  LDFLAGS += --mapfile $$@.map
  REMOVES += $(PROGRAM).map
endef

# Linker flags for generating VICE label file
define _labelfile_
  LDFLAGS += -Ln $$@.lbl
  REMOVES += $(PROGRAM).lbl
endef

###############################################################################
###  Defaults to be used if nothing defined in the editable sections above  ###
###############################################################################

# Presume the C64 target like the cl65 compile & link utility does.
# Set TARGETS to override.
ifeq ($(TARGETS),)
  TARGETS := c64
endif

# Presume the SKUs are the same as the targets.
# Set SKUS to override.
ifeq ($(SKUS),)
  SKUS := $(TARGETS)
endif

# Presume we're in a project directory so name the program like the current
# directory. Set PROGRAM to override.
ifeq ($(PROGRAM),)
  PROGRAM := $(notdir $(CURDIR))
endif

# Presume the C and asm source files to be located in the subdirectory 'src'.
# Set SRCDIR to override.
ifeq ($(SRCDIR),)
  SRCDIR := src
endif

# Presume the object and dependency files to be located in the subdirectory
# 'obj' (which will be created). Set OBJDIR to override.
ifeq ($(OBJDIR),)
  OBJDIR := obj
endif
TARGETOBJDIR := $(OBJDIR)/$(TARGETS)
TARGETSKUDIR := $(TARGETOBJDIR)/$(SKUS)

# On Windows it is mandatory to have CC65_HOME set. So do not unnecessarily
# rely on cl65 being added to the PATH in this scenario.
ifdef CC65_HOME
  CC := $(CC65_HOME)/bin/cl65
else
  CC := cl65
endif

# Default emulator commands and options for particular targets.
# Set EMUCMD to override.
c64_EMUCMD := $(VICE_HOME)x64 -kernal kernal -VICIIdsize -autoload
c128_EMUCMD := $(VICE_HOME)x128 -kernal kernal -VICIIdsize -autoload
vic20_EMUCMD := $(VICE_HOME)xvic -kernal kernal -VICdsize -autoload
pet_EMUCMD := $(VICE_HOME)xpet -Crtcdsize -autoload
plus4_EMUCMD := $(VICE_HOME)xplus4 -TEDdsize -autoload
# So far there is no x16 emulator in VICE (why??) so we have to use xplus4 with -memsize option
c16_EMUCMD := $(VICE_HOME)xplus4 -ramsize 16 -TEDdsize -autoload
cbm510_EMUCMD := $(VICE_HOME)xcbm2 -model 510 -VICIIdsize -autoload
cbm610_EMUCMD := $(VICE_HOME)xcbm2 -model 610 -Crtcdsize -autoload
atari_EMUCMD := atari800 -windowed -xl -pal -nopatchall -run

ifeq ($(EMUCMD),)
  EMUCMD := $($(TARGETS)_EMUCMD)
endif

###############################################################################
### The magic begins                                                        ###
###############################################################################

COMMA := ,
SPACE := $(N/A) $(N/A)
define NEWLINE


endef
# Note: Do not remove any of the two empty lines above !

TARGETLIST := $(subst $(COMMA),$(SPACE),$(TARGETS))
SKULIST := $(subst $(COMMA),$(SPACE),$(SKUS))

ifeq ($(words $(TARGETLIST)),1)
ifeq ($(words $(SKULIST)),1)

# Set PROGRAM to something like 'myprog-c64.c64'.
override PROGRAM := $(PROGRAM)-$(SKULIST).$(TARGETLIST)

# Set SOURCES to something like 'src/foo.c src/bar.s'.
SOURCES := $(wildcard $(SRCDIR)/*.c)
SOURCES += $(wildcard $(SRCDIR)/*.s)
SOURCES += $(wildcard $(SRCDIR)/$(SKULIST)/*.c)
SOURCES += $(wildcard $(SRCDIR)/$(SKULIST)/*.s)

# Set the CONFIG to something like 'src/c64/myprog.cfg'
CFGFILE := $(wildcard $(SRCDIR)/$(SKULIST)/*.cfg)
ifneq ($(strip $(CFGFILE)),)
CFGFILE := -C $(CFGFILE)
endif # ($(strip $(CFGFILE)),)

# Set OBJECTS to something like 'obj/target/foo.o obj/target/bar.o'.
OBJECTS := $(addsuffix .o,$(basename $(SOURCES:$(SRCDIR)%=$(TARGETOBJDIR)%)))

# Set DEPENDS to something like 'obj/target/foo.d obj/target/bar.d'.
DEPENDS := $(OBJECTS:.o=.d)

.SUFFIXES:
.PHONY: all test clean zap love

all: $(PROGRAM)

-include $(DEPENDS)
-include $(STATEFILE)

# If OPTIONS are given on the command line then save them to STATEFILE
# if (and only if) they have actually changed. But if OPTIONS are not
# given on the command line then load them from STATEFILE. Have object
# files depend on STATEFILE only if it actually exists.
ifeq ($(origin OPTIONS),command line)
  ifneq ($(OPTIONS),$(_OPTIONS_))
    ifeq ($(OPTIONS),)
      $(info Removing OPTIONS)
      $(shell $(RM) $(STATEFILE))
      $(eval $(STATEFILE):)
    else
      $(info Saving OPTIONS=$(OPTIONS))
      $(shell echo "_OPTIONS_=$(OPTIONS)" > $(STATEFILE))
    endif
    $(eval $(OBJECTS): $(STATEFILE))
  endif
else
  ifeq ($(origin _OPTIONS_),file)
    $(info Using saved OPTIONS=$(_OPTIONS_))
    OPTIONS = $(_OPTIONS_)
    $(eval $(OBJECTS): $(STATEFILE))
  endif
endif

# Transform the abstract OPTIONS to the actual cc65 options.
$(foreach o,$(subst $(COMMA),$(SPACE),$(OPTIONS)),$(eval $(_$o_)))

# The remaining targets.
$(TARGETOBJDIR):
	mkdir -p $@

$(TARGETSKUDIR):
	mkdir -p $@

$(TARGETOBJDIR)/%.o: $(SRCDIR)/%.c | $(TARGETOBJDIR) $(TARGETSKUDIR)
	$(CC) -t $(TARGETS) -c --create-dep $(@:.o=.d) $(CFLAGS) -o $@ $<

$(TARGETOBJDIR)/%.o: $(SRCDIR)/%.s | $(TARGETOBJDIR) $(TARGETSKUDIR)
	$(CC) -t $(TARGETS) -c --create-dep $(@:.o=.d) $(ASFLAGS) -o $@ $<

$(PROGRAM): $(OBJECTS)
	$(CC) -t $(TARGETS) $(LDFLAGS) -o $@ $^ $(CFGFILE)

test: $(PROGRAM)
	$(PREEMUCMD)
	$(EMUCMD) $<
	$(POSTEMUCMD)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(DEPENDS)
	$(RM) $(REMOVES)
	$(RM) $(PROGRAM)

else # $(words $(SKULIST)),1

all test clean:
	$(foreach t,$(SKULIST),$(MAKE) SKUS=$t $@$(NEWLINE))

endif # $(words $(SKULIST)),1

else # $(words $(TARGETLIST)),1

all test clean:
	$(foreach t,$(TARGETLIST),$(MAKE) TARGETS=$t $@$(NEWLINE))

endif # $(words $(TARGETLIST)),1

OBJDIRLIST := $(wildcard $(OBJDIR)/*)

zap:
	$(foreach o,$(OBJDIRLIST),-$(RM) $o/*.o $o/*.d $o/*.lst$(NEWLINE))
	$(foreach o,$(OBJDIRLIST),-rmdir $o$(NEWLINE))
	-rmdir $(OBJDIR)
	-$(RM) $(basename $(PROGRAM)).* $(STATEFILE)

love:
	@echo "Not war, eh?"