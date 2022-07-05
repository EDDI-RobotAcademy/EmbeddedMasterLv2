################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/bin/armcl" -mv7R4 --code_state=32 --float_support=VFPv3D16 --include_path="C:/Users/a0324020/workspace_v8/TMS570LC4357_FreeRTOS_LWIP" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.4.LTS/include" --include_path="C:/Users/a0324020/workspace_v8/TMS570LC4357_FreeRTOS_LWIP" --include_path="C:/ti/Hercules/HALCoGen EMAC Driver with lwIP Demonstration/v00.03.00/example/hdk/inc" --include_path="C:/ti/Hercules/HALCoGen EMAC Driver with lwIP Demonstration/v00.03.00/lwip-1.4.1" --include_path="C:/ti/Hercules/HALCoGen EMAC Driver with lwIP Demonstration/v00.03.00/lwip-1.4.1/src/include" --include_path="C:/ti/Hercules/HALCoGen EMAC Driver with lwIP Demonstration/v00.03.00/lwip-1.4.1/src/include/ipv4" --include_path="C:/ti/Hercules/HALCoGen EMAC Driver with lwIP Demonstration/v00.03.00/lwip-1.4.1/ports/hdk/check" --include_path="C:/ti/Hercules/HALCoGen EMAC Driver with lwIP Demonstration/v00.03.00/lwip-1.4.1/ports/hdk/include/netif" --include_path="C:/ti/Hercules/HALCoGen EMAC Driver with lwIP Demonstration/v00.03.00/lwip-1.4.1/ports/hdk/include" --include_path="C:/ti/Hercules/HALCoGen EMAC Driver with lwIP Demonstration/v00.03.00/lwip-1.4.1/ports/hdk/netif" --include_path="C:/ti/Hercules/HALCoGen EMAC Driver with lwIP Demonstration/v00.03.00/lwip-1.4.1/ports/hdk" --include_path="C:/ti/Hercules/HALCoGen EMAC Driver with lwIP Demonstration/v00.03.00/lwip-1.4.1/apps/httpserver_raw" --define=_TMS570LC43x_ -g --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi --endian=big --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


