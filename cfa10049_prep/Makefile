CURRENTDIR	= .
SRCDIR		= $(CURRENTDIR)
OUTPUTDIR	= $(CURRENTDIR)/output-target
COMMONSRCDIR	= $(CURRENTDIR)/../mach-$(ARCH)

INCLUDEDIRS	= -I$(COMMONSRCDIR) -I$(COMMONSRCDIR)/includes

# Linker script
BASE_ADDR	?= 0x00000000
BOOT_LAYOUT	= $(SRCDIR)/link.lds

# Output ELF image
CFA10049_PREP	= $(OUTPUTDIR)/cfa10049_prep

CROSS_COMPILE ?= arm-linux-gnueabi-

AS	= $(CROSS_COMPILE)as
CC	= $(CROSS_COMPILE)gcc
LD	= $(CROSS_COMPILE)ld
CPP	= $(CROSS_COMPILE)cpp
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump

MEM_TYPE ?= MEM_DDR1

CONFIGFLAG = ?
ifeq ($(CONFIGFLAG),MXS_VBUS_CURRENT_DRAW)
CFLAGS += -DMXS_VBUS_CURRENT_DRAW
endif

CFLAGS 	+= -Wall $(INCLUDEDIRS) -I./ -O -DSTMP378x -D$(ARCH)
LDFLAGS = -static -gc-sections -nostdlib -T $(BOOT_LAYOUT)


#TOP_LEVEL_OBJS = cfa10049_prep.o

#CFA10049_PREP_OBJS += $(COMMONSRCDIR)/hw/libhw.a
#CFA10049_PREP_OBJS += $(COMMONSRCDIR)/drivers/libddi.a
hw_libs += $(COMMONSRCDIR)/hw/power/libhw_power.a
hw_libs += $(COMMONSRCDIR)/hw/lradc/libhw_lradc.a
hw_libs += $(COMMONSRCDIR)/hw/icoll/libhw_icoll.a
hw_libs += $(COMMONSRCDIR)/hw/digctl/libhw_digctl.a
hw_libs += $(COMMONSRCDIR)/hw/core/libhw_core.a
ddi_libs += $(COMMONSRCDIR)/drivers/power/libddi_power.a


CFA10049_PREP_OBJS = cfa10049_prep.o eabi.o

# Generic code
CORE_OBJS = debug.o

# Default goal
.PHONY: all
all: cfa10049_prep

%.o: %.S
	$(CC) -c $(CFLAGS) -D__ASSEMBLY__ -o $@ $<

%.o: %.arm
	$(CC) -c $(CFLAGS) -D__ASSEMBLY__ -o $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<



#
# Make targets
#
.PHONY: build build_prep clean
cfa10049_prep: libs $(BOOT_LAYOUT) $(CFA10049_PREP_OBJS) $(CORE_OBJS)
	$(LD) -o $@ $(CFA10049_PREP_OBJS) $(ddi_libs) $(hw_libs) \
	$(LDFLAGS) $(CORE_OBJS)
	@nm -n $@ > cfa10049_prep.map

libs:
	$(MAKE) -C $(COMMONSRCDIR)/hw
	$(MAKE) -C $(COMMONSRCDIR)/drivers

clean:
	@echo Cleaning...
	rm -f cfa10049_prep.map
	rm -f cfa10049_prep
	$(MAKE) clean -C $(COMMONSRCDIR)/hw
	$(MAKE) clean -C $(COMMONSRCDIR)/drivers
	@echo Files:
	rm -f $(CFA10049_PREP_OBJS)
	@echo Build output:

