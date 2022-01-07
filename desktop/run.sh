export `pkg-config wayland-client wayland-scanner wayland-egl wayland-cursor egl 'xkbcommon >= 0.5.0' --libs-only-L|sed 's|-L||g;s| |:|g;s|^|export LD_LIBRARY_PATH=|'`
./cp19	
