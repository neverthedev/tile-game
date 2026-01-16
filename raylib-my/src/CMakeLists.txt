file(GLOB_RECURSE C_FILES CONFIGURE_DEPENDS *.c)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS *.h *.hpp)

# Only compile game.cpp directly, all other .cpp files are header-like and should be included
set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/game.cpp ${C_FILES})

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})
