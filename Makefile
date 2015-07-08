#******************************************************************************
#
# The compiler to be used.
#
#******************************************************************************
OUTPUT_DIR			?=Output
OBJ_BUILD			?=${OUTPUT_DIR}/obj
TARGET_DESC			?=LightCCB
OPT					?=3
CPU_NAME			?=cortex-m4
ENABLE_OS			?=1
STM32F429x			?=0

OPENOCD           	?= openocd
OPENOCD_INTERFACE 	?= interface/stlink-v2.cfg
OPENOCD_TARGET    	?= target/stm32f4x_stlink.cfg
##############################################################################
#
#   Common directories 
#
##############################################################################
DIR_APP=app
DIR_PLATFORM=platform
DIR_ASIC=${DIR_PLATFORM}/asic
DIR_DEVICES=${DIR_PLATFORM}/driver
DIR_HAL=${DIR_ASIC}/hal
DIR_OS=${DIR_PLATFORM}/os

##############################################################################
#
#   Application Include Directories
#
##############################################################################
CFLAGS_INC=-I ${DIR_APP}/include 

##############################################################################
#
#   Platform Include Directories
#
##############################################################################

## APPLICATION
CFLAGS_INC+=-I ${DIR_APP}/include

## ASIC
CFLAGS_INC+=-I ${DIR_PLATFORM}/common
CFLAGS_INC+=-I ${DIR_ASIC}/stm32f4XX/include
CFLAGS_INC+=-I ${DIR_ASIC}/stm32f4XX/include/cmsis

ifeq ($(STM32F429x), 1)
    LINKER_FILE=${DIR_ASIC}/stm32f4XX/linker/stm32f4xxxg_flash.ld
else
    LINKER_FILE=${DIR_ASIC}/stm32f4XX/linker/stm32f407_flash.ld
endif
## HAL
CFLAGS_INC+=-I ${DIR_HAL}/include

## DEVICES
CFLAGS_INC+=-I ${DIR_DEVICES}/include

## OS
CFLAGS_INC+=-I ${DIR_OS}/Source/include
CFLAGS_INC+=-I ${DIR_OS}/Source/portable/GCC/ARM_CM4F
##############################################################################
#
#   Source directories
#
##############################################################################

## OS
VPATH+=:${DIR_APP}/src

## ASIC

VPATH+=:${DIR_ASIC}/stm32f4XX/src
VPATH+=:${DIR_ASIC}/stm32f4XX/startup


## DEVICES
VPATH+=:${DIR_DEVICES}/src

## HAL
VPATH+=:${DIR_HAL}/src
VPATH+=:${DIR_PLATFORM}/test

## OS
VPATH+=:${DIR_OS}/Source
VPATH+=:${DIR_OS}/Source/portable/GCC/ARM_CM4F
VPATH+=:${DIR_OS}/Source/portable/MemMang
VPATH+=:${DIR_PLATFORM}/common



#******************************************************************************
#
# The compiler to be used.
#
#******************************************************************************

ifndef COMPILER
COMPILER=${OBJ_BUILD}
endif

#******************************************************************************
#
# The debugger to be used.
#
#******************************************************************************

ifndef DEBUGGER
DEBUGGER=gdb
endif

###############################################################################
#
# Define FPU settings here
###############################################################################

#FPU+= -mfloat-abi=soft
FPU+=-mfloat-abi=hard  
FPU+=-mfpu=fpv4-sp-d16
FPU+=-D___FPU_USED=1
FPU+=-D___FPU_PRESENT=1

###############################################################################
#
# Define CPU settings here
###############################################################################
CPU_DEF=-mcpu=${CPU_NAME}
CPU_DEF+=-mthumb
CPU_DEF+=${FPU}
CPU_DEF+=-O$(OPT)
CPU_DEF+=-MD -MP
CPU_DEF+=-fsingle-precision-constant
CPU_DEF+=-Wall
CPU_DEF+=-std=gnu99
CPU_DEF+=-gdwarf-2
CPU_DEF+=-Wstrict-prototypes
CPU_DEF+=-ffunction-sections
CPU_DEF+=-fdata-sections
CPU_DEF+=-Wa,-adhlns=${COMPILER}/$(*F).lst 
CPU_DEF+=--specs=rdimon.specs 

