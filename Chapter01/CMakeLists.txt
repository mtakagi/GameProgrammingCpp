set(TARGET_NAME Chapter01)

set(SOURCES
    game.cpp
    main.cpp
)

add_executable(${TARGET_NAME} ${SOURCES})

target_link_libraries(${TARGET_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)