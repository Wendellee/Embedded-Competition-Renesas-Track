################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/devices/dev_gpio.c \
../src/devices/dev_spi.c \
../src/devices/dev_timer.c 

C_DEPS += \
./src/devices/dev_gpio.d \
./src/devices/dev_spi.d \
./src/devices/dev_timer.d 

OBJS += \
./src/devices/dev_gpio.o \
./src/devices/dev_spi.o \
./src/devices/dev_timer.o 

SREC += \
Embedded_Competition_Renesas_Game.srec 

MAP += \
Embedded_Competition_Renesas_Game.map 


# Each subdirectory must supply rules for building sources it contributes
src/devices/%.o: ../src/devices/%.c
	$(file > $@.in,-mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM33 -D_RA_ORDINAL=1 -I"E:/Renesas/prj/RA8P1_workspace/Embedded_Competition_Renesas_Game/ra_cfg/fsp_cfg/bsp" -I"." -I"E:/Renesas/prj/RA8P1_workspace/Embedded_Competition_Renesas_Game/ra_gen" -I"E:/Renesas/prj/RA8P1_workspace/Embedded_Competition_Renesas_Game/ra_cfg/fsp_cfg" -I"E:/Renesas/prj/RA8P1_workspace/Embedded_Competition_Renesas_Game/src" -I"E:/Renesas/prj/RA8P1_workspace/Embedded_Competition_Renesas_Game/ra/fsp/inc" -I"E:/Renesas/prj/RA8P1_workspace/Embedded_Competition_Renesas_Game/ra/fsp/inc/api" -I"E:/Renesas/prj/RA8P1_workspace/Embedded_Competition_Renesas_Game/ra/fsp/inc/instances" -I"E:/Renesas/prj/RA8P1_workspace/Embedded_Competition_Renesas_Game/ra/arm/CMSIS_6/CMSIS/Core/Include" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

