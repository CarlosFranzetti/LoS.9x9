BUILD_DIR ?= build
CMAKE ?= cmake
APP_BUNDLE := $(BUILD_DIR)/LoS9x9_artefacts/LoS9x9.app

.PHONY: configure build run clean rebuild

configure:
	$(CMAKE) -S . -B $(BUILD_DIR)

build: configure
	$(CMAKE) --build $(BUILD_DIR) -j4

run: build
	@if [ -d "$(APP_BUNDLE)" ]; then \
		open "$(APP_BUNDLE)"; \
	else \
		echo "App bundle not found: $(APP_BUNDLE)"; \
		echo "Build first, then adjust APP_BUNDLE if your generator uses a different layout."; \
	fi

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean build
