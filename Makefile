ifndef MBED_HOME
$(error MBED_HOME is not set)
endif

TARGET_BUILD_PATH = $(MBED_HOME)/build/mbed/TARGET_KL25Z/TOOLCHAIN_GCC_ARM
MBED_INCLUDE_PATH = $(MBED_HOME)/libraries/mbed
MBED_INCLUDE_PATHS = -I$(MBED_INCLUDE_PATH)/ -I$(MBED_INCLUDE_PATH)/api -I$(MBED_INCLUDE_PATH)/hal -I$(MBED_INCLUDE_PATH)/common -I$(MBED_INCLUDE_PATH)/targets/cmsis/TARGET_Freescale -I$(MBED_INCLUDE_PATH)/targets/cmsis -I$(MBED_INCLUDE_PATH)/targets/cmsis/TARGET_Freescale/TARGET_KLXX/TARGET_KL25Z -I$(MBED_INCLUDE_PATH)/targets/hal/TARGET_Freescale/TARGET_KLXX -I$(MBED_INCLUDE_PATH)/targets/hal/TARGET_Freescale/TARGET_KLXX/TARGET_KL25Z

GCC_BIN = 

AS      = $(GCC_BIN)arm-none-eabi-as
CC      = $(GCC_BIN)arm-none-eabi-gcc
CPP     = $(GCC_BIN)arm-none-eabi-g++
LD      = $(GCC_BIN)arm-none-eabi-gcc
OBJCOPY = $(GCC_BIN)arm-none-eabi-objcopy
OBJDUMP = $(GCC_BIN)arm-none-eabi-objdump
SIZE    = $(GCC_BIN)arm-none-eabi-size 

PROJECT = display_test

CPU = -mcpu=cortex-m0plus -mthumb 
CC_FLAGS = $(CPU) -c -g -fno-common -fmessage-length=0 -Wall -Wextra -fno-exceptions -ffunction-sections -fdata-sections -fomit-frame-pointer -MMD -MP

LD_FLAGS = $(CPU) -Wl,--gc-sections --specs=nano.specs -Wl,--wrap,main -Wl,-Map=$(PROJECT).map,--cref
LD_SYS_LIBS = -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys

ifeq ($(DEBUG), 1)
  CC_FLAGS += -DDEBUG -O0
else
  CC_FLAGS += -DNDEBUG -Os
endif

PROGRAM_OBJECTS = hd44780.o main.o

OBJECTS = $(TARGET_BUILD_PATH)/board.o $(TARGET_BUILD_PATH)/cmsis_nvic.o $(TARGET_BUILD_PATH)/mbed_overrides.o $(TARGET_BUILD_PATH)/retarget.o $(TARGET_BUILD_PATH)/startup_MKL25Z4.o $(TARGET_BUILD_PATH)/system_MKL25Z4.o $(PROGRAM_OBJECTS)

SYS_OBJECTS = 

INCLUDE_PATHS = -I. $(MBED_INCLUDE_PATHS)
LIBRARY_PATHS = -L $(TARGET_BUILD_PATH)
LIBRARIES = -lmbed
LINKER_SCRIPT = $(TARGET_BUILD_PATH)/MKL25Z4.ld


all: $(PROJECT).bin $(PROJECT).hex size


clean:
	rm -f $(PROJECT).bin $(PROJECT).elf $(PROJECT).hex $(PROJECT).map $(PROJECT).lst $(PROGRAM_OBJECTS) $(DEPS)


.asm.o:
	$(CC) $(CPU) -c -x assembler-with-cpp -o $@ $<
.s.o:
	$(CC) $(CPU) -c -x assembler-with-cpp -o $@ $<
.S.o:
	$(CC) $(CPU) -c -x assembler-with-cpp -o $@ $<

.c.o:
	$(CC)  $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu99   $(INCLUDE_PATHS) -o $@ $<

.cpp.o:
	$(CPP) $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu++98 -fno-rtti $(INCLUDE_PATHS) -o $@ $<



$(PROJECT).elf: $(OBJECTS) $(SYS_OBJECTS)
	$(LD) $(LD_FLAGS) -T$(LINKER_SCRIPT) $(LIBRARY_PATHS) -o $@ $^ $(LIBRARIES) $(LD_SYS_LIBS) $(LIBRARIES) $(LD_SYS_LIBS)


$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

$(PROJECT).hex: $(PROJECT).elf
	@$(OBJCOPY) -O ihex $< $@

$(PROJECT).lst: $(PROJECT).elf
	@$(OBJDUMP) -Sdh $< > $@

lst: $(PROJECT).lst

size: $(PROJECT).elf
	$(SIZE) $(PROJECT).elf

DEPS = $(PROGRAM_OBJECTS:.o=.d) $(SYS_OBJECTS:.o=.d)
-include $(DEPS)
