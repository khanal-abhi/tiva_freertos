cmake_minimum_required(VERSION "3.20")
add_compile_definitions(gcc)

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
set(FREERTOS_SRC  ${CMAKE_BINARY_DIR}/_deps/freertos_kernel-src)
set(CMAKE_TOOLCHAIN_FILE toolchain.cmake)


add_executable(
    ${CMAKE_PROJECT_NAME}.elf
    startup_gcc.c
    board.c
    main.c
    uart_rtos.c
    freertos.c
    ${FREERTOS_SRC}/FreeRTOS/Source/croutine.c
    ${FREERTOS_SRC}/FreeRTOS/Source/queue.c
    ${FREERTOS_SRC}/FreeRTOS/Source/event_groups.c
    ${FREERTOS_SRC}/FreeRTOS/Source/list.c
    ${FREERTOS_SRC}/FreeRTOS/Source/stream_buffer.c
    ${FREERTOS_SRC}/FreeRTOS/Source/tasks.c
    ${FREERTOS_SRC}/FreeRTOS/Source/timers.c
    ${FREERTOS_SRC}/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    ${FREERTOS_SRC}/FreeRTOS/Source/portable/MemMang/heap_4.c
)

target_link_libraries(
    ${CMAKE_PROJECT_NAME}.elf
    ${TIVA_SDK_PATH}/driverlib/gcc/libdriver.a
)

include_directories(
    ${TIVA_SDK_PATH}/
    include
    ${FREERTOS_SRC}/FreeRTOS/Source/include
    ${FREERTOS_SRC}/FreeRTOS/Source/portable/GCC/ARM_CM4F
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