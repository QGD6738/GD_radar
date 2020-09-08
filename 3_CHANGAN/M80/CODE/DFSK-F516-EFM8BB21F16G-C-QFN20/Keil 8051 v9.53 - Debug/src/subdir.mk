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

src/Adc.OBJ: C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Cpu.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Adc.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Command.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Timer.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/DetectObj.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Serial.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdbool.h

src/Command.OBJ: C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Cpu.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Timer.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Serial.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Uart.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Command.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/DetectObj.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Adc.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/EFM8BB2_FlashPrimitives.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/UartCfg.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdbool.h

src/Cpu.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Cpu.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Adc.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Timer.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdbool.h

src/DetectObj.OBJ: C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Cpu.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/DetectObj.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Timer.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Adc.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Command.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Serial.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdbool.h

src/EFM8BB2_FlashPrimitives.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/EFM8BB2_FlashPrimitives.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdbool.h

src/InitDevice.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/InitDevice.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdbool.h

src/Main.OBJ: C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Cpu.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Adc.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/InitDevice.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Timer.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Serial.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Uart.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Command.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/DetectObj.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/EFM8BB2_FlashPrimitives.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/UartCfg.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdbool.h

src/%.OBJ: ../src/%.A51
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Assembler'
	AX51 "@$(patsubst %.OBJ,%.__ia,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '

src/Serial.OBJ: C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Cpu.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/DetectObj.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Timer.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Serial.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Command.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Uart.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/UartCfg.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdbool.h

src/Speaker.OBJ: C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Cpu.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Speaker.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/DetectObj.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdbool.h

src/Timer.OBJ: C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Cpu.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Timer.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/DetectObj.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Serial.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdbool.h

src/Uart.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Uart.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/UartCfg.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Command.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Cpu.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Timer.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdbool.h

src/UartCfg.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Uart.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/UartCfg.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Command.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Cpu.h C:/Users/lzdd/Desktop/m80/DFSK-F516-EFM8BB21F16G-C-QFN20/inc/Timer.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.6/Device/shared/si8051Base/stdbool.h


