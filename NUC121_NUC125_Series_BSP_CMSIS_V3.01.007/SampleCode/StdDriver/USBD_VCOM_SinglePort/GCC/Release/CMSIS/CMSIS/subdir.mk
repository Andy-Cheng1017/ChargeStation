################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/Device/Nuvoton/NUC121/Source/system_NUC121.c 

OBJS += \
./CMSIS/CMSIS/system_NUC121.o 

C_DEPS += \
./CMSIS/CMSIS/system_NUC121.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/CMSIS/system_NUC121.o: C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/Device/Nuvoton/NUC121/Source/system_NUC121.c CMSIS/CMSIS/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/CMSIS/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/Device/Nuvoton/NUC121/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/StdDriver/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

