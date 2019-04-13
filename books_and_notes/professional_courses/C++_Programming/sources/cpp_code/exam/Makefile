SOURCES=$(wildcard *.cpp)

.PHONY: default clean-all clean clean-makes

default: build

build:
	for X in *; do \
		if [ -d $$X ]; then \
			cd $$X; \
			if (ls *.cpp $&> /dev/null); then \
				if ! [ -e Makefile ]; then \
					ln -s ../Makefile.one Makefile; \
				fi; \
			else \
				if ! [ -e Makefile ]; then \
					ln -s ../Makefile Makefile; \
				fi; \
				if ! [ -e Makefile.one ]; then \
					ln -s ../Makefile.one Makefile.one; \
				fi; \
			fi; \
			$(MAKE); \
			cd ..; \
		fi; \
	done;

clean-all: clean clean-makes

clean-makes:
	for X in *; do \
		if [ -d $$X ]; then \
			cd $$X; \
			if [ -e Makefile.one ]; then \
				$(MAKE) clean-makes; \
				rm Makefile.one; \
			fi; \
			rm Makefile; \
			cd ..; \
		fi; \
	done;

clean:
	for X in *; do \
		if [ -d $$X ]; then \
			cd $$X; \
			$(MAKE) clean; \
			cd ..; \
		fi; \
	done;
