cmake_minimum_required(VERSION "3.20")
set(CMAKE_TOOLCHAIN_FILE toolchain.cmake)

set(TIVA_SDK_PATH $ENV{TIVA_SDK_PATH})
set(OPENOCD_CONFIG_PATH $ENV{OPENOCD_CONFIG_PATH})
set(OPENOCD_BOARD dk-tm4c129)
set(LINKERSCRIPT_PATH ${CMAKE_SOURCE_DIR}/linkerscript.ld)
set(FREERTOS_KERNEL_PATH)

project(tiva_freertos LANGUAGES C)

add_executable(
    ${CMAKE_PROJECT_NAME}
    startup_gcc.c
    main.c
)

add_custom_target(
    link_tiva_freertos
    COMMAND ${CMAKE_LINKER} -T ${LINKERSCRIPT_PATH}
        ${CMAKE_BINARY_DIR}/CMakeFiles/tiva_freertos.dir/startup_gcc.c.obj
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

include_directories(
    ${TIVA_SDK_PATH}
)