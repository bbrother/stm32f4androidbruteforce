PROJ_NAME=bruteforceandroid

################################################################################
#                   SETUP TOOLS                                                #
################################################################################

#UPDATE this if you do not have the arm toolchain in your path
#https://launchpad.net/gcc-arm-embedded  is currently the recommended 
#toolchain for getting up to date tools it is recommended by the older
#summon arm toolchain which would not compile for me on ubuntu 13.10
TOOLS_DIR = 

#If necessary change this to reflect where you have unzipped the 
#	STM32F4DISCOVERY board firmware package http://www.st.com/web/en/catalog/tools/PF257904
STM_ROOT         =../STM32F4-Discovery_FW_V1.1.0

CC      = $(TOOLS_DIR)arm-none-eabi-gcc
OBJCOPY = $(TOOLS_DIR)arm-none-eabi-objcopy
GDB     = $(TOOLS_DIR)arm-none-eabi-gdb
AS      = $(TOOLS_DIR)arm-none-eabi-as

##### Preprocessor options

# directories to be searched for header files
INCLUDE = $(addprefix -I,$(INC_DIRS))

# #defines needed when working with the STM peripherals library
DEFS    = -DUSE_STDPERIPH_DRIVER
# DEFS   += -DUSE_FULL_ASSERT

##### Assembler options

AFLAGS  = -mcpu=cortex-m4 
AFLAGS += -mthumb
AFLAGS += -mthumb-interwork
AFLAGS += -mlittle-endian
AFLAGS += -mfloat-abi=hard
AFLAGS += -mfpu=fpv4-sp-d16

##### Compiler options

CFLAGS  = -ggdb
CFLAGS += -O0
CFLAGS += -Wall -Wextra -Warray-bounds
CFLAGS += $(AFLAGS)

##### Linker options

# tell ld which linker file to use
# (this file is in the current directory)
LFLAGS  = -Tstm32_flash.ld


################################################################################
#                   SOURCE FILES DIRECTORIES                                   #
################################################################################

STM_SRC_DIR      = $(STM_ROOT)/Libraries/STM32F4xx_StdPeriph_Driver/src
STM_SRC_DIR     += $(STM_ROOT)/Utilities/STM32F4-Discovery
STM_SRC_DIR     += $(STM_ROOT)/Libraries/STM32_USB_Device_Library/Core/src
STM_SRC_DIR     += $(STM_ROOT)/Libraries/STM32_USB_OTG_Driver/src

STM_STARTUP_DIR += $(STM_ROOT)/Libraries/CMSIS/ST/STM32F4xx/Source/Templates/TrueSTUDIO

vpath %.c $(STM_SRC_DIR)
vpath %.s $(STM_STARTUP_DIR)


################################################################################
#                   HEADER FILES DIRECTORIES                                   #
################################################################################

# The header files we use are located here
INC_DIRS  = $(STM_ROOT)/Utilities/STM32F4-Discovery
INC_DIRS += $(STM_ROOT)/Libraries/CMSIS/Include
INC_DIRS += $(STM_ROOT)/Libraries/CMSIS/ST/STM32F4xx/Include
INC_DIRS += $(STM_ROOT)/Libraries/STM32F4xx_StdPeriph_Driver/inc
INC_DIRS += $(STM_ROOT)/Libraries/STM32_USB_Device_Library/Core/inc
INC_DIRS += $(STM_ROOT)/Libraries/STM32_USB_OTG_Driver/inc
INC_DIRS += .


################################################################################
#                   SOURCE FILES TO COMPILE                                    #
################################################################################


SRCS  += main.c
SRCS  += system_stm32f4xx.c
SRCS  += stm32f4_discovery.c
SRCS  += stm32f4xx_rcc.c
SRCS  += stm32f4xx_gpio.c
SRCS  += stm32f4xx_tim.c
SRCS  += stm32f4xx_exti.c
SRCS  += stm32f4xx_syscfg.c
SRCS  += misc.c
SRCS  +=usb_bsp.c
SRCS  +=usbd_usr.c
SRCS  +=usbd_desc.c
SRCS  +=stm32f4xx_it.c
SRCS  +=keyboard.c
SRCS  +=permute.c
#usb drivers
SRCS  += usbd_hid_core.c
SRCS  +=usbd_core.c  
SRCS  +=usbd_ioreq.c
SRCS  +=usbd_req.c
#SRCS  +=usb_bsp_template.c
SRCS  +=usb_core.c
SRCS  +=usb_dcd.c
SRCS  +=usb_dcd_int.c
#SRCS  +=usb_hcd.c
#SRCS  +=usb_hcd_int.c
#SRCS  +=usb_otg.c


# startup file, calls main
ASRC  = startup_stm32f4xx.s

OBJS  = $(SRCS:.c=.o)
OBJS += $(ASRC:.s=.o)


######################################################################
#                         SETUP TARGETS                              #
######################################################################

.PHONY: all

all: $(PROJ_NAME).elf


%.o : %.c
	@echo "[Compiling  ]  $^"
	@$(CC) -c -o $@ $(INCLUDE) $(DEFS) $(CFLAGS) $^

%.o : %.s
	@echo "[Assembling ]" $^
	@$(AS) $(AFLAGS) $< -o $@


$(PROJ_NAME).elf: $(OBJS)
	@echo "[Linking    ]  $@"
	@$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@ 
	@$(OBJCOPY) -O ihex $(PROJ_NAME).elf   $(PROJ_NAME).hex
	@$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

clean:
	rm -f *.o $(PROJ_NAME).elf $(PROJ_NAME).hex $(PROJ_NAME).bin

flash: all
	st-flash write $(PROJ_NAME).bin 0x8000000

debug:
# before you start gdb, you must start st-util
	$(GDB) -tui $(PROJ_NAME).elf


