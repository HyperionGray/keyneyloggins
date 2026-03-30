CONFIG_MODULE_SIG=n
ifeq ($(KERNELRELEASE),)

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

.PHONY: build clean

build:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)/src modules

clean:
	rm -rf src/*.o src/*~ src/.*.cmd src/*.ko src/*.mod src/*.mod.c src/*.mod.o
	rm -rf src/Module.symvers src/modules.order src/.tmp_versions
else

$(info Building with KERNELRELEASE = ${KERNELRELEASE})
obj-m := rootkit.o

endif