## ASIC BUILD DEFINE
CFLAGS_DEF+=-D_USE_STDPERIPH_DRIVER
CFLAGS_DEF+=-D_USE_FULL_ASSERT
CFLAGS_DEF+=-D_HSE_VALUE=8000000

##############################################################################
#
#   The flags passed to the assembler.
#
##############################################################################
AFLAGS=-mcpu=${CPU_NAME}
AFLAGS+=-mthumb
AFLAGS+=${FPU}
AFLAGS+=-O$(OPT)
AFLAGS+=-x assembler-with-cpp
AFLAGS+=-Wa,-amhls=${COMPILER}/$(*F).lst  

##############################################################################
#
# Definitions for using GCC.
#
##############################################################################
ifeq (${COMPILER}, ${OBJ_BUILD})

##############################################################################
#
#   The command for calling the compiler.
#
##############################################################################

CC=arm-none-eabi-gcc

##############################################################################
#
#   The flags passed to the compiler.
#
##############################################################################

CFLAGS+= ${CFLAGS_INC} ${CFLAGS_DEF} ${CPU_DEF}

##############################################################################
#
#   The command for calling the linker.
#
##############################################################################

LD=arm-none-eabi-gcc -mthumb -mcpu=${CPU_NAME} ${FPU} -nostartfiles

##############################################################################
#
#    The flags passed to the linker.
#
##############################################################################

LDFLAGS= -lc -u _printf_float -Wl,-Map=${COMPILER}/${TARGET_DESC}.map,--cref,--gc-sections,--no-warn-mismatch -o ${COMPILER}/${TARGET_DESC}.elf

##############################################################################
#
#    The command for extracting images from the linked executables.
#
##############################################################################

OBJCOPY=arm-none-eabi-objcopy

##############################################################################
#
#    The command for dumping output file
#
##############################################################################

OBJDUMP=arm-none-eabi-objdump
##############################################################################
#
#    The command for show size #
##############################################################################
OBJSIZE=arm-none-eabi-size

endif

#******************************************************************************
#
# Tell the compiler to include debugging information if the DEBUG environment
# variable is set.
#
#******************************************************************************

ifdef DEBUG
CFLAGS += -g
endif

#******************************************************************************
#
# The rule for building the object file from each C source file.
#
#******************************************************************************

${COMPILER}/%.o: %.c
	@if [ 'x${VERBOSE}' = x ];                               \
	 then                                                    \
	     echo "  CC    ${<}";                                \
	 else                                                    \
	     echo ${CC} ${CFLAGS} -o ${@} -c ${<};               \
	 fi
	@${CC} ${CFLAGS} -o ${@} -c ${<}

#******************************************************************************
#
# The rule for building the object file from each assembly source file.
#
#******************************************************************************
${COMPILER}/%.o: %.s
	@if [ 'x${VERBOSE}' = x ];                               \
	 then                                                    \
	     echo "  CC    ${<}";                                \
	 else                                                    \
	     echo ${CC} ${AFLAGS} -o ${@} -c ${<};               \
	 fi
	@${CC} ${AFLAGS} -o ${@} -c ${<}
#******************************************************************************
#
# The rule for creating an object library.
#
#******************************************************************************
${COMPILER}/%.a:
	@if [ 'x${VERBOSE}' = x ];                               \
	 then                                                    \
	     echo "  AR    ${@}";                                \
	 else                                                    \
	     echo ${AR} -cr ${@} ${^};                           \
	 fi
	@${AR} -cr ${@} ${^}

#******************************************************************************
#
# The rule for linking the application.
#
#******************************************************************************

${COMPILER}/%.elf:

	@${LD} -T${SCATTER_${notdir ${@:.elf=}}} --entry ${ENTRY_${notdir ${@:.elf=}}} ${LDFLAGSgcc_${notdir ${@:.elf=}}} ${LDFLAGS} -o ${@} ${^}; echo "  LD    ${@}"            
         
	@${OBJCOPY} -O binary ${@} ${OUTPUT_DIR}/${TARGET_DESC}.bin ; ${OBJCOPY} -O ihex ${@} ${OUTPUT_DIR}/${TARGET_DESC}.hex

