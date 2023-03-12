cmake_minimum_required(VERSION "3.20")
set(CMAKE_TOOLCHAIN_FILE toolchain.cmake)

include(FetchContent)

set(TIVA_SDK_PATH $ENV{TIVA_SDK_PATH})

if(NOT DEFINED ENV{TIVA_SDK_PATH})
    message(FATAL_ERROR "You must set TIVA_SDK_PATH environment variable that points tiva sdk path")
endif()

set(OPENOCD_CONFIG_PATH $ENV{OPENOCD_CONFIG_PATH})

if(NOT DEFINED ENV{OPENOCD_CONFIG_PATH})
    message(WARN "You must set OPENOCD_CONFIG_PATH environment variable that points to the path conatining conifg file for your board")
endif()

project(tiva_freertos LANGUAGES C)

set(OPENOCD_BOARD dk-tm4c129)
set(LINKERSCRIPT_PATH ${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}.ld)

add_executable(
    ${CMAKE_PROJECT_NAME}
    startup_gcc.c
    board.c
    main.c
    uart_rtos.c
)

include_directories(
    ${TIVA_SDK_PATH}/
    include
)

FetchContent_Declare( freertos_kernel
    GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel.git
    GIT_TAG        V10.0.0
)

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config SYSTEM
INTERFACE
    include
)

set( FREERTOS_HEAP "4" CACHE STRING "" FORCE)
# Select the native compile PORT
set( FREERTOS_PORT "GCC_POSIX" CACHE STRING "" FORCE)
# Select the cross-compile PORT
if (CMAKE_CROSSCOMPILING)
  set(FREERTOS_PORT "GCC_ARM_M4F" CACHE STRING "" FORCE)
endif()

FetchContent_MakeAvailable(freertos_kernel)

add_custom_target(
    link_tiva_freertos
    COMMAND ${CMAKE_LINKER} -T ${LINKERSCRIPT_PATH}
        ${CMAKE_BINARY_DIR}/CMakeFiles/tiva_freertos.dir/startup_gcc.c.obj
        ${CMAKE_BINARY_DIR}/CMakeFiles/tiva_freertos.dir/board.c.obj
        ${CMAKE_BINARY_DIR}/CMakeFiles/tiva_freertos.dir/uart_rtos.c.obj
        ${CMAKE_BINARY_DIR}/CMakeFiles/tiva_freertos.dir/main.c.obj
        ${TIVA_SDK_PATH}/driverlib/gcc/libdriver.a
        -Map=${CMAKE_PROJECT_NAME}.map
        -o ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf
)

add_custom_target(
    flash_tiva_freertos
    COMMAND openocd -f ${OPENOCD_CONFIG_PATH}/${OPENOCD_BOARD}.cfg
        -c "program ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf verify reset exit"
)

add_custom_target(
    start_gdb_server
    COMMAND openocd -f ${OPENOCD_CONFIG_PATH}/${OPENOCD_BOARD}.cfg
)

add_custom_target(
    attach_to_gdb_server
    COMMAND arm-none-eabi-gdb ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf
)