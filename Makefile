SRC_DIR = src
BUILD_DIR = build
TEST_DIR = test

build: clean
	if [ ! -d $(BUILD_DIR) ]; then \
		node-gyp configure; \
	fi;
	node-gyp build;

run:
	node $(TEST_DIR)/node_libpcap.js

clean:
	rm -rf $(BUILD_DIR)

.PHONY: build
