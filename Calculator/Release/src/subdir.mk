################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CalcStack.c \
../src/CalcSymbols.c \
../src/Calculator.c \
../src/CalculatorApp.c 

OBJS += \
./src/CalcStack.o \
./src/CalcSymbols.o \
./src/Calculator.o \
./src/CalculatorApp.o 

C_DEPS += \
./src/CalcStack.d \
./src/CalcSymbols.d \
./src/Calculator.d \
./src/CalculatorApp.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -std=c89 -pedantic -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


