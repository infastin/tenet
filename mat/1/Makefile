BUILD_DIR=build/

init:
	meson $(BUILD_DIR)

build:
	ninja -C $(BUILD_DIR) all

ctags:
	ninja -C $(BUILD_DIR) ctags

clean:
	ninja -C $(BUILD_DIR) clean

compdb:
	ninja -C $(BUILD_DIR) -t compdb > compile_commands.json

reconfigure:
	meson $(BUILD_DIR) --reconfigure

install:
	ninja -C $(BUILD_DIR) install

test:
	meson test -C $(BUILD_DIR)

all: build ctags compdb

.PHONY: all build ctags compdb clean install test reconfigure
