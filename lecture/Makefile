BUILD_DIR=build/

PROD_ARGS=-Db_sanitize=none -Dbuildtype=debugoptimized
DEV_ARGS=-Db_sanitize=address -Dbuildtype=debug

init-dev:
	meson setup $(BUILD_DIR) $(DEV_ARGS)

init-prod:
	meson setup $(BUILD_DIR) $(PROD_ARGS)

build:
	ninja -C $(BUILD_DIR) all

ctags:
	ninja -C $(BUILD_DIR) ctags

clean:
	ninja -C $(BUILD_DIR) clean

compdb:
	ninja -C $(BUILD_DIR) -t compdb > compile_commands.json

doc:
	meson compile -C $(BUILD_DIR) liboctangle-doc

reconfigure:
	meson setup $(BUILD_DIR) --reconfigure

wipe:
	meson setup --wipe $(BUILD_DIR)

install:
	ninja -C $(BUILD_DIR) install

test:
	meson test -C $(BUILD_DIR)

all: build ctags compdb

.PHONY: all build ctags compdb clean install doc test reconfigure wipe
