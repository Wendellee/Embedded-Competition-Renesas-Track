################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/arm/CMSIS-NN/Source/ConcatenationFunctions/arm_concatenation_s8_w.c \
../ra/arm/CMSIS-NN/Source/ConcatenationFunctions/arm_concatenation_s8_x.c \
../ra/arm/CMSIS-NN/Source/ConcatenationFunctions/arm_concatenation_s8_y.c \
../ra/arm/CMSIS-NN/Source/ConcatenationFunctions/arm_concatenation_s8_z.c 

C_DEPS += \
./ra/arm/CMSIS-NN/Source/ConcatenationFunctions/arm_concatenation_s8_w.d \
./ra/arm/CMSIS-NN/Source/ConcatenationFunctions/arm_concatenation_s8_x.d \
./ra/arm/CMSIS-NN/Source/ConcatenationFunctions/arm_concatenation_s8_y.d \
./ra/arm/CMSIS-NN/Source/ConcatenationFunctions/arm_concatenation_s8_z.d 

OBJS += \
./ra/arm/CMSIS-NN/Source/ConcatenationFunctions/arm_concatenation_s8_w.o \
./ra/arm/CMSIS-NN/Source/ConcatenationFunctions/arm_concatenation_s8_x.o \
./ra/arm/CMSIS-NN/Source/ConcatenationFunctions/arm_concatenation_s8_y.o \
./ra/arm/CMSIS-NN/Source/ConcatenationFunctions/arm_concatenation_s8_z.o 

SREC += \
DS_1_13_struct.srec 

MAP += \
DS_1_13_struct.map 


# Each subdirectory must supply rules for building sources it contributes
ra/arm/CMSIS-NN/Source/ConcatenationFunctions/%.o: ../ra/arm/CMSIS-NN/Source/ConcatenationFunctions/%.c
	$(file > $@.in,-mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM33 -D_RA_ORDINAL=1 -I"E:/Renesas/prj/Renesas_Cup_workspace/DS_1_13_struct/src" -I"." -I"E:/Renesas/prj/Renesas_Cup_workspace/DS_1_13_struct/ra/fsp/inc" -I"E:/Renesas/prj/Renesas_Cup_workspace/DS_1_13_struct/ra/fsp/inc/api" -I"E:/Renesas/prj/Renesas_Cup_workspace/DS_1_13_struct/ra/fsp/inc/instances" -I"E:/Renesas/prj/Renesas_Cup_workspace/DS_1_13_struct/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"E:/Renesas/prj/Renesas_Cup_workspace/DS_1_13_struct/ra_gen" -I"E:/Renesas/prj/Renesas_Cup_workspace/DS_1_13_struct/ra_cfg/fsp_cfg/bsp" -I"E:/Renesas/prj/Renesas_Cup_workspace/DS_1_13_struct/ra_cfg/fsp_cfg" -I"E:/Renesas/prj/Renesas_Cup_workspace/DS_1_13_struct/src/led" -I"E:/Renesas/prj/Renesas_Cup_workspace/DS_1_13_struct/ra/arm/CMSIS-DSP/PrivateInclude" -I"E:/Renesas/prj/Renesas_Cup_workspace/DS_1_13_struct/ra/arm/CMSIS-DSP/Include" -I"E:/Renesas/prj/Renesas_Cup_workspace/DS_1_13_struct/ra/arm/CMSIS-NN/Include" -I"E:/Renesas/prj/Renesas_Cup_workspace/DS_1_13_struct/ra/arm/CMSIS-NN" -I"E:/Renesas/prj/Renesas_Cup_workspace/DS_1_13_struct/src/key" -std=c99 -Wno-stringop-overflow -Wno-format-truncation -w --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

