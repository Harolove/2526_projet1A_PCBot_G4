################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/capteur.c \
../Core/Src/comm.c \
../Core/Src/main.c \
../Core/Src/map.c \
../Core/Src/moteur.c \
../Core/Src/odometrie.c \
../Core/Src/stm32g4xx_hal_msp.c \
../Core/Src/stm32g4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g4xx.c \
../Core/Src/zone.c 

OBJS += \
./Core/Src/capteur.o \
./Core/Src/comm.o \
./Core/Src/main.o \
./Core/Src/map.o \
./Core/Src/moteur.o \
./Core/Src/odometrie.o \
./Core/Src/stm32g4xx_hal_msp.o \
./Core/Src/stm32g4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g4xx.o \
./Core/Src/zone.o 

C_DEPS += \
./Core/Src/capteur.d \
./Core/Src/comm.d \
./Core/Src/main.d \
./Core/Src/map.d \
./Core/Src/moteur.d \
./Core/Src/odometrie.d \
./Core/Src/stm32g4xx_hal_msp.d \
./Core/Src/stm32g4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g4xx.d \
./Core/Src/zone.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/capteur.cyclo ./Core/Src/capteur.d ./Core/Src/capteur.o ./Core/Src/capteur.su ./Core/Src/comm.cyclo ./Core/Src/comm.d ./Core/Src/comm.o ./Core/Src/comm.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/map.cyclo ./Core/Src/map.d ./Core/Src/map.o ./Core/Src/map.su ./Core/Src/moteur.cyclo ./Core/Src/moteur.d ./Core/Src/moteur.o ./Core/Src/moteur.su ./Core/Src/odometrie.cyclo ./Core/Src/odometrie.d ./Core/Src/odometrie.o ./Core/Src/odometrie.su ./Core/Src/stm32g4xx_hal_msp.cyclo ./Core/Src/stm32g4xx_hal_msp.d ./Core/Src/stm32g4xx_hal_msp.o ./Core/Src/stm32g4xx_hal_msp.su ./Core/Src/stm32g4xx_it.cyclo ./Core/Src/stm32g4xx_it.d ./Core/Src/stm32g4xx_it.o ./Core/Src/stm32g4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g4xx.cyclo ./Core/Src/system_stm32g4xx.d ./Core/Src/system_stm32g4xx.o ./Core/Src/system_stm32g4xx.su ./Core/Src/zone.cyclo ./Core/Src/zone.d ./Core/Src/zone.o ./Core/Src/zone.su

.PHONY: clean-Core-2f-Src

