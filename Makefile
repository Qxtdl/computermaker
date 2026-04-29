TARGET = linux

ifeq ($(TARGET),linux)
	CC = gcc
	CFLAGS = -Wall -Wno-unused-result -g \
			-Isrc/include -Idep/cglm/include
	LIBS = dep/cglm/libcglm.a dep/glfw/src/libglfw3.a -lm -fsanitize=address
else
	# windows
	CC = x86_64-w64-mingw32-gcc
	CFLAGS = -Wall -Wno-unused-result -g \
			-Isrc/include -Idep/cglm/include \
			-static
	# not in dep folder, install yourself
	LIBS = dep/cglm/windows/libcglm.a dep/glfw/windows/src/libglfw3.a -lopengl32 -lgdi32 -lm
endif

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
ifeq ($(TARGET),linux)
	cd dep/cglm && cmake . -DCGLM_STATIC=ON && make
	cd dep/glfw && cmake . && make
else
	cd dep/cglm && cmake -B windows \
		-DCGLM_STATIC=ON \
		-DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
		-DCMAKE_SYSTEM_NAME=Windows && \
		cmake --build windows

	cd dep/glfw && cmake -B windows \
		-DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
		-DCMAKE_SYSTEM_NAME=Windows && \
		cmake --build windows
endif

compile: $(OUT)
	cp -R res $(BUILD)/.

run:
	./$(OUT)

$(OUT): $(COBJ)
	$(CC) $(CFLAGS) -o $(OUT) $(COBJ) $(LIBS)

$(BUILD)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

