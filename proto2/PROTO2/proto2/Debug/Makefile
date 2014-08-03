################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../uCOS_II


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../ADC.c \
../LED.c \
../main.c \
../moteur.c \
../PWM.c \
../UART.c \
../uCOS_II/os_core.c \
../uCOS_II/os_cpu_c.c \
../uCOS_II/os_flag.c \
../uCOS_II/os_mbox.c \
../uCOS_II/os_mem.c \
../uCOS_II/os_mutex.c \
../uCOS_II/os_q.c \
../uCOS_II/os_sem.c \
../uCOS_II/os_task.c \
../uCOS_II/os_time.c \
../uCOS_II/ucos_ii.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
ADC.o \
LED.o \
main.o \
moteur.o \
PWM.o \
UART.o \
uCOS_II/os_core.o \
uCOS_II/os_cpu_c.o \
uCOS_II/os_flag.o \
uCOS_II/os_mbox.o \
uCOS_II/os_mem.o \
uCOS_II/os_mutex.o \
uCOS_II/os_q.o \
uCOS_II/os_sem.o \
uCOS_II/os_task.o \
uCOS_II/os_time.o \
uCOS_II/ucos_ii.o

OBJS_AS_ARGS +=  \
ADC.o \
LED.o \
main.o \
moteur.o \
PWM.o \
UART.o \
uCOS_II/os_core.o \
uCOS_II/os_cpu_c.o \
uCOS_II/os_flag.o \
uCOS_II/os_mbox.o \
uCOS_II/os_mem.o \
uCOS_II/os_mutex.o \
uCOS_II/os_q.o \
uCOS_II/os_sem.o \
uCOS_II/os_task.o \
uCOS_II/os_time.o \
uCOS_II/ucos_ii.o

C_DEPS +=  \
ADC.d \
LED.d \
main.d \
moteur.d \
PWM.d \
UART.d \
uCOS_II/os_core.d \
uCOS_II/os_cpu_c.d \
uCOS_II/os_flag.d \
uCOS_II/os_mbox.d \
uCOS_II/os_mem.d \
uCOS_II/os_mutex.d \
uCOS_II/os_q.d \
uCOS_II/os_sem.d \
uCOS_II/os_task.d \
uCOS_II/os_time.d \
uCOS_II/ucos_ii.d

C_DEPS_AS_ARGS +=  \
ADC.d \
LED.d \
main.d \
moteur.d \
PWM.d \
UART.d \
uCOS_II/os_core.d \
uCOS_II/os_cpu_c.d \
uCOS_II/os_flag.d \
uCOS_II/os_mbox.d \
uCOS_II/os_mem.d \
uCOS_II/os_mutex.d \
uCOS_II/os_q.d \
uCOS_II/os_sem.d \
uCOS_II/os_task.d \
uCOS_II/os_time.d \
uCOS_II/ucos_ii.d

OUTPUT_FILE_PATH +=proto2.elf

OUTPUT_FILE_PATH_AS_ARGS +=proto2.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler



































./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 3.4.4
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -funsigned-char -funsigned-bitfields -DDEBUG  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega32 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	

uCOS_II/%.o: ../uCOS_II/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 3.4.4
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -funsigned-char -funsigned-bitfields -DDEBUG  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega32 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 3.4.4
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="proto2.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega32 
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "proto2.elf" "proto2.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "proto2.elf" "proto2.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "proto2.elf" > "proto2.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "proto2.elf" "proto2.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-size.exe" "proto2.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "proto2.elf" "proto2.a" "proto2.hex" "proto2.lss" "proto2.eep" "proto2.map" "proto2.srec"
	