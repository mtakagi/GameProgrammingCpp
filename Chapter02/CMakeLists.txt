set(TARGET_NAME Chapter02)

set(SOURCES
    Game.cpp
    Actor.cpp
    Ship.cpp
    Component.cpp
    SpriteComponent.cpp
    AnimSpriteComponent.cpp
    BGSpriteComponent.cpp
    Math.cpp
    Main.cpp
)

add_executable(${TARGET_NAME} ${SOURCES})

target_link_libraries(${TARGET_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image)

if(WIN32)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_RUNTIME_DLLS:${TARGET_NAME}>" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
            COMMAND_EXPAND_LISTS)
endif()

set(ASSET_DIRS Assets)

foreach(DIR IN LISTS ASSET_DIRS)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}"
            "$<TARGET_FILE_DIR:${TARGET_NAME}>/${DIR}"
            COMMENT "Copying ${DIR} to output directory..."
    )
endforeach()