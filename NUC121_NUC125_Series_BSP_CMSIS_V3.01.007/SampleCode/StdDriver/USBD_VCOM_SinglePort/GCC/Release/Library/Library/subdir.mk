################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/clk.c \
C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/retarget.c \
C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/sys.c \
C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/uart.c \
C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/usbd.c 

OBJS += \
./Library/Library/clk.o \
./Library/Library/retarget.o \
./Library/Library/sys.o \
./Library/Library/uart.o \
./Library/Library/usbd.o 

C_DEPS += \
./Library/Library/clk.d \
./Library/Library/retarget.d \
./Library/Library/sys.d \
./Library/Library/uart.d \
./Library/Library/usbd.d 


# Each subdirectory must supply rules for building sources it contributes
Library/Library/clk.o: C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/clk.c Library/Library/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/CMSIS/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/Device/Nuvoton/NUC121/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/StdDriver/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/Library/retarget.o: C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/retarget.c Library/Library/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/CMSIS/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/Device/Nuvoton/NUC121/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/StdDriver/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/Library/sys.o: C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/sys.c Library/Library/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/CMSIS/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/Device/Nuvoton/NUC121/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/StdDriver/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/Library/uart.o: C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/uart.c Library/Library/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/CMSIS/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/Device/Nuvoton/NUC121/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/StdDriver/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/Library/usbd.o: C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/Library/StdDriver/src/usbd.c Library/Library/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/CMSIS/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/Device/Nuvoton/NUC121/Include" -I"C:/Users/swps9/Downloads/NUC121_NUC125_Series_BSP_CMSIS_V3.01.007/SampleCode/StdDriver/USBD_VCOM_SinglePort/GCC/../../../../Library/StdDriver/inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


