# SPDX-License-Identifier: GPL-2.0

# find target path all c file 
# example $(call FIND_SRC_FILE,./target)
define FIND_SRC_FILE
	$(addprefix $(1)/,$(notdir $(patsubst %.c,%.o,$(wildcard $(dir $(abspath $(lastword $(MAKEFILE_LIST))))$(1)/*.c))))
endef
