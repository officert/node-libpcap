SRC_DIR = src
BUILD_DIR = build

build: clean
	if [ ! -d $(BUILD_DIR) ]; then \
		node-gyp configure; \
	fi;
	node-gyp build;

run:
	node test/test.js

clean:
	rm -rf $(BUILD_DIR)

.PHONY: build
