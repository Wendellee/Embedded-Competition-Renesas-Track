################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.c \
../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.c 

C_DEPS += \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.d \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.d 

OBJS += \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/startup.o \
./ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/system.o 

SREC += \
Renesas_Cup_reailty_ai.srec 

MAP += \
Renesas_Cup_reailty_ai.map 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/%.o: ../ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/%.c
	$(file > $@.in,-mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM33 -D_RA_ORDINAL=1 -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src" -I"." -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra/fsp/inc" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra/fsp/inc/api" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra/fsp/inc/instances" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra_gen" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra_cfg/fsp_cfg/bsp" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra_cfg/fsp_cfg" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/led" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra/arm/CMSIS-DSP/PrivateInclude" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra/arm/CMSIS-DSP/Include" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/key" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/applications" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/devices" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/drivers" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/libs" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/include" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/rai" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/Net" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/FatFs" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/SD" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/RTC" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

