################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/hal_entry.c 

C_DEPS += \
./src/hal_entry.d 

OBJS += \
./src/hal_entry.o 

SREC += \
testRA0bare.srec 

MAP += \
testRA0bare.map 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	$(file > $@.in,-mcpu=cortex-m23 -mthumb -Oz -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM23 -D_RA_ORDINAL=1 -I"/media/fabiop/SSD_1T/Projects/e2studio/testRA0bare/src" -I"." -I"/media/fabiop/SSD_1T/Projects/e2studio/testRA0bare/ra/fsp/inc" -I"/media/fabiop/SSD_1T/Projects/e2studio/testRA0bare/ra/fsp/inc/api" -I"/media/fabiop/SSD_1T/Projects/e2studio/testRA0bare/ra/fsp/inc/instances" -I"/media/fabiop/SSD_1T/Projects/e2studio/testRA0bare/ra_gen" -I"/media/fabiop/SSD_1T/Projects/e2studio/testRA0bare/ra_cfg/fsp_cfg/bsp" -I"/media/fabiop/SSD_1T/Projects/e2studio/testRA0bare/ra_cfg/fsp_cfg" -I"/media/fabiop/SSD_1T/Projects/e2studio/testRA0bare/ra/arm/CMSIS_6/CMSIS/Core/Include" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

