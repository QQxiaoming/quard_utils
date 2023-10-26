# SPDX-License-Identifier: BSD 3-Clause

BUILD_DIR = output

all:macgen mdio translate

clean:
	rm -rf $(BUILD_DIR)

macgen:macgen.c $(BUILD_DIR) Makefile
	gcc -o $(BUILD_DIR)/macgen macgen.c

mdio:mdio.c $(BUILD_DIR) Makefile
	gcc -o $(BUILD_DIR)/mdio mdio.c

translate:translate.c $(BUILD_DIR) Makefile
	gcc -o $(BUILD_DIR)/translate translate.c

pagemap_dump:pagemap_dump.c $(BUILD_DIR) Makefile
	gcc -std=gnu99 -o $(BUILD_DIR)/pagemap_dump pagemap_dump.c

ip-calc-regex:ip-calc-regex.cpp $(BUILD_DIR) Makefile
	g++ -std=c++17 -o $(BUILD_DIR)/ip-calc-regex ip-calc-regex.cpp

ifeq ($(BUILD_DIR), $(wildcard $(BUILD_DIR)))
else
$(BUILD_DIR):
	@mkdir $@
endif