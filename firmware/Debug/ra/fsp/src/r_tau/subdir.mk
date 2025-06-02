################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/r_tau/r_tau.c 

C_DEPS += \
./ra/fsp/src/r_tau/r_tau.d 

OBJS += \
./ra/fsp/src/r_tau/r_tau.o 

SREC += \
C64PSU.srec 

MAP += \
C64PSU.map 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/r_tau/%.o: ../ra/fsp/src/r_tau/%.c
	$(file > $@.in,-mcpu=cortex-m23 -mthumb -Oz -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM23 -D_RA_ORDINAL=1 -I"/media/fabiop/SSD_1T/Projects/e2studio/C64PSU/src" -I"/media/fabiop/SSD_1T/Projects/e2studio/C64PSU/ra/fsp/inc" -I"/media/fabiop/SSD_1T/Projects/e2studio/C64PSU/src/ssd1306" -I"/media/fabiop/SSD_1T/Projects/e2studio/C64PSU/src/ssd1306/fonts" -I"/media/fabiop/SSD_1T/Projects/e2studio/C64PSU/ra/fsp/inc/api" -I"/media/fabiop/SSD_1T/Projects/e2studio/C64PSU/ra/fsp/inc/instances" -I"/media/fabiop/SSD_1T/Projects/e2studio/C64PSU/ra_gen" -I"/media/fabiop/SSD_1T/Projects/e2studio/C64PSU/ra_cfg/fsp_cfg/bsp" -I"/media/fabiop/SSD_1T/Projects/e2studio/C64PSU/ra_cfg/fsp_cfg" -I"/media/fabiop/SSD_1T/Projects/e2studio/C64PSU/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"." -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

