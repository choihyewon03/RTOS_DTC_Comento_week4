################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/IC/eeprom_25lc256.c \
../Drivers/IC/gatedriver_drv8871.c \
../Drivers/IC/pmic_mp5475.c 

OBJS += \
./Drivers/IC/eeprom_25lc256.o \
./Drivers/IC/gatedriver_drv8871.o \
./Drivers/IC/pmic_mp5475.o 

C_DEPS += \
./Drivers/IC/eeprom_25lc256.d \
./Drivers/IC/gatedriver_drv8871.d \
./Drivers/IC/pmic_mp5475.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/IC/%.o Drivers/IC/%.su Drivers/IC/%.cyclo: ../Drivers/IC/%.c Drivers/IC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F413xx -c -I../Core/Inc -I../Drivers/IC -I../Drivers/Protocol -I../App/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-IC

clean-Drivers-2f-IC:
	-$(RM) ./Drivers/IC/eeprom_25lc256.cyclo ./Drivers/IC/eeprom_25lc256.d ./Drivers/IC/eeprom_25lc256.o ./Drivers/IC/eeprom_25lc256.su ./Drivers/IC/gatedriver_drv8871.cyclo ./Drivers/IC/gatedriver_drv8871.d ./Drivers/IC/gatedriver_drv8871.o ./Drivers/IC/gatedriver_drv8871.su ./Drivers/IC/pmic_mp5475.cyclo ./Drivers/IC/pmic_mp5475.d ./Drivers/IC/pmic_mp5475.o ./Drivers/IC/pmic_mp5475.su

.PHONY: clean-Drivers-2f-IC

