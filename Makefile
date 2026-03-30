# keyneyloggins - top-level build
# Delegates to src/ for the kernel module build

.PHONY: build clean reload

build:
	$(MAKE) -C src build

clean:
	$(MAKE) -C src clean

reload:
	./reload.sh
