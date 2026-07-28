.PHONY: default all header generator sample clean

CC ?= cc
RM ?= rm -f

TARGET ?= glfl.h

GENERATOR_TARGET_NAME ?= generator
ifeq ($(OS),Windows_NT)
GENERATOR_TARGET ?= $(GENERATOR_TARGET_NAME).exe
else
GENERATOR_TARGET ?= $(GENERATOR_TARGET_NAME)
endif

SAMPLE_TARGET_NAME ?= Sample
ifeq ($(OS),Windows_NT)
SAMPLE_TARGET ?= $(SAMPLE_TARGET_NAME).exe
else
SAMPLE_TARGET ?= $(SAMPLE_TARGET_NAME)
endif

default: all

all: header sample

header: $(TARGET)

$(TARGET): $(GENERATOR_TARGET)
	./$(GENERATOR_TARGET) > $(TARGET)

generator: $(GENERATOR_TARGET)

$(GENERATOR_TARGET): generator.c
	$(CC) generator.c -o $(GENERATOR_TARGET)

sample: $(SAMPLE_TARGET)

SAMPLE_CFLAGS += $(shell pkg-config --cflags glfw3) -I.
SAMPLE_LDFLAGS += $(shell pkg-config --libs glfw3)
ifeq ($(OS),Windows_NT)
SAMPLE_LDFLAGS += -lopengl32
else
SAMPLE_LDFLAGS += -lGL -ldl
endif

$(SAMPLE_TARGET): sample.c $(TARGET)
	$(CC) sample.c $(SAMPLE_CFLAGS) $(SAMPLE_LDFLAGS) -o $(SAMPLE_TARGET)

format:
	clang-format -i *.c *.h

tidy: all
	clang-tidy generator.c sample.c -fix-errors

lint: tidy

clobber: clean
	$(RM) $(TARGET)

purge: clobber

rebuild: clobber all

clean:
	$(RM) $(GENERATOR_TARGET) $(SAMPLE_TARGET) $(TARGET)

help:
	@echo "Targets:"
	@echo "	default,all   Build the project"
	@echo "	header        Generate glfl.h"
	@echo "	generator     Build the header generator"
	@echo "	sample        Build the sample program"
	@echo "	format        Format source code"
	@echo "	tidy,lint     Tidy and lint source code"
	@echo "	clean         Remove intermediate files"
	@echo "	clobber,purge Remove all generated files"
	@echo "	rebuild       Clobber and rebuild"
	@echo "	help          Display this message"
