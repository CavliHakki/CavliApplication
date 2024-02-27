################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CavliLib/C16QS.c \
../CavliLib/Serial.c 

OBJS += \
./CavliLib/C16QS.o \
./CavliLib/Serial.o 

C_DEPS += \
./CavliLib/C16QS.d \
./CavliLib/Serial.d 


# Each subdirectory must supply rules for building sources it contributes
CavliLib/%.o CavliLib/%.su CavliLib/%.cyclo: ../CavliLib/%.c CavliLib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G071xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"/home/hk/Cavli/CavliC16QSEVK-MQTT-1/CavliLib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-CavliLib

clean-CavliLib:
	-$(RM) ./CavliLib/C16QS.cyclo ./CavliLib/C16QS.d ./CavliLib/C16QS.o ./CavliLib/C16QS.su ./CavliLib/Serial.cyclo ./CavliLib/Serial.d ./CavliLib/Serial.o ./CavliLib/Serial.su

.PHONY: clean-CavliLib

