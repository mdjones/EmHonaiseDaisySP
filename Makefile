.PHONY: lib_prep

lib_prep:
	git submodule update --init --recursive
	cd lib/DaisySP/ ; make
	cd lib/libDaisy/ ; make