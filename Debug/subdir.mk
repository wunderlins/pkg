################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../options.o \
../stringlib.o \
../upkg.o 

CPP_SRCS += \
../options.cpp \
../stringlib.cpp \
../upkg.cpp 

OBJS += \
./options.o \
./stringlib.o \
./upkg.o 

CPP_DEPS += \
./options.d \
./stringlib.d \
./upkg.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


