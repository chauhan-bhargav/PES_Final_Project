################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Final_Project.c \
../source/accelerometer.c \
../source/audio_out.c \
../source/cbfifo.c \
../source/gpio_interrupt.c \
../source/i2c.c \
../source/led.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/sine.c \
../source/sysclock.c \
../source/uart.c 

OBJS += \
./source/Final_Project.o \
./source/accelerometer.o \
./source/audio_out.o \
./source/cbfifo.o \
./source/gpio_interrupt.o \
./source/i2c.o \
./source/led.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/sine.o \
./source/sysclock.o \
./source/uart.o 

C_DEPS += \
./source/Final_Project.d \
./source/accelerometer.d \
./source/audio_out.d \
./source/cbfifo.d \
./source/gpio_interrupt.d \
./source/i2c.d \
./source/led.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/sine.d \
./source/sysclock.d \
./source/uart.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\bharg\Documents\MCUXpressoIDE_11.2.0_4120\workspace\Final_Project\board" -I"C:\Users\bharg\Documents\MCUXpressoIDE_11.2.0_4120\workspace\Final_Project\source" -I"C:\Users\bharg\Documents\MCUXpressoIDE_11.2.0_4120\workspace\Final_Project" -I"C:\Users\bharg\Documents\MCUXpressoIDE_11.2.0_4120\workspace\Final_Project\drivers" -I"C:\Users\bharg\Documents\MCUXpressoIDE_11.2.0_4120\workspace\Final_Project\CMSIS" -I"C:\Users\bharg\Documents\MCUXpressoIDE_11.2.0_4120\workspace\Final_Project\utilities" -I"C:\Users\bharg\Documents\MCUXpressoIDE_11.2.0_4120\workspace\Final_Project\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


