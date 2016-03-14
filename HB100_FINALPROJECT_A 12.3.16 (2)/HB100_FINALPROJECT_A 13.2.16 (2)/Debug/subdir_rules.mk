################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
FFT_signal.obj: ../FFT_signal.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71/CMSIS-SP-00300-r4p5-00rel0/CMSIS/Include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71" -g --gcc --define=ccs="ccs" --define=__FPU_PRESENT=1 --define=ARM_MATH_CM4 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="FFT_signal.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

buffer.obj: ../buffer.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71/CMSIS-SP-00300-r4p5-00rel0/CMSIS/Include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71" -g --gcc --define=ccs="ccs" --define=__FPU_PRESENT=1 --define=ARM_MATH_CM4 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="buffer.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

filter_data.obj: ../filter_data.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71/CMSIS-SP-00300-r4p5-00rel0/CMSIS/Include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71" -g --gcc --define=ccs="ccs" --define=__FPU_PRESENT=1 --define=ARM_MATH_CM4 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="filter_data.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

gui.obj: ../gui.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71/CMSIS-SP-00300-r4p5-00rel0/CMSIS/Include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71" -g --gcc --define=ccs="ccs" --define=__FPU_PRESENT=1 --define=ARM_MATH_CM4 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="gui.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

images.obj: ../images.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71/CMSIS-SP-00300-r4p5-00rel0/CMSIS/Include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71" -g --gcc --define=ccs="ccs" --define=__FPU_PRESENT=1 --define=ARM_MATH_CM4 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="images.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

lcd_ssd2119_8bit.obj: ../lcd_ssd2119_8bit.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71/CMSIS-SP-00300-r4p5-00rel0/CMSIS/Include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71" -g --gcc --define=ccs="ccs" --define=__FPU_PRESENT=1 --define=ARM_MATH_CM4 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="lcd_ssd2119_8bit.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71/CMSIS-SP-00300-r4p5-00rel0/CMSIS/Include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71" -g --gcc --define=ccs="ccs" --define=__FPU_PRESENT=1 --define=ARM_MATH_CM4 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

math_helper.obj: ../math_helper.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71/CMSIS-SP-00300-r4p5-00rel0/CMSIS/Include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71" -g --gcc --define=ccs="ccs" --define=__FPU_PRESENT=1 --define=ARM_MATH_CM4 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="math_helper.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

samping.obj: ../samping.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71/CMSIS-SP-00300-r4p5-00rel0/CMSIS/Include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71" -g --gcc --define=ccs="ccs" --define=__FPU_PRESENT=1 --define=ARM_MATH_CM4 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="samping.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

signal_dsp.obj: ../signal_dsp.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71/CMSIS-SP-00300-r4p5-00rel0/CMSIS/Include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71" -g --gcc --define=ccs="ccs" --define=__FPU_PRESENT=1 --define=ARM_MATH_CM4 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="signal_dsp.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ti_hamming_window_vector.obj: ../ti_hamming_window_vector.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71/CMSIS-SP-00300-r4p5-00rel0/CMSIS/Include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71" -g --gcc --define=ccs="ccs" --define=__FPU_PRESENT=1 --define=ARM_MATH_CM4 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="ti_hamming_window_vector.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

tm4c123gh6pm_startup_ccs.obj: ../tm4c123gh6pm_startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71/CMSIS-SP-00300-r4p5-00rel0/CMSIS/Include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71" -g --gcc --define=ccs="ccs" --define=__FPU_PRESENT=1 --define=ARM_MATH_CM4 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="tm4c123gh6pm_startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

touch.obj: ../touch.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --opt_for_speed=0 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.2/include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71/CMSIS-SP-00300-r4p5-00rel0/CMSIS/Include" --include_path="C:/ti/TivaWare_C_Series-2.1.1.71" -g --gcc --define=ccs="ccs" --define=__FPU_PRESENT=1 --define=ARM_MATH_CM4 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="touch.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


