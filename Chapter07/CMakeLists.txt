# FMOD は再配布できないため CI のランナーには存在しない。
# 無い環境ではこの章をビルド対象から外し、他の章の検証は続けられるようにする。
if(NOT FMOD_FOUND)
    message(STATUS "Chapter07: FMOD が見つからないためスキップします")

    return()
endif()

add_chapter(Chapter07
        SOURCES
        Game.cpp
        Actor.cpp
        CameraActor.cpp
        PlaneActor.cpp
        Component.cpp
        AudioComponent.cpp
        SpriteComponent.cpp
        MeshComponent.cpp
        MoveComponent.cpp
        Math.cpp
        Mesh.cpp
        Shader.cpp
        Texture.cpp
        Renderer.cpp
        VertexArray.cpp
        AudioSystem.cpp
        SoundEvent.cpp
        Main.cpp
        LIBS
        GLEW::GLEW
        SOIL::SOIL
        RapidJSON::RapidJSON
        FMOD::Studio
        ASSET_DIRS
        Assets
        Shaders
)
