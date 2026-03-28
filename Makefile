CSRCS = $(shell find src -name '*.c')
COBJS = $(patsubst %.c,$(BUILD)/%.o,$(CSRCS))

DEPS = dep/cglm/libcglm.a

BUILD = build
OUTPUT = $(BUILD)/comm

CC = gcc
INCLUDES = -Isrc/include \
	-Idep/cglm/include

CFLAGS = -Wall \
	-g \
	$(INCLUDES)

LDFLAGS =

LIBS = -lglfw -lGL -lm \
	-fsanitize=address

all: clean deps $(OUTPUT) run

# TODO: other libs should be made a dep not assumed preinstalled
deps:
	cd dep/cglm && cmake . -DCGLM_STATIC=ON && make

$(BUILD)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT): $(COBJS)
	$(CC) $(CFLAGS) $(COBJS) $(LIBS) $(LDFLAGS) -o $(OUTPUT)

clean:
	mkdir -p $(BUILD)
	rm -rf $(BUILD)/*

run:
	$(OUTPUT)
