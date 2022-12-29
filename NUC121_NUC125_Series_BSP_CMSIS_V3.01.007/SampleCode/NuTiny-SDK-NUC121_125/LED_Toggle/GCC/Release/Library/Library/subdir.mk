################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/clk.c \
D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/gpio.c \
D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/retarget.c \
D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/sys.c \
D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/uart.c 

OBJS += \
./Library/Library/clk.o \
./Library/Library/gpio.o \
./Library/Library/retarget.o \
./Library/Library/sys.o \
./Library/Library/uart.o 

C_DEPS += \
./Library/Library/clk.d \
./Library/Library/gpio.d \
./Library/Library/retarget.d \
./Library/Library/sys.d \
./Library/Library/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Library/Library/clk.o: D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/clk.c Library/Library/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/CMSIS/Include" -I"D:\work\ChargeStation\NUC121_NUC125_Series_BSP_CMSIS_V3.01.007\Library\StdDriver" -I"D:\work\ChargeStation\NUC121_NUC125_Series_BSP_CMSIS_V3.01.007\SampleCode\NuTiny-SDK-NUC121_125\LED_Toggle\third party library" -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/StdDriver/inc" -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/Device/Nuvoton/NUC121/Include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/Library/gpio.o: D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/gpio.c Library/Library/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/CMSIS/Include" -I"D:\work\ChargeStation\NUC121_NUC125_Series_BSP_CMSIS_V3.01.007\Library\StdDriver" -I"D:\work\ChargeStation\NUC121_NUC125_Series_BSP_CMSIS_V3.01.007\SampleCode\NuTiny-SDK-NUC121_125\LED_Toggle\third party library" -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/StdDriver/inc" -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/Device/Nuvoton/NUC121/Include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/Library/retarget.o: D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/retarget.c Library/Library/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/CMSIS/Include" -I"D:\work\ChargeStation\NUC121_NUC125_Series_BSP_CMSIS_V3.01.007\Library\StdDriver" -I"D:\work\ChargeStation\NUC121_NUC125_Series_BSP_CMSIS_V3.01.007\SampleCode\NuTiny-SDK-NUC121_125\LED_Toggle\third party library" -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/StdDriver/inc" -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/Device/Nuvoton/NUC121/Include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/Library/sys.o: D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/sys.c Library/Library/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/CMSIS/Include" -I"D:\work\ChargeStation\NUC121_NUC125_Series_BSP_CMSIS_V3.01.007\Library\StdDriver" -I"D:\work\ChargeStation\NUC121_NUC125_Series_BSP_CMSIS_V3.01.007\SampleCode\NuTiny-SDK-NUC121_125\LED_Toggle\third party library" -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/StdDriver/inc" -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/Device/Nuvoton/NUC121/Include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/Library/uart.o: D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/uart.c Library/Library/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/CMSIS/Include" -I"D:\work\ChargeStation\NUC121_NUC125_Series_BSP_CMSIS_V3.01.007\Library\StdDriver" -I"D:\work\ChargeStation\NUC121_NUC125_Series_BSP_CMSIS_V3.01.007\SampleCode\NuTiny-SDK-NUC121_125\LED_Toggle\third party library" -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/StdDriver/inc" -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/Device/Nuvoton/NUC121/Include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


