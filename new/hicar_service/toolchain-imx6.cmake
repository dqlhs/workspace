# CMake system name must be something like "Linux".
# This is important for cross-compiling.
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(ENV{PKG_CONFIG_LIBDIR} /opt/02_imx6/3.14.52-1.1.1/sysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi/usr/lib/pkgconfig)
set(ENV{PKG_CONFIG_SYSROOT_DIR} /opt/02_imx6/3.14.52-1.1.1/sysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi)
set(SYSROOT_DIR /opt/02_imx6/3.14.52-1.1.1/sysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi)
set(CMAKE_C_COMPILER /opt/02_imx6/3.14.52-1.1.1/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER /opt/02_imx6/3.14.52-1.1.1/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-g++)
set(CMAKE_ASM_COMPILER /opt/02_imx6/3.14.52-1.1.1/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-gcc)
set(CMAKE_AR /opt/02_imx6/3.14.52-1.1.1/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-ar)
set(CMAKE_C_FLAGS " -march=armv7-a -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9  --sysroot=/opt/02_imx6/3.14.52-1.1.1/sysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi   -DSAL_PLATFORM_IMX6  -O2 -pipe -g -feliminate-unused-debug-types")
set(CMAKE_CXX_FLAGS " -march=armv7-a -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9  --sysroot=/opt/02_imx6/3.14.52-1.1.1/sysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi   -DSAL_PLATFORM_IMX6  -O2 -pipe -g -feliminate-unused-debug-types   -fvisibility-inlines-hidden")
set(CMAKE_ASM_FLAGS " -march=armv7-a -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9  --sysroot=/opt/02_imx6/3.14.52-1.1.1/sysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi   -DSAL_PLATFORM_IMX6  -O2 -pipe -g -feliminate-unused-debug-types ")
set(CMAKE_C_FLAGS_RELEASE "-O2 -pipe -g -feliminate-unused-debug-types   -DSAL_PLATFORM_IMX6  -O2 -pipe -g -feliminate-unused-debug-types -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -pipe -g -feliminate-unused-debug-types   -DSAL_PLATFORM_IMX6  -O2 -pipe -g -feliminate-unused-debug-types -fvisibility-inlines-hidden -DNDEBUG")
set(CMAKE_ASM_FLAGS_RELEASE "-O2 -pipe -g -feliminate-unused-debug-types   -DSAL_PLATFORM_IMX6  -O2 -pipe -g -feliminate-unused-debug-types -DNDEBUG")
set(CMAKE_C_LINK_FLAGS " -march=armv7-a -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9  --sysroot=/opt/02_imx6/3.14.52-1.1.1/sysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi   -DSAL_PLATFORM_IMX6  -Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed")
set(CMAKE_CXX_LINK_FLAGS " -march=armv7-a -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9  --sysroot=/opt/02_imx6/3.14.52-1.1.1/sysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi   -DSAL_PLATFORM_IMX6  -O2 -pipe -g -feliminate-unused-debug-types -fvisibility-inlines-hidden -Wl,-O1 -Wl,--hash-style=gnu -Wl,--as-needed")

# only search in the paths provided so cmake doesnt pick
# up libraries and tools from the native build machine
set(CMAKE_FIND_ROOT_PATH /opt/02_imx6/3.14.52-1.1.1/sysroots/x86_64-pokysdk-linux /opt/02_imx6/3.14.52-1.1.1/sysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Use qt.conf settings
set(ENV{QT_CONF_PATH} /opt/02_imx6/3.14.52-1.1.1/sysroots/x86_64-pokysdk-linux/usr/bin/qt5/qt.conf)

# We need to set the rpath to the correct directory as cmake does not provide any
# directory as rpath by default
set(CMAKE_INSTALL_RPATH)

# Use native cmake modules
set(CMAKE_MODULE_PATH /opt/02_imx6/3.14.52-1.1.1/sysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi/usr/share/cmake/Modules/)

# add for non /usr/lib libdir, e.g. /usr/lib64
set(CMAKE_LIBRARY_PATH /usr/lib /lib)

