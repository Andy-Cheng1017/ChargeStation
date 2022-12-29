################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/Device/Nuvoton/NUC121/Source/GCC/_syscalls.c 

S_UPPER_SRCS += \
D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/Device/Nuvoton/NUC121/Source/GCC/startup_NUC121.S 

OBJS += \
./CMSIS/CMSIS/GCC/_syscalls.o \
./CMSIS/CMSIS/GCC/startup_NUC121.o 

S_UPPER_DEPS += \
./CMSIS/CMSIS/GCC/startup_NUC121.d 

C_DEPS += \
./CMSIS/CMSIS/GCC/_syscalls.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/CMSIS/GCC/_syscalls.o: D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/Device/Nuvoton/NUC121/Source/GCC/_syscalls.c CMSIS/CMSIS/GCC/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/CMSIS/Include" -I"D:\work\ChargeStation\NUC121_NUC125_Series_BSP_CMSIS_V3.01.007\Library\StdDriver" -I"D:\work\ChargeStation\NUC121_NUC125_Series_BSP_CMSIS_V3.01.007\SampleCode\NuTiny-SDK-NUC121_125\LED_Toggle\third party library" -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/StdDriver/inc" -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/Device/Nuvoton/NUC121/Include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CMSIS/CMSIS/GCC/startup_NUC121.o: D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/Device/Nuvoton/NUC121/Source/GCC/startup_NUC121.S CMSIS/CMSIS/GCC/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


