################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/cdc_serial.c \
C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/descriptors.c \
C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/main.c 

OBJS += \
./User/cdc_serial.o \
./User/descriptors.o \
./User/main.o 

C_DEPS += \
./User/cdc_serial.d \
./User/descriptors.d \
./User/main.d 


# Each subdirectory must supply rules for building sources it contributes
User/cdc_serial.o: C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/cdc_serial.c User/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/CMSIS/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/Device/Nuvoton/NUC121/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/StdDriver/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

User/descriptors.o: C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/descriptors.c User/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/CMSIS/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/Device/Nuvoton/NUC121/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/StdDriver/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

User/main.o: C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/main.c User/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/CMSIS/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/Device/Nuvoton/NUC121/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/StdDriver/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


