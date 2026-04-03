################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_init_f32.c \
../ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_init_q15.c \
../ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_init_q31.c \
../ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_reset_f32.c \
../ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_reset_q15.c \
../ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_reset_q31.c \
../ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_sin_cos_f32.c \
../ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_sin_cos_q31.c 

C_DEPS += \
./ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_init_f32.d \
./ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_init_q15.d \
./ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_init_q31.d \
./ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_reset_f32.d \
./ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_reset_q15.d \
./ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_reset_q31.d \
./ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_sin_cos_f32.d \
./ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_sin_cos_q31.d 

OBJS += \
./ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_init_f32.o \
./ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_init_q15.o \
./ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_init_q31.o \
./ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_reset_f32.o \
./ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_reset_q15.o \
./ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_pid_reset_q31.o \
./ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_sin_cos_f32.o \
./ra/arm/CMSIS-DSP/Source/ControllerFunctions/arm_sin_cos_q31.o 

SREC += \
Renesas_Cup_reailty_ai.srec 

MAP += \
Renesas_Cup_reailty_ai.map 


# Each subdirectory must supply rules for building sources it contributes
ra/arm/CMSIS-DSP/Source/ControllerFunctions/%.o: ../ra/arm/CMSIS-DSP/Source/ControllerFunctions/%.c
	$(file > $@.in,-mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM33 -D_RA_ORDINAL=1 -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src" -I"." -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra/fsp/inc" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra/fsp/inc/api" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra/fsp/inc/instances" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra_gen" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra_cfg/fsp_cfg/bsp" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra_cfg/fsp_cfg" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/led" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra/arm/CMSIS-DSP/PrivateInclude" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/ra/arm/CMSIS-DSP/Include" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/key" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/applications" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/devices" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/drivers" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/libs" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/include" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/rai" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/Net" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/FatFs" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/SD" -I"E:/Renesas/prj/Renesas_Cup_workspace/Renesas_Cup_reailty_ai/src/RTC" -std=c99 -Wno-stringop-overflow -Wno-format-truncation -w --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

