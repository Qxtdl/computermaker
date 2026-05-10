TARGET = windows

ifeq ($(TARGET),linux)
	CC = gcc
	CFLAGS = \
		-Wno-address-of-packed-member \
		-Wall -Wno-unused-result -g \
		-Isrc/include -Idep/cglm/include -Idep/glfw/include
	LIBS = dep/cglm/libcglm.a dep/glfw/src/libglfw3.a -lm -fsanitize=address
else
	# windows
	CC = x86_64-w64-mingw32-gcc
	CFLAGS = \
			-Wno-address-of-packed-member \
			-Wall -Wno-unused-result -g \
			-Isrc/include -Idep/cglm/include -Idep/glfw/include \
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
ci: clean deps2 compile

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

deps2:
ifeq ($(TARGET),linux)
	cd dep/cglm && cmake . -DCGLM_STATIC=ON && make
	cd dep/glfw && cmake . -DGLFW_BUILD_WAYLAND=OFF -DGLFW_BUILD_X11=OFF && make
else
	cd dep/cglm && cmake -B windows \
		-DCGLM_STATIC=ON -DGLFW_BUILD_WAYLAND=OFF -DGLFW_BUILD_X11=OFF \
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
