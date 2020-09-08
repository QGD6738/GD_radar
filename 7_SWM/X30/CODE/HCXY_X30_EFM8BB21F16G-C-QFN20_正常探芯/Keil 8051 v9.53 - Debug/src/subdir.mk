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

src/Adc.OBJ: C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Cpu.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Adc.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Timer.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/DetectObj.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h

src/Command.OBJ: C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Cpu.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Timer.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Serial.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Uart.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Command.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/DetectObj.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Adc.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/EFM8BB2_FlashPrimitives.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/UartCfg.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h

src/Cpu.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Cpu.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Adc.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Timer.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h

src/DetectObj.OBJ: C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Cpu.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/DetectObj.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Timer.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Adc.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Command.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Serial.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h

src/EFM8BB2_FlashPrimitives.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/EFM8BB2_FlashPrimitives.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h

src/InitDevice.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/InitDevice.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h

src/Main.OBJ: C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Cpu.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Adc.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/InitDevice.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Timer.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Uart.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Serial.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Command.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/DetectObj.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/EFM8BB2_FlashPrimitives.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/UartCfg.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h

src/%.OBJ: ../src/%.A51
	@echo 'Building file: $<'
	@echo 'Invoking: Keil 8051 Assembler'
	AX51 "@$(patsubst %.OBJ,%.__ia,$@)" || $(RC)
	@echo 'Finished building: $<'
	@echo ' '

src/Speaker.OBJ: C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Cpu.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Speaker.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/DetectObj.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h

src/Timer.OBJ: C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Cpu.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Timer.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/DetectObj.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Serial.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h

src/Uart.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Uart.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/UartCfg.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Command.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Cpu.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h

src/UartCfg.OBJ: C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Defs.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Uart.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/UartCfg.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Command.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Cpu.h C:/WorkSpace/7_SWM/X30/HCXY_X30_EFM8BB21F16G-C-QFN20_正常探芯/inc/Timer.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/si_toolchain.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/EFM8BB2/inc/SI_EFM8BB2_Register_Enums.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdint.h C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/8051/v4.1.7/Device/shared/si8051Base/stdbool.h


