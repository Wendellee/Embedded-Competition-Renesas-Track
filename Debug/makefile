################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

-include makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/rai/subdir.mk
-include src/libs/ring_buffer/subdir.mk
-include src/libs/delay/subdir.mk
-include src/led/subdir.mk
-include src/key/subdir.mk
-include src/drivers/subdir.mk
-include src/devices/DHT11/subdir.mk
-include src/devices/ADXL345/subdir.mk
-include src/devices/subdir.mk
-include src/debug_uart/subdir.mk
-include src/applications/subdir.mk
-include src/SEGGER_RTT/subdir.mk
-include src/SD/subdir.mk
-include src/RTC/subdir.mk
-include src/Net/onenet/subdir.mk
-include src/Net/device/subdir.mk
-include src/Net/cJSON/subdir.mk
-include src/Net/MQTT/subdir.mk
-include src/FatFs/ff15/subdir.mk
-include src/subdir.mk
-include ra_gen/subdir.mk
-include ra/fsp/src/rm_rai_data_shipper/subdir.mk
-include ra/fsp/src/rm_rai_data_collector/subdir.mk
-include ra/fsp/src/rm_comms_uart/subdir.mk
-include ra/fsp/src/rm_comms_lock/subdir.mk
-include ra/fsp/src/r_spi/subdir.mk
-include ra/fsp/src/r_sdhi/subdir.mk
-include ra/fsp/src/r_sci_uart/subdir.mk
-include ra/fsp/src/r_rtc/subdir.mk
-include ra/fsp/src/r_ioport/subdir.mk
-include ra/fsp/src/r_gpt/subdir.mk
-include ra/fsp/src/r_dmac/subdir.mk
-include ra/fsp/src/bsp/mcu/ra6m5/subdir.mk
-include ra/fsp/src/bsp/mcu/all/subdir.mk
-include ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/subdir.mk
-include ra/arm/CMSIS-DSP/Source/WindowFunctions/subdir.mk
-include ra/arm/CMSIS-DSP/Source/TransformFunctions/subdir.mk
-include ra/arm/CMSIS-DSP/Source/SupportFunctions/subdir.mk
-include ra/arm/CMSIS-DSP/Source/StatisticsFunctions/subdir.mk
-include ra/arm/CMSIS-DSP/Source/SVMFunctions/subdir.mk
-include ra/arm/CMSIS-DSP/Source/QuaternionMathFunctions/subdir.mk
-include ra/arm/CMSIS-DSP/Source/MatrixFunctions/subdir.mk
-include ra/arm/CMSIS-DSP/Source/InterpolationFunctions/subdir.mk
-include ra/arm/CMSIS-DSP/Source/FilteringFunctions/subdir.mk
-include ra/arm/CMSIS-DSP/Source/FastMathFunctions/subdir.mk
-include ra/arm/CMSIS-DSP/Source/DistanceFunctions/subdir.mk
-include ra/arm/CMSIS-DSP/Source/ControllerFunctions/subdir.mk
-include ra/arm/CMSIS-DSP/Source/ComplexMathFunctions/subdir.mk
-include ra/arm/CMSIS-DSP/Source/CommonTables/subdir.mk
-include ra/arm/CMSIS-DSP/Source/BayesFunctions/subdir.mk
-include ra/arm/CMSIS-DSP/Source/BasicMathFunctions/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(JMP_DEPS)),)
-include $(JMP_DEPS)
endif
ifneq ($(strip $(FSY_DEPS)),)
-include $(FSY_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(SRC_UPPER_DEPS)),)
-include $(SRC_UPPER_DEPS)
endif
ifneq ($(strip $(P_UPPER_DEPS)),)
-include $(P_UPPER_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(PP_UPPER_DEPS)),)
-include $(PP_UPPER_DEPS)
endif
ifneq ($(strip $(P_DEPS)),)
-include $(P_DEPS)
endif
ifneq ($(strip $(FSY_UPPER_DEPS)),)
-include $(FSY_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif
ifneq ($(strip $(JMP_UPPER_DEPS)),)
-include $(JMP_UPPER_DEPS)
endif
ifneq ($(strip $(PP_DEPS)),)
-include $(PP_DEPS)
endif
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(SX_UPPER_DEPS)),)
-include $(SX_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
Renesas_Cup_reailty_ai.srec \

SECONDARY_SIZE += \
Renesas_Cup_reailty_ai.siz \


# All Target
all: Renesas_Cup_reailty_ai.elf secondary-outputs

# Tool invocations
Renesas_Cup_reailty_ai.elf: $(OBJS) $(USER_OBJS) $(LINKER_SCRIPT)
	$(file > Renesas_Cup_reailty_ai.elf.in,-mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -T "fsp.ld" -Xlinker --gc-sections -L "E:\\Renesas\\prj\\Renesas_Cup_workspace\\Renesas_Cup_reailty_ai\\src\\rai" -L "E:\\Renesas\\prj\\Renesas_Cup_workspace\\Renesas_Cup_reailty_ai/script" -Wl,-Map,"Renesas_Cup_reailty_ai.map" --specs=nano.specs -u _printf_float -o "Renesas_Cup_reailty_ai.elf" -Wl,--start-group $(OBJS) $(USER_OBJS) $(LIBS) -Wl,--end-group)
	@echo Building target: $@ && arm-none-eabi-gcc @"Renesas_Cup_reailty_ai.elf.in"
Renesas_Cup_reailty_ai.srec: Renesas_Cup_reailty_ai.elf
	arm-none-eabi-objcopy -O srec "Renesas_Cup_reailty_ai.elf"  "Renesas_Cup_reailty_ai.srec"
Renesas_Cup_reailty_ai.siz: Renesas_Cup_reailty_ai.elf
	arm-none-eabi-size --format=berkeley "Renesas_Cup_reailty_ai.elf"
# Other Targets
clean:
	-$(file > clean_file.tmp,  $(JMP_DEPS)  $(FSY_DEPS)  $(C_UPPER_DEPS)  $(SECONDARY_SIZE)  $(SRC_UPPER_DEPS)  $(P_UPPER_DEPS)  $(S_DEPS)  $(PP_UPPER_DEPS)  $(P_DEPS)  $(FSY_UPPER_DEPS)  $(C_DEPS)  $(SRC_DEPS)  $(JMP_UPPER_DEPS)  $(PP_DEPS)  $(SX_DEPS)  $(ASM_UPPER_DEPS)  $(SX_UPPER_DEPS)  $(OBJS)  $(SECONDARY_FLASH)  $(ASM_DEPS)  $(SREC)  $(S_UPPER_DEPS)  $(MAP) )
	-@ xargs -P 2 -s 32000 -t -a clean_file.tmp $(RM)
	-@$(RM) clean_file.tmp
	-$(RM)  Renesas_Cup_reailty_ai.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
