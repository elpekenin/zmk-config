ZMK = /home/elpekenin/zmk
CONFIG = $(ZMK)/zmk-config
USERSPACE = $(ZMK)/zmk-userspace

.PHONY: build init

define _compile
	$(eval $@_WEST_ARGS = $(1))
	$(eval $@_CMAKE_ARGS = $(2))
	$(eval $@_CMD = west build -p ${$@_WEST_ARGS} -b cornholius_a -- -DZMK_CONFIG=$(CONFIG)/config -DZMK_EXTRA_MODULES="$(CONFIG);$(USERSPACE)" ${$@_CMAKE_ARGS})
	cd $(ZMK)/app && ${$@_CMD}
endef

build compile:
	$(call _compile,,)

init:
	$(call _compile,-t initlevels,)
