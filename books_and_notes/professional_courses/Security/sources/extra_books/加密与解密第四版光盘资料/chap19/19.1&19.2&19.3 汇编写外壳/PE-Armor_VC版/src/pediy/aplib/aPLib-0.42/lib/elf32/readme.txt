- To convert aplib.a into a dynamically linkable file, use:

  gcc -shared -nostdlib -Wl,--whole-archive aplib.a -o aplib.so
