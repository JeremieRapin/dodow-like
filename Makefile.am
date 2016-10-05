bin_PROGRAMS = dodow-like.elf

AUTOMAKE_OPTIONS = subdir-objects

dodow_like_elf_SOURCES  = \
  src/main.c \
	src/button.c \
	src/dodow.c \
  src/error.c \
	src/BSP/bsp.c \
  src/Tools/save.c \
	src/Tools/pwmOutput.c \
  src/LM4F_startup.c \
  misc/ti/driverlib/adc.c \
  misc/ti/driverlib/can.c \
  misc/ti/driverlib/comp.c \
  misc/ti/driverlib/cpu.c \
  misc/ti/driverlib/eeprom.c \
  misc/ti/driverlib/epi.c \
  misc/ti/driverlib/ethernet.c \
  misc/ti/driverlib/fan.c \
  misc/ti/driverlib/flash.c \
  misc/ti/driverlib/fpu.c \
  misc/ti/driverlib/gpio.c \
  misc/ti/driverlib/i2c.c \
  misc/ti/driverlib/i2s.c \
  misc/ti/driverlib/interrupt.c \
  misc/ti/driverlib/lpc.c \
  misc/ti/driverlib/mpu.c \
  misc/ti/driverlib/peci.c \
  misc/ti/driverlib/pwm.c \
  misc/ti/driverlib/qei.c \
  misc/ti/driverlib/ssi.c \
  misc/ti/driverlib/sysctl.c \
  misc/ti/driverlib/sysexc.c \
  misc/ti/driverlib/systick.c \
  misc/ti/driverlib/timer.c \
  misc/ti/driverlib/uart.c \
  misc/ti/driverlib/udma.c \
  misc/ti/driverlib/usb.c \
  misc/ti/driverlib/watchdog.c \
  misc/FreeRTOS/Source/event_groups.c \
  misc/FreeRTOS/Source/portable/MemMang/heap_1.c \
  misc/FreeRTOS/Source/list.c \
  misc/FreeRTOS/Source/queue.c \
  misc/FreeRTOS/Source/tasks.c \
  misc/FreeRTOS/Source/timers.c \
	misc/FreeRTOS/Source/portable/GCC/tm4c123g/port.c \
	misc/FreeRTOS/Source/portable/GCC/tm4c123g/scb.c \
	misc/FreeRTOS/Source/portable/GCC/tm4c123g/systick.c

dodow_like_elf_CFLAGS = \
  -DUSE_FREERTOS \
  -DPART_LM4F123GH6PM \
  -DARM_MATH_CM4 \
  -DTARGET_IS_BLIZZARD_RA1 \
  -Dgcc \
  -mfpu=fpv4-sp-d16 \
  -mfloat-abi=softfp \
  --specs=rdimon.specs \
  -Wl,--gc-sections \
  -mcpu=cortex-m4 \
  -mthumb \
  -Og \
  -std=c99 \
  -pedantic \
  -fmessage-length=0 \
  -fsigned-char \
  -ffunction-sections \
  -fdata-sections \
  -ffreestanding \
  -Wall \
  -Wextra  \
  -MD \
  -g3 \
  -DDEBUG \
  \
  -I$(srcdir)/include \
	-I$(srcdir)/include/BSP \
	-I$(srcdir)/include/Config \
  -I$(srcdir)/include/Object \
  -I$(srcdir)/include/Tools \
  -I$(srcdir)/misc \
	-I$(srcdir)/misc/FreeRTOS \
	-I$(srcdir)/misc/FreeRTOS/Source \
	-I$(srcdir)/misc/FreeRTOS/Source/include \
	-I$(srcdir)/misc/FreeRTOS/Source/portable \
	-I$(srcdir)/misc/FreeRTOS/Source/portable/GCC \
	-I$(srcdir)/misc/FreeRTOS/Source/portable/GCC/tm4c123g \
	-I$(srcdir)/misc/FreeRTOS/Source/portable/MemMang \
  -I$(srcdir)/misc/ti \
  -I$(srcdir)/misc/ti/driverlib \
  -I$(srcdir)/misc/ti/inc \
  \
  -I$(srcdir)/src

dodow_like_elf_LDFLAGS = \
  -mcpu=cortex-m4 \
  -mfpu=fpv4-sp-d16 \
  -mfloat-abi=softfp \
  -mthumb \
  -Og \
  -Wl,-Map=$(bin_PROGRAMS).map \
  -fmessage-length=0 \
  -fsigned-char \
  -ffunction-sections \
  -fdata-sections \
  -ffreestanding \
  -Wall \
  -Wextra \
  -g3 \
  -nostartfiles \
  -Xlinker \
  --gc-sections \
  -L$(srcdir)/ldscripts \
  -T LM4F.ld

EXTRA_PROGRAMS = prog
all:
	$(host)-size --format=berkeley $(builddir)/$(bin_PROGRAMS)
	$(host)-objcopy -Obinary $(builddir)/$(bin_PROGRAMS) $(top_builddir)/$(bin_PROGRAMS).bin
.PHONY: all
