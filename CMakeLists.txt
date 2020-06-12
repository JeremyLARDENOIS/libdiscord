cmake_minimum_required(VERSION 3.0)
project(libdiscord VERSION 0.3.0 LANGUAGES C)

message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION} system")
message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")

## Library info
set(PACKAGE libdiscord)
set(CPACK_PACKAGE_NAME libdiscord)

set(LD_VERSION_MAJOR 0)
set(LD_VERSION_MINOR .3)
set(LD_VERSION_PATCH )
set(LD_VERSION "${LD_VERSION_MAJOR}${LD_VERSION_MINOR}${LD_VERSION_PATCH}")

## libdiscord options
set(LD_RINGBUFFER_DEFAULT_SIZE 16)
set(LD_CONNECT_DELAY_INTERVAL 5)  # in ld_service, if ld_connect returns error, ld_service will block for this amount of time. not so good for event loops with other things happening in the background

## Discord-specific default options
set(LD_API_URL "\"https://discordapp.com/api\"")
set(LD_REST_API_VERSION "\"/v6\"")
set(LD_WS_API_VERSION 6)
set(LD_GITHUB_URL "\"https://github.com/dxing97/libdiscord\"") # do we need this?


## Compilation
set(CMAKE_C_STANDARD 99)

include_directories(/usr/local/include)
include_directories("${PROJECT_SOURCE_DIR}/include")

configure_file(
        "${PROJECT_SOURCE_DIR}/cmake/libdiscord_config.h.in"
        "${PROJECT_SOURCE_DIR}/include/libdiscord/libdiscord_config.h"
)


## Library


file( GLOB hdr   "include/libdiscord/*.h"  )
file( GLOB src   "src/*.c"  )



add_library(discord SHARED ${src} )

target_link_libraries(discord websockets jansson curl crypto )

set_target_properties(discord PROPERTIES VERSION ${PROJECT_VERSION})

target_compile_options(discord PRIVATE "-Wall")
target_compile_options(discord PRIVATE "-Werror")
target_compile_options(discord PRIVATE "-Wno-unknown-pragmas")
target_compile_options(discord PRIVATE "-g")

install( TARGETS "discord"
	    RUNTIME DESTINATION ${INSTALL_BIN_PATH}  COMPONENT main
	    LIBRARY DESTINATION ${INSTALL_LIB_PATH}  COMPONENT main
        ARCHIVE DESTINATION ${INSTALL_AR_PATH}   COMPONENT main )

install( FILES ${hdr}
	    DESTINATION ${INSTALL_HEADERS_PATH}/libdiscord/${name} 
        COMPONENT main )

## Example Bots
set(BOT_AYY_SOURCE example_bots/example-ayylmao.c cmake/libdiscord_config.h.in)
add_executable(example-bot-ayylmao ${BOT_AYY_SOURCE})
target_link_libraries(example-bot-ayylmao discord)

set(BOT_PING_SOURCE example_bots/example-ping.c cmake/libdiscord_config.h.in)
add_executable(example-bot-ping ${BOT_PING_SOURCE})
target_link_libraries(example-bot-ping discord)

set(BOT_COUNTER_SOURCE example_bots/example-counter.c cmake/libdiscord_config.h.in)
add_executable(example-bot-counter ${BOT_COUNTER_SOURCE})
target_link_libraries(example-bot-counter discord)

set(EXAMPLE_SIMPLEPOST_SOURCE cmake/libdiscord_config.h.in example_bots/example-simplepost.c)
add_executable(example-simplepost ${EXAMPLE_SIMPLEPOST_SOURCE})
target_link_libraries(example-simplepost discord)

set(BOT_HASH_SOURCE cmake/libdiscord_config.h.in example_bots/example-hash.c)
add_executable(example-bot-hash ${BOT_HASH_SOURCE})
target_link_libraries(example-bot-hash discord)

