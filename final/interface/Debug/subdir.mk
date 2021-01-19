################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MC1_MAIN.c \
../adc.c \
../interrupt.c \
../keypad.c \
../lcd.c \
../timer.c \
../uart.c 

OBJS += \
./MC1_MAIN.o \
./adc.o \
./interrupt.o \
./keypad.o \
./lcd.o \
./timer.o \
./uart.o 

C_DEPS += \
./MC1_MAIN.d \
./adc.d \
./interrupt.d \
./keypad.d \
./lcd.d \
./timer.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