##############################################################################
#
#   Include Object files
#
##############################################################################
ifeq ($(ENABLE_OS), 1)
OBJ_OS=${COMPILER}/croutine.o                     \
       ${COMPILER}/list.o                         \
       ${COMPILER}/queue.o                        \
       ${COMPILER}/tasks.o                        \
       ${COMPILER}/timers.o                       \
       ${COMPILER}/port.o                         \
       ${COMPILER}/heap_4.o                       \
       ${COMPILER}/os_assert.o                         
endif

OBJ_ASIC=${COMPILER}/misc.o                         \
         ${COMPILER}/stm32f4xx_system.o             \
	     ${COMPILER}/stm32f4xx_i2c.o                \
	     ${COMPILER}/stm32f4xx_spi.o                \
	     ${COMPILER}/stm32f4xx_gpio.o               \
	     ${COMPILER}/stm32f4xx_rcc.o                \
	     ${COMPILER}/stm32f4xx_flash.o              \
	     ${COMPILER}/stm32f4xx_dma.o                \
	     ${COMPILER}/stm32f4xx_tim.o                \
	     ${COMPILER}/stm32f4xx_usart.o              \
	     ${COMPILER}/stm32f4xx_exti.o              \
	     ${COMPILER}/stm32f4xx_syscfg.o              

ifeq ($(STM32F429x), 1)
OBJ_ASIC+=${COMPILER}/startup_stm32f429xx.o
else
OBJ_ASIC+=${COMPILER}/startup_stm32f40xx.o
endif

OBJ_DRIVER=${COMPILER}/init_platform.o     \
		   ${COMPILER}/log.o 				\
            

OBJ_HAL+=${COMPILER}/hal_pwm.o              \
         ${COMPILER}/hal_com.o              \
         ${COMPILER}/hal_wdg.o              \
         ${COMPILER}/hal_spi.o              


OBJ_APP=${COMPILER}/___main.o             \
        ${COMPILER}/__task_list.o         \
        ${COMPILER}/__task_watchdog_kick.o  \


$(shell mkdir -p ${OUTPUT_DIR} 2>/dev/null)
$(shell mkdir -p ${OBJ_BUILD} 2>/dev/null)

##############################################################################
#
#   The default rule, which causes init to be built.
#
##############################################################################

all: ${COMPILER}                    \
     ${COMPILER}/${TARGET_DESC}.elf \
     showsize 
 

showsize: ${COMPILER}      
	  	  @$(OBJSIZE) -B ${OUTPUT_DIR}/obj/$(TARGET_DESC).elf
 
##############################################################################
#
#   The rule to clean out all the build products
#
############################################################################## 

clean:
	@rm -rf ${OUTPUT_DIR} ${COMPILER} ${wildcard *.bin} ${TARGET_DESC}.elf

docs:
	@mkdir doc; doxygen Doxygen

#flash:
#	@st-flash write Output/LightCCB.bin 0x08000000
	#Flash the stm.
flash:
	$(OPENOCD) -d2 -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) -c init -c targets -c "reset halt" \
                 -c "flash write_image erase $(OBJ_BUILD)/${TARGET_DESC}.elf" -c "verify_image $(OBJ_BUILD)/${TARGET_DESC}.elf" -c "reset run" -c shutdown
                 
reset : 
	$(OPENOCD) -d0 -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) -c init -c targets -c "reset" -c shutdown
#
# The rule to create the target directory
#
${COMPILER}:
	@mkdir ${COMPILER} ${OUTPUT_DIR} 


${COMPILER}/${TARGET_DESC}.elf:  ${OBJ_ASIC} ${OBJ_APP} ${OBJ_OS} ${OBJ_DRIVER} ${OBJ_HAL}
SCATTER_${TARGET_DESC}=${LINKER_FILE}
ENTRY_${TARGET_DESC}=Reset_Handler

#
#
# Include the automatically generated dependency files.
#
-include ${wildcard ${COMPILER}/*.d} __dummy__

