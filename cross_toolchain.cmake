SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)

SET(CROSS_TOOLCHAIN_ROOT 
"/home/student/pputvios1/toolchain/marvell/armv5-marvell-linux-gnueabi-softfp/bin")

SET(CROSS_ROOTFS 
"/home/student/pputvios1/toolchain/marvell/marvell-sdk-1046/rootfs")

SET(DIRECTFB_PATH "${CROSS_ROOTFS}/home/galois/lib/directfb-1.4-6-libs/")

SET(CMAKE_FIND_ROOT_PATH 
	${CROSS_ROOTFS}
	${DIRECTFB_PATH}
)

SET(CMAKE_C_COMPILER "${CROSS_TOOLCHAIN_ROOT}/arm-marvell-linux-gnueabi-gcc")

include_directories("${CROSS_ROOTFS}/usr/include/")
include_directories("${CROSS_ROOTFS}/usr/include/directfb")
link_directories(${DIRECTFB_PATH})

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

SET(CMAKRE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
