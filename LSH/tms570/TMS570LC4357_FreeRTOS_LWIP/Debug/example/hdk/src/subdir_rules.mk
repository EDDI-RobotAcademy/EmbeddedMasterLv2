################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
example/hdk/src/lwip_main.obj: C:/Users/user2/Downloads/5557.TMS570LC4357_FreeRTOS_LWIP/TMS570LC4357_FreeRTOS_LWIP/example/hdk/src/lwip_main.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7R4 --code_state=32 --float_support=VFPv3D16 --include_path="C:/Users/user2/Downloads/5557.TMS570LC4357_FreeRTOS_LWIP/TMS570LC4357_FreeRTOS_LWIP" --include_path="C:/ti/ccs1040/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --include_path="C:/Users/user2/Downloads/5557.TMS570LC4357_FreeRTOS_LWIP/TMS570LC4357_FreeRTOS_LWIP" --include_path="C:/Users/user2/Downloads/5557.TMS570LC4357_FreeRTOS_LWIP/TMS570LC4357_FreeRTOS_LWIP/include" --include_path="C:/Users/user2/Downloads/5557.TMS570LC4357_FreeRTOS_LWIP/TMS570LC4357_FreeRTOS_LWIP/example/hdk/inc" --include_path="C:/Users/user2/Downloads/5557.TMS570LC4357_FreeRTOS_LWIP/TMS570LC4357_FreeRTOS_LWIP/lwip-1.4.1" --include_path="C:/Users/user2/Downloads/5557.TMS570LC4357_FreeRTOS_LWIP/TMS570LC4357_FreeRTOS_LWIP/lwip-1.4.1/src/include" --include_path="C:/Users/user2/Downloads/5557.TMS570LC4357_FreeRTOS_LWIP/TMS570LC4357_FreeRTOS_LWIP/lwip-1.4.1/src/include/ipv4" --include_path="C:/Users/user2/Downloads/5557.TMS570LC4357_FreeRTOS_LWIP/TMS570LC4357_FreeRTOS_LWIP/lwip-1.4.1/ports/hdk/check" --include_path="C:/Users/user2/Downloads/5557.TMS570LC4357_FreeRTOS_LWIP/TMS570LC4357_FreeRTOS_LWIP/lwip-1.4.1/ports/hdk/include/netif" --include_path="C:/Users/user2/Downloads/5557.TMS570LC4357_FreeRTOS_LWIP/TMS570LC4357_FreeRTOS_LWIP/lwip-1.4.1/ports/hdk/include" --include_path="C:/Users/user2/Downloads/5557.TMS570LC4357_FreeRTOS_LWIP/TMS570LC4357_FreeRTOS_LWIP/lwip-1.4.1/ports/hdk/netif" --include_path="C:/Users/user2/Downloads/5557.TMS570LC4357_FreeRTOS_LWIP/TMS570LC4357_FreeRTOS_LWIP/lwip-1.4.1/ports/hdk" --include_path="C:/Users/user2/Downloads/5557.TMS570LC4357_FreeRTOS_LWIP/TMS570LC4357_FreeRTOS_LWIP/lwip-1.4.1/apps/httpserver_raw" --define=_TMS570LC43x_ -g --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi --endian=big --preproc_with_compile --preproc_dependency="example/hdk/src/$(basename $(<F)).d_raw" --obj_directory="example/hdk/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


