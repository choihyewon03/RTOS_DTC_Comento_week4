################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Protocol/can_manager.c 

OBJS += \
./Drivers/Protocol/can_manager.o 

C_DEPS += \
./Drivers/Protocol/can_manager.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Protocol/%.o Drivers/Protocol/%.su Drivers/Protocol/%.cyclo: ../Drivers/Protocol/%.c Drivers/Protocol/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F413xx -c -I../Core/Inc -I../Drivers/IC -I../Drivers/Protocol -I../App/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Protocol

clean-Drivers-2f-Protocol:
	-$(RM) ./Drivers/Protocol/can_manager.cyclo ./Drivers/Protocol/can_manager.d ./Drivers/Protocol/can_manager.o ./Drivers/Protocol/can_manager.su

.PHONY: clean-Drivers-2f-Protocol

