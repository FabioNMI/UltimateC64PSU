################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ra/fsp/src/bsp/mcu/all/bsp_clocks.c \
../src/ra/fsp/src/bsp/mcu/all/bsp_common.c \
../src/ra/fsp/src/bsp/mcu/all/bsp_delay.c \
../src/ra/fsp/src/bsp/mcu/all/bsp_group_irq.c \
../src/ra/fsp/src/bsp/mcu/all/bsp_guard.c \
../src/ra/fsp/src/bsp/mcu/all/bsp_io.c \
../src/ra/fsp/src/bsp/mcu/all/bsp_irq.c \
../src/ra/fsp/src/bsp/mcu/all/bsp_macl.c \
../src/ra/fsp/src/bsp/mcu/all/bsp_register_protection.c \
../src/ra/fsp/src/bsp/mcu/all/bsp_rom_registers.c \
../src/ra/fsp/src/bsp/mcu/all/bsp_sbrk.c \
../src/ra/fsp/src/bsp/mcu/all/bsp_sdram.c \
../src/ra/fsp/src/bsp/mcu/all/bsp_security.c 

C_DEPS += \
./src/ra/fsp/src/bsp/mcu/all/bsp_clocks.d \
./src/ra/fsp/src/bsp/mcu/all/bsp_common.d \
./src/ra/fsp/src/bsp/mcu/all/bsp_delay.d \
./src/ra/fsp/src/bsp/mcu/all/bsp_group_irq.d \
./src/ra/fsp/src/bsp/mcu/all/bsp_guard.d \
./src/ra/fsp/src/bsp/mcu/all/bsp_io.d \
./src/ra/fsp/src/bsp/mcu/all/bsp_irq.d \
./src/ra/fsp/src/bsp/mcu/all/bsp_macl.d \
./src/ra/fsp/src/bsp/mcu/all/bsp_register_protection.d \
./src/ra/fsp/src/bsp/mcu/all/bsp_rom_registers.d \
./src/ra/fsp/src/bsp/mcu/all/bsp_sbrk.d \
./src/ra/fsp/src/bsp/mcu/all/bsp_sdram.d \
./src/ra/fsp/src/bsp/mcu/all/bsp_security.d 

OBJS += \
./src/ra/fsp/src/bsp/mcu/all/bsp_clocks.o \
./src/ra/fsp/src/bsp/mcu/all/bsp_common.o \
./src/ra/fsp/src/bsp/mcu/all/bsp_delay.o \
./src/ra/fsp/src/bsp/mcu/all/bsp_group_irq.o \
./src/ra/fsp/src/bsp/mcu/all/bsp_guard.o \
./src/ra/fsp/src/bsp/mcu/all/bsp_io.o \
./src/ra/fsp/src/bsp/mcu/all/bsp_irq.o \
./src/ra/fsp/src/bsp/mcu/all/bsp_macl.o \
./src/ra/fsp/src/bsp/mcu/all/bsp_register_protection.o \
./src/ra/fsp/src/bsp/mcu/all/bsp_rom_registers.o \
./src/ra/fsp/src/bsp/mcu/all/bsp_sbrk.o \
./src/ra/fsp/src/bsp/mcu/all/bsp_sdram.o \
./src/ra/fsp/src/bsp/mcu/all/bsp_security.o 

SREC += \
testRA0bare.srec 

MAP += \
testRA0bare.map 


# Each subdirectory must supply rules for building sources it contributes
src/ra/fsp/src/bsp/mcu/all/%.o: ../src/ra/fsp/src/bsp/mcu/all/%.c
	$(file > $@.in,-mcpu=cortex-m23 -mthumb -Oz -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM23 -D_RA_ORDINAL=1 -I"/media/fabiop/SSD_1T/Projects/e2studio/testRA0bare/src" -I"." -I"/media/fabiop/SSD_1T/Projects/e2studio/testRA0bare/ra/fsp/inc" -I"/media/fabiop/SSD_1T/Projects/e2studio/testRA0bare/ra/fsp/inc/api" -I"/media/fabiop/SSD_1T/Projects/e2studio/testRA0bare/ra/fsp/inc/instances" -I"/media/fabiop/SSD_1T/Projects/e2studio/testRA0bare/ra_gen" -I"/media/fabiop/SSD_1T/Projects/e2studio/testRA0bare/ra_cfg/fsp_cfg/bsp" -I"/media/fabiop/SSD_1T/Projects/e2studio/testRA0bare/ra_cfg/fsp_cfg" -I"/media/fabiop/SSD_1T/Projects/e2studio/testRA0bare/ra/arm/CMSIS_6/CMSIS/Core/Include" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

