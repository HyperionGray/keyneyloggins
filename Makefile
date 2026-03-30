CONFIG_MODULE_SIG=n
ifeq ($(KERNELRELEASE),)

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

.PHONY: build clean

build:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)/src modules
	mkdir -p build
	@if ls src/*.ko 1>/dev/null 2>&1; then cp src/*.ko build/; fi

clean:
	rm -rf src/*.o src/*~ src/.*.cmd src/*.ko src/*.mod.c src/*.mod.o src/*.mod
	rm -rf build/*.ko
	rm -f Module.symvers modules.order
else

$(info Building with KERNELRELEASE = ${KERNELRELEASE})
obj-m :=	rootkit.o

endif
