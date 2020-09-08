################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Keil tools exit code 1 is warnings only, so don't treat as an error in make
RC := test $$? -lt 2

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: DFSK-F516-EFM8BB21F16G-C-QFN20_20200107.omf

# Tool invocations
DFSK-F516-EFM8BB21F16G-C-QFN20_20200107.omf: echo_path $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Keil 8051 Linker'
	LX51 "@DFSK-F516-EFM8BB21F16G-C-QFN20_20200107.lnp" || $(RC)
	@echo 'Finished building target: $@'

# Change from all upper case to the expected case
	@-mv "DFSK-F516-EFM8BB21F16G-C-QFN20_20200107.OMF.CRBUILD" "$@"

# Generate hex file: DFSK-F516-EFM8BB21F16G-C-QFN20_20200107.hex
	@Ohx51 "DFSK-F516-EFM8BB21F16G-C-QFN20_20200107.omf" "HEXFILE (DFSK-F516-EFM8BB21F16G-C-QFN20_20200107.hex)" "H386" 2>&1 >/dev/null

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS) DFSK-F516-EFM8BB21F16G-C-QFN20_20200107.omf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
# echo the path
echo_path:
	@echo PATH=$$PATH
	@echo ' '

