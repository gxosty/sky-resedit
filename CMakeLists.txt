cmake_minimum_required(VERSION 3.19.0)
set(CIPHER_URL https://github.com/lukas0x1/Userlib-SML/releases/latest/download/libciphered.so)
set(CIPHER_PATH ${CMAKE_SOURCE_DIR}/lib/libciphered.so)

if (NOT EXISTS ${CIPHER_PATH})
	message(STATUS "Downloading libciphered.so...")
	file(DOWNLOAD ${CIPHER_URL} ${CIPHER_PATH} STATUS LIBCIPHER_DOWNLOAD_STATUS)
	message(STATUS "Status: ${LIB_CIPHERED_DOWNLOAD_STATUS}")
endif()

### Here starts set up of cross-compile variables ###

if (WIN32)
	set(_SUFFIX_CMD ".cmd")
	set(_SUFFIX_EXE ".exe")
	set(_HOST_TAG "windows-x86_64")
else()
	# Assume the platform is Linux
	set(_SUFFIX_CMD "")
	set(_SUFFIX_EXE "")
	set(_HOST_TAG "linux-x86_64")
endif (WIN32)

set(CMAKE_SYSTEM_NAME Android)

# USER CONFIGURATION
set(HOST_TAG ${_HOST_TAG})													# The Host where we compile the lib										
# set(ANDROID_NDK_VERSION "26.3.11579264")											# Only needed if you are not using standalone NDK, otherwise you can put this empty
set(ANDROID_ARCH "aarch64")													# Android architecture
set(CMAKE_ANDROID_ARCH_ABI "arm64-v8a")												# Android ABI
set(CMAKE_ANDROID_API 34)													# Android API version
set(CMAKE_SYSTEM_VERSION ${CMAKE_ANDROID_API})											# This tells cmake the Android API we are targeting for
set(CMAKE_ANDROID_PLATFORM android-26)												# Min Android API
set(ANDROID_STL "c++_shared")													# CXX STL mode for lib
set(CMAKE_CXX_STANDARD 20)													# CXX Standard

cmake_path(SET ANDROID_SDK_PATH "$ENV{ANDROID_HOME}") # to escape backslashes on Windows
cmake_path(SET ANDROID_NDK_PATH "$ENV{ANDROID_NDK_HOME}")

if (ANDROID_NDK_PATH)
	message(STATUS "Found Android NDK Path")
	set(CMAKE_ANDROID_NDK ${ANDROID_NDK_PATH})										# Path to standalone NDK
elseif (ANDROID_SDK_PATH)
	message(STATUS "Found Android NDK path in Android SDK")
	set(CMAKE_ANDROID_NDK ${ANDROID_SDK_PATH}/ndk/${ANDROID_NDK_VERSION})			# Path to NDK (ANDROID_NDK_VERSION required)
else()
	message(SEND_ERROR "Couldn't find path to ndk. Please define path to Android NDK in your ANDROID_NDK_HOME system environment variable")
endif()

set(CMAKE_TOOLCHAIN ${CMAKE_ANDROID_NDK}/toolchains/llvm/prebuilt/${HOST_TAG})		# Toolchain path
set(CMAKE_ANDROID_TOOLCHAIN_PREFIX ${CMAKE_TOOLCHAIN}/bin)
set(CMAKE_SYSROOT ${CMAKE_TOOLCHAIN}/sysroot)										# Sysroot path

set(CMAKE_C_COMPILER ${CMAKE_TOOLCHAIN}/bin/${ANDROID_ARCH}-linux-android${CMAKE_ANDROID_API}-clang${_SUFFIX_CMD} CACHE PATH "" FORCE)
set(CMAKE_CXX_COMPILER ${CMAKE_TOOLCHAIN}/bin/${ANDROID_ARCH}-linux-android${CMAKE_ANDROID_API}-clang++${_SUFFIX_CMD} CACHE PATH "" FORCE)
set(CMAKE_AR ${CMAKE_TOOLCHAIN}/bin/llvm-ar${_SUFFIX_EXE})
set(CMAKE_AS ${CMAKE_TOOLCHAIN}/bin/llvm-as${_SUFFIX_EXE})
set(CMAKE_STRIP ${CMAKE_TOOLCHAIN}/bin/llvm-strip${_SUFFIX_EXE})
set(CMAKE_RANLIB ${CMAKE_TOOLCHAIN}/bin/llvm-ranlib${_SUFFIX_EXE})
set(CMAKE_OBJCOPY ${CMAKE_TOOLCHAIN}/bin/llvm-objcopy${_SUFFIX_EXE})
set(CMAKE_LINKER ${CMAKE_TOOLCHAIN}/bin/ld.lld${_SUFFIX_EXE})

set(CMAKE_LINKER_FLAGS "$CMAKE_LINKER_FLAGS -L${SYSROOT}/usr/lib/${ANDROID_ARCH}-linux-android/${CMAKE_ANDROID_API}")

#####################################################

project("userlib")

file(READ ${CMAKE_HOME_DIRECTORY}/config.json MY_JSON_STRING) #reads soname from config
string(JSON CUR_NAME GET ${MY_JSON_STRING} name)
string(LENGTH ${CUR_NAME} CUR_NAME_LEN)
string(SUBSTRING ${CUR_NAME} 3 ${CUR_NAME_LEN} CUR_NAME) #ugly but there's no lazy regex support
string(REPLACE ".so" "" CUR_NAME ${CUR_NAME})

# Append version name to the lib after everthing is done
string(JSON LIB_MAJOR_VERSION GET ${MY_JSON_STRING} majorVersion)
string(JSON LIB_MINOR_VERSION GET ${MY_JSON_STRING} minorVersion)
string(JSON LIB_PATCH_VERSION GET ${MY_JSON_STRING} patchVersion)
set(LIB_VERSION ${LIB_MAJOR_VERSION}.${LIB_MINOR_VERSION}.${LIB_PATCH_VERSION})
set(RENAMED_CUR_NAME lib${CUR_NAME}-${LIB_VERSION}.so)
add_compile_definitions(__LIB_VERSION__="${LIB_VERSION}") # You can use lib_version inside your sources

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(include/minizip)
add_compile_definitions(MODUI_EMBED_ICONS=1)

add_library( # Sets the name of the library.
	${CUR_NAME}
	SHARED

	resedit/asset_manager_hook.cpp
	resedit/main.cpp
	resedit/config.cpp
	resedit/resedit.cpp
	resedit/resedit_app.cpp
	resedit/resedit_io.cpp
	resedit/utils.cpp

	# Core components
	resedit/core/resource_pack_manager.cpp
	resedit/core/resource_pack.cpp
	resedit/core/append_edit.cpp
	resedit/core/replace_edit.cpp
	resedit/core/json_edit.cpp

	# Screens
	resedit/ui/main_screen.cpp

	# Widgets
	resedit/ui/widgets/resource_pack_card.cpp

	# ModUI
	include/stb/stb_image.cpp

	include/modui/animation/animation.cxx
	include/modui/animation/screen/screenanimation.cxx
	include/modui/animation/screen/fadescreenanimation.cxx
	include/modui/animation/theme/themetransitionanimation.cxx

	include/modui/core/image/image.cxx
	include/modui/core/image/rasterimage.cxx
	include/modui/core/image/selectableimage.cxx
	include/modui/core/image/vectoricon.cxx
	include/modui/core/image/vectorimage.cxx
	include/modui/core/style/theme.cxx
	include/modui/core/style/thememanager.cxx
	include/modui/core/common.cxx
	include/modui/core/exceptions.cxx

	include/modui/internal/internal_core.cxx
	include/modui/internal/internal_utils.cxx

	include/modui/ui/appbar.cxx
	include/modui/ui/basebutton.cxx
	include/modui/ui/baseslider.cxx
	include/modui/ui/button.cxx
	include/modui/ui/card.cxx
	include/modui/ui/checkbox.cxx
	include/modui/ui/divider.cxx
	include/modui/ui/iconbutton.cxx
	include/modui/ui/linearlayout.cxx
	include/modui/ui/listitem.cxx
	include/modui/ui/memorytext.cxx
	include/modui/ui/menu.cxx
	include/modui/ui/screen.cxx
	include/modui/ui/screenmanager.cxx
	include/modui/ui/scrolllayout.cxx
	include/modui/ui/slider.cxx
	include/modui/ui/switch.cxx
	include/modui/ui/text.cxx
	include/modui/ui/widget.cxx

	include/modui/icons.cxx
	include/modui/app.cxx
)

target_link_libraries(
	${CUR_NAME}
	ciphered
	log
	android

	EGL
	GLESv3

	dobby
	minizip
)

add_custom_command(
	TARGET ${CUR_NAME}
	POST_BUILD
	COMMAND ${CMAKE_OBJCOPY}
	ARGS --add-section .config=${CMAKE_HOME_DIRECTORY}/config.json --set-section-flags .config=readonly,contents,data $<TARGET_FILE:${CUR_NAME}>
)

# Add icon. EXCLUSIVELY .PNG FORMAT!!
add_custom_command(
	TARGET ${CUR_NAME}
	POST_BUILD
	COMMAND ${CMAKE_OBJCOPY}
	ARGS --add-section .icon=${CMAKE_HOME_DIRECTORY}/icon.png --set-section-flags .icon=readonly,contents,data $<TARGET_FILE:${CUR_NAME}>
)

add_custom_command(
	TARGET ${CUR_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E rename $<TARGET_FILE:${CUR_NAME}> ${CMAKE_BINARY_DIR}/${RENAMED_CUR_NAME}
)
