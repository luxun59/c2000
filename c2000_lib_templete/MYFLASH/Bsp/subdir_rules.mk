################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Bsp/%.obj: ../Bsp/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/CCS/ccs1040/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -O3 --opt_for_speed=3 --include_path="D:/CCS/workplacec2000/c2000_lib_templete" --include_path="D:/CCS/ccs1040/ccs/tools/compiler/ti-cgt-c2000_21.6.0.LTS/include" --include_path="D:/CCS/workplacec2000/c2000_lib_templete/DCL" --include_path="D:/CCS/workplacec2000/c2000_lib_templete/App/inc" --include_path="D:/CCS/workplacec2000/c2000_lib_templete/dsp" --include_path="D:/CCS/workplacec2000/c2000_lib_templete/Bsp" --include_path="D:/CCS/workplacec2000/c2000_lib_templete/C2000ware" --include_path="D:/CCS/workplacec2000/c2000_lib_templete/C2000ware/inc" --include_path="D:/CCS/workplacec2000/c2000_lib_templete/C2000ware/ccs/Debug" --include_path="D:/CCS/workplacec2000/c2000_lib_templete/Device" --include_path="D:/CCS/workplacec2000/c2000_lib_templete/Lib" --advice:performance=all --define=_FLASH --define=_MYFLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="Bsp/$(basename $(<F)).d_raw" --obj_directory="Bsp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


