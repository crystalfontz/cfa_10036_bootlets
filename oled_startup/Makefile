CURRENTDIR	= .
SRCDIR		= $(CURRENTDIR)
OUTPUTDIR	= $(CURRENTDIR)/output-target
COMMONSRCDIR	= $(CURRENTDIR)/../mach-$(ARCH)

INCLUDEDIRS	= -I$(COMMONSRCDIR) -I$(COMMONSRCDIR)/includes

# Linker script
BASE_ADDR	?= 0x00000000
BOOT_LAYOUT	= $(SRCDIR)/link.lds

# Output ELF image
CFA_STARTUP	= $(OUTPUTDIR)/cfa_startup

CROSS_COMPILE ?= arm-linux-gnueabi-

AS	= $(CROSS_COMPILE)as
CC	= $(CROSS_COMPILE)gcc
LD	= $(CROSS_COMPILE)ld
CPP	= $(CROSS_COMPILE)cpp
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump

MEM_TYPE ?= MEM_DDR1

POWERFLAG = ?
ifeq ($(POWERFLAG),MXS_VBUS_CURRENT_DRAW)
CFLAGS += -DMXS_VBUS_CURRENT_DRAW
endif

CFLAGS 	+= -Wall $(INCLUDEDIRS) -I./ -O -DSTMP378x -D$(ARCH) -ffreestanding
LDFLAGS = -static -gc-sections -nostdlib -T $(BOOT_LAYOUT)



hw_libs += $(COMMONSRCDIR)/hw/power/libhw_power.a
hw_libs += $(COMMONSRCDIR)/hw/lradc/libhw_lradc.a
hw_libs += $(COMMONSRCDIR)/hw/icoll/libhw_icoll.a
hw_libs += $(COMMONSRCDIR)/hw/digctl/libhw_digctl.a
hw_libs += $(COMMONSRCDIR)/hw/core/libhw_core.a
ddi_libs += $(COMMONSRCDIR)/drivers/power/libddi_power.a


STARTUP_OBJS = oled_startup.o iomux.o mxs_gpio.o data.o CFA_init.o i2c.o oled.o
# Generic code
CORE_OBJS = debug.o utils.o

# Default goal
.PHONY: all
all: oled_startup

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
oled_startup: libs $(BOOT_LAYOUT) $(STARTUP_OBJS) $(CORE_OBJS)
	$(LD) -o $@ $(STARTUP_OBJS) $(ddi_libs) $(hw_libs) \
	$(LDFLAGS) $(CORE_OBJS)
	@nm -n $@ > oled_startup.map

libs:
	$(MAKE) -C $(COMMONSRCDIR)/hw
	$(MAKE) -C $(COMMONSRCDIR)/drivers

clean:
	@echo Cleaning...
	rm -f oled_startup.map
	rm -f oled_startup
	$(MAKE) clean -C $(COMMONSRCDIR)/hw
	$(MAKE) clean -C $(COMMONSRCDIR)/drivers
	@echo Files:
	rm -f $(STARTUP_OBJS) $(CORE_OBJS)
	@echo Build output:

