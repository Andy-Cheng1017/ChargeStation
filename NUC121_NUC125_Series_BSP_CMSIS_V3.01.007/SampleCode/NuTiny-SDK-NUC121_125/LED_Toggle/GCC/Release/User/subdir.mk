################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/main.c 

OBJS += \
./User/main.o 

C_DEPS += \
./User/main.d 


# Each subdirectory must supply rules for building sources it contributes
User/main.o: D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/main.c User/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/CMSIS/Include" -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/StdDriver/inc" -I"D:/work/ChargeStation/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/NuTiny-SDK-NUC121_125/LED_Toggle/GCC/../../../../Library/Device/Nuvoton/NUC121/Include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

