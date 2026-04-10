CC = gcc
CFLAGS = -Wall -Wno-unused-result -g \
		 -Isrc/include -Idep/cglm/include
LIBS = -lglfw -lGL -lm -fsanitize=address

SRC = src
BUILD = build
OUT = $(BUILD)/comm

CSRC = $(shell find $(SRC) -type f -name '*.c')
COBJ = $(patsubst %.c,$(BUILD)/%.o,$(CSRC))

all: clean deps compile

clean:
	mkdir -p $(BUILD)
	rm -rf $(BUILD)/*

deps:
	# TODO: other libs should be made a dep not assumed preinstalled
	cd dep/cglm && cmake . -DCGLM_STATIC=ON && make

compile: $(OUT)
	cp -R res $(BUILD)/.

run:
	./$(OUT)

$(OUT): $(COBJ)
	$(CC) $(CFLAGS) -o $(OUT) $(COBJ) $(LIBS)

$(BUILD)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

