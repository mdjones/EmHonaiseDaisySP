.PHONY: lib_prep clean

lib_prep:
	git submodule update --init --recursive
	cd lib/DaisySP/ ; make
	cd lib/libDaisy/ ; make

clean:
	rm -fR projects/*/build
