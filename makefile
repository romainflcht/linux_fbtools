# _ FILES ______________________________________________________________________
TARGET    = main
EXEC_NAME = fbtools


# _ DIRECTORIES ________________________________________________________________
SRCS_DIR = srcs
OBJS_DIR = objs
INCS_DIR = includes
BIN_DIR  = bin

# _ FILES ______________________________________________________________________
SRCS = main.c graphics.c colors.c iso_font.c utils.c
OBJS = $(addprefix $(OBJS_DIR)/,$(SRCS:.c=.o))


# _ COMPILER OPT _______________________________________________________________
CC     = arm-uclinuxeabi-gcc

CFLAGS = -I$(INCS_DIR) -march=armv7-m -mtune=cortex-m4 -mlittle-endian -mthumb \
		 -Os -ffast-math -ffunction-sections -fdata-sections \
		 -Wl,--gc-sections -fno-common --param max-inline-insns-single=1000 \
		 -Wl,-elf2flt=-s -Wl,-elf2flt=16384 -Wall -Wextra -Werror

LFLAGS = 

# _ FONT _______________________________________________________________________
MAGENTA  = \e[35m
CYAN     = \e[36m
WHITE    = \e[37m
YELLOW   = \e[33m
RED      = \e[31m
GREEN    = \e[32m

BOLD     = \e[1m
RST      = \e[0m
CLEAR    = \e[2J
CUR_HOME = \e[H

# _ ROOFS BINARY PATH __________________________________________________________
UCLINUX_PATH    = /home/romain/Developer/embedded_c/uClinux/stm32f429-linux-maker_v2/
ROOTFS_BIN_PATH = /home/romain/Developer/embedded_c/uClinux/stm32f429-linux-maker_v2/rootfs/usr/bin

# _ TTY ________________________________________________________________________
TTY_STM32  = /dev/ttyACM0
BAUD_SPEED = 115200

all: $(BIN_DIR)/$(EXEC_NAME)

# LINKING ALL OBJECTS FILE FROM OBJS DIRECTORY. 
$(BIN_DIR)/$(EXEC_NAME): $(OBJS) | mkdir_bin
	@echo "\n$(RED)--SOURCES FILE FOUND : $(RST)$(BOLD)$(SRCS)$(RST)"
	@echo "$(YELLOW)--OBJECTS FILE FOUND : $(RST)$(BOLD)$(OBJS)$(RST)"
	@echo "\n$(CYAN)~LINKING $(RST)$(BOLD)$<$(RST)$(CYAN) TO EXECUTABLE TARGET $(RST)$(BOLD)$@$(RST)"
	@$(CC) $^ -o $@ $(CFLAGS) $(LINK)
	@echo "$(GREEN)-> FINISHED!$(RST)"

# COMPILING SOURCES FROM SRCS DIRECTORY. 
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | mkdir_obj
	@echo "$(MAGENTA)~COMPILING $(RST)$(BOLD)$<$(RST)$(MAGENTA) TO $(RST)$(BOLD)$@$(RST)"
	@$(CC) -c $< -o $@ $(CFLAGS)

# COMPILING SOURCES FROM MAIN DIRECTORY. 
$(OBJS_DIR)/$(TARGET).o: $(TARGET).c | mkdir_obj
	@echo "$(MAGENTA)~COMPILING $(WHITE)$(BOLD)$<$(RST)$(MAGENTA) TO $(RST)$(BOLD)$@$(RST)"
	@$(CC) -c $< -o $@ $(CFLAGS)

mkdir_obj: 
	@mkdir -p $(OBJS_DIR)

mkdir_bin: 
	@mkdir -p $(BIN_DIR)

# Copy the newly compiled binary into the /usr/bin uClinux rootfs. 
copy: all 
	@echo "$(BOLD)$(YELLOW)~ COPYING $(EXEC_NAME) INTO $(ROOTFS_BIN_PATH)... ~$(RST)"
	@cp $(BIN_DIR)/$(EXEC_NAME) $(ROOTFS_BIN_PATH)
	@ls $(ROOTFS_BIN_PATH)

# Flash the new rootfs onto the STM32. 
install: copy
	@echo "$(BOLD)$(GREEN)~ BUILDING & INSTALLING ROOTFS ONTO $(TTY_STM32) ~$(RST)"
	@cd $(UCLINUX_PATH) && make clean-rootfs && make build-rootfs && make install
	@uart $(TTY_STM32) $(BAUD_SPEED)

# Emulate binary execution using qmu ARM32 (can't open /dev/fb0).
run: all
	@clear
	@echo "$(GREEN)~ RUNNING $(TARGET)... ~$(RST)"
	@qemu-arm -cpu cortex-m4 $(BIN_DIR)/$(EXEC_NAME)


clean:
	@echo "$(BOLD)$(RED)~ CLEANING BIN DIRECTORY... ~"
	@rm -f $(BIN_DIR)/$(TARGET) $(BIN_DIR)/$(EXEC_NAME).gdb
	@echo "$(BOLD)$(GREEN)~ DONE ~"

	@echo "$(BOLD)$(RED)~ CLEANING OBJS DIRECTORY... ~"
	@rm -rf $(OBJS_DIR)
	@echo "$(BOLD)$(GREEN)~ DONE ~"

.PHONY: all clean mkdir_obj mkdir_bin copy install run