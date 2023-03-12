cmake_minimum_required(VERSION "3.20")
set(CMAKE_TOOLCHAIN_FILE toolchain.cmake)

project(tiva_freertos LANGUAGES C)

add_executable(
    tiva_freertos
    startup_gcc.c
    main.c 
)

add_custom_target(
    link_tiva_freertos
    COMMAND ${CMAKE_LINKER} -T ${CMAKE_SOURCE_DIR}/linkerscript.ld
        ${CMAKE_BINARY_DIR}/CMakeFiles/tiva_freertos.dir/startup_gcc.c.obj
        ${CMAKE_BINARY_DIR}/CMakeFiles/tiva_freertos.dir/main.c.obj
        -o ${CMAKE_BINARY_DIR}/tiva_freertos.elf
)

add_custom_target(
    flash_tiva_freertos
    COMMAND openocd -f $ENV{OPENOCD_CONFIG_PATH}/dk-tm4c129.cfg
        -c "program ${CMAKE_BINARY_DIR}/tiva_freertos.elf verify reset exit"
)

include_directories(
    $ENV{TIVA_SDK_ROOT}
)