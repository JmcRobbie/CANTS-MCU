# Project settings
PROJECT := CANTS-MCU

# Default target, if not overriden
TARGET ?= Debug

# List all folders and subfolders with header files
INCDIRS := src \
           src/boards \
           src/can \
           src/cants \
           src/common \
           src/FreeRTOS/include \
           src/FreeRTOS/portable/GCC/PicoSkyFT \
           src/gpio \
           src/protocol \
           src/timer

# List all folders and subfolders with source files
# NOTE: Every path must end with "/"!
SRCDIRS := src/ \
           src/boards/ \
           src/can/ \
           src/cants/ \
           src/common/ \
           src/gpio/ \
           src/FreeRTOS/ \
           src/FreeRTOS/portable/GCC/PicoSkyFT/ \
           src/protocol/ \
           src/timer/

# List all source files (*.S or *.c) not included in above folders
FILES :=

# Path to PicoSkyFT compiler. Not needed if folder is in PATH.
COMPILER_PATH ?=

# Path to build output directory
OUTPUT_PATH ?= bin

# Extra compiler/linker options
EXTRA_C_FLAGS =
EXTRA_LD_FLAGS =

include mk/rules.mk
