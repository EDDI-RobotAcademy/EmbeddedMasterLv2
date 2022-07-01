################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
source/%.obj: ../source/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/son/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7R4 --code_state=32 --float_support=VFPv3D16 --include_path="/home/son/proj/eddi/academy/EmbeddedMasterLv2/LSH/tms570/TMS570LC4357_FreeRTOS_LWIP" --include_path="/home/son/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --include_path="/home/son/proj/eddi/academy/EmbeddedMasterLv2/LSH/tms570/TMS570LC4357_FreeRTOS_LWIP" --include_path="/home/son/proj/eddi/academy/EmbeddedMasterLv2/LSH/tms570/TMS570LC4357_FreeRTOS_LWIP/include" --include_path="/home/son/C:/Users/user2/Downloads/5557.TMS570LC4357_FreeRTOS_LWIP/TMS570LC4357_FreeRTOS_LWIP/lwip-1.4.1" --define=_TMS570LC43x_ -g --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi --endian=big --preproc_with_compile --preproc_dependency="source/$(basename $(<F)).d_raw" --obj_directory="source" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

source/%.obj: ../source/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/son/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7R4 --code_state=32 --float_support=VFPv3D16 --include_path="/home/son/proj/eddi/academy/EmbeddedMasterLv2/LSH/tms570/TMS570LC4357_FreeRTOS_LWIP" --include_path="/home/son/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --include_path="/home/son/proj/eddi/academy/EmbeddedMasterLv2/LSH/tms570/TMS570LC4357_FreeRTOS_LWIP" --include_path="/home/son/proj/eddi/academy/EmbeddedMasterLv2/LSH/tms570/TMS570LC4357_FreeRTOS_LWIP/include" --include_path="/home/son/C:/Users/user2/Downloads/5557.TMS570LC4357_FreeRTOS_LWIP/TMS570LC4357_FreeRTOS_LWIP/lwip-1.4.1" --define=_TMS570LC43x_ -g --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi --endian=big --preproc_with_compile --preproc_dependency="source/$(basename $(<F)).d_raw" --obj_directory="source" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


