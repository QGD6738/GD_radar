################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
A51_UPPER_SRCS += \
../src/SILABS_STARTUP.A51 

C_SRCS += \
../src/Adc.c \
../src/Command.c \
../src/Cpu.c \
../src/DetectObj.c \
../src/EFM8BB2_FlashPrimitives.c \
../src/InitDevice.c \
../src/Main.c \
../src/Serial.c \
../src/Speaker.c \
../src/Timer.c \
../src/Uart.c \
../src/UartCfg.c 

OBJS += \
./src/Adc.OBJ \
./src/Command.OBJ \
./src/Cpu.OBJ \
./src/DetectObj.OBJ \
./src/EFM8BB2_FlashPrimitives.OBJ \
./src/InitDevice.OBJ \
./src/Main.OBJ \
./src/SILABS_STARTUP.OBJ \
./src/Serial.OBJ \
./src/Speaker.OBJ \
./src/Timer.OBJ \
./src/Uart.OBJ \
./src/UartCfg.OBJ 


# Each subdirectory must supply rules for building sources it contributes
src/%.OBJ: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Compiler'
	C51 "@$(patsubst %.OBJ,%.__i,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '

src/Adc.OBJ: G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Cpu.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Adc.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Command.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Timer.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/DetectObj.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Serial.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/si_toolchain.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdint.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdbool.h

src/Command.OBJ: G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Cpu.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Timer.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Serial.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Uart.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Command.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/DetectObj.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Adc.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/EFM8BB2_FlashPrimitives.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/UartCfg.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/si_toolchain.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdint.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdbool.h

src/Cpu.OBJ: D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Cpu.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Adc.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Timer.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/si_toolchain.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdint.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdbool.h

src/DetectObj.OBJ: G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Cpu.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/DetectObj.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Timer.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Adc.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Command.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Serial.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/si_toolchain.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdint.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdbool.h

src/EFM8BB2_FlashPrimitives.OBJ: D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/EFM8BB2_FlashPrimitives.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/si_toolchain.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdint.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdbool.h

src/InitDevice.OBJ: D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/InitDevice.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/si_toolchain.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdint.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdbool.h

src/Main.OBJ: G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Cpu.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Adc.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/InitDevice.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Timer.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Serial.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Uart.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Command.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/DetectObj.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/EFM8BB2_FlashPrimitives.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/UartCfg.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/si_toolchain.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdint.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdbool.h

src/%.OBJ: ../src/%.A51
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Assembler'
	AX51 "@$(patsubst %.OBJ,%.__ia,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '

src/Serial.OBJ: G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Cpu.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/DetectObj.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Timer.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Serial.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Command.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Uart.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/UartCfg.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/si_toolchain.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdint.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdbool.h

src/Speaker.OBJ: G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Cpu.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Speaker.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/DetectObj.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/si_toolchain.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdint.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdbool.h

src/Timer.OBJ: G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Cpu.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Timer.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/DetectObj.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Serial.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/si_toolchain.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdint.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdbool.h

src/Uart.OBJ: D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Uart.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/UartCfg.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Command.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Cpu.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Timer.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/si_toolchain.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdint.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdbool.h

src/UartCfg.OBJ: D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Uart.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/UartCfg.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Command.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Cpu.h G:/Office_information/souce_software/1_DFSK/F508/1_CODE/DFSK-F508-EFM8BB21F16G-C-QFN20/inc/Timer.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/si_toolchain.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdint.h D:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.5/Device/shared/si8051Base/stdbool.h


