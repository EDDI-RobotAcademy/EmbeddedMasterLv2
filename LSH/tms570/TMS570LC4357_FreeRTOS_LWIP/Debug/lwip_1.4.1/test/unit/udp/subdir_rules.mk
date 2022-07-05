################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
lwip_1.4.1/test/unit/udp/%.obj: ../lwip_1.4.1/test/unit/udp/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armcl" -mv7R5 --code_state=32 --float_support=VFPv3D16 --include_path="C:/Users/a0324020/workspace_v8/TMS570LC4357_FreeRTOS_LWIP/lwip_1.4.1" --include_path="C:/Users/a0324020/workspace_v8/TMS570LC4357_FreeRTOS_LWIP/lwip_1.4.1/apps/httpserver_raw" --include_path="C:/Users/a0324020/workspace_v8/TMS570LC4357_FreeRTOS_LWIP/lwip_1.4.1/hdk/inc" --include_path="C:/Users/a0324020/workspace_v8/TMS570LC4357_FreeRTOS_LWIP/lwip_1.4.1/ports/hdk/check" --include_path="C:/Users/a0324020/workspace_v8/TMS570LC4357_FreeRTOS_LWIP/lwip_1.4.1/ports/hdk/include" --include_path="C:/Users/a0324020/workspace_v8/TMS570LC4357_FreeRTOS_LWIP/lwip_1.4.1/ports/hdk/include/netif" --include_path="C:/Users/a0324020/workspace_v8/TMS570LC4357_FreeRTOS_LWIP/lwip_1.4.1/ports/hdk/netif" --include_path="C:/Users/a0324020/workspace_v8/TMS570LC4357_FreeRTOS_LWIP/lwip_1.4.1/src/include" --include_path="C:/Users/a0324020/workspace_v8/TMS570LC4357_FreeRTOS_LWIP/lwip_1.4.1/src/include/ipv4" --include_path="C:/Users/a0324020/workspace_v8/TMS570LC4357_FreeRTOS_LWIP" --include_path="C:/Users/a0324020/workspace_v8/TMS570LC4357_FreeRTOS_LWIP/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi --preproc_with_compile --preproc_dependency="lwip_1.4.1/test/unit/udp/$(basename $(<F)).d_raw" --obj_directory="lwip_1.4.1/test/unit/udp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


