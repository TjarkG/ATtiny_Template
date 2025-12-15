cmake_minimum_required(VERSION 3.24)
project(ATtiny_Template CXX)

set(MCU "attiny1617")
set(PORT "/dev/ttyUSB0")
set(F_CPU "20000000")
set(DFP "/usr/lib/gcc/avr/device-specs")

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/avr-g++)
set(CMAKE_OBJCOPY avr-objcopy)

set(CMAKE_C_FLAGS "-mmcu=${MCU} -B ${DFP}/gcc/dev/${MCU}/ -I ${DFP}/include/ -DF_CPU=${F_CPU} -Os")
set(CMAKE_C_LINK_FLAGS "-mmcu=${MCU}")

set(CMAKE_CXX_FLAGS "-mmcu=${MCU} -B ${DFP}/gcc/dev/${MCU}/ -I ${DFP}/include/ -DF_CPU=${F_CPU} -Os")
set(CMAKE_CXX_LINK_FLAGS "-mmcu=${MCU}")

file(GLOB_RECURSE SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC})

target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++23>
        $<$<COMPILE_LANGUAGE:C>:-std=c2x>
        -ffunction-sections
        -fdata-sections
        -fstack-usage
        -fno-exceptions
        -Wall
        -Wextra
        -Wpedantic
        -Wdouble-promotion
        -Wno-deprecated-enum-enum-conversion
        -Wno-missing-field-initializers
        -Os
)

target_link_options(${PROJECT_NAME} PRIVATE
        -Wl,--gc-sections
        -Wl,--print-memory-usage
)

# convert compiler output to intel hex and show size
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME} ${PROJECT_NAME}.hex --remove-section .eeprom
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME} ${PROJECT_NAME}_eeprom.hex
)

# compile executable and program it using pymcuprog
add_custom_target(program pymcuprog write -d ${MCU} -t uart -u ${PORT} -f ${PROJECT_NAME}.hex --erase DEPENDS ${PROJECT_NAME})

# same as program, but also writes eeprom
add_custom_target(program_eeprom pymcuprog write -d ${MCU} -t uart -u ${PORT} -f ${PROJECT_NAME}_eeprom.hex --erase DEPENDS ${PROJECT_NAME})
