add_executable(
        AnimeTest
        buffer.cpp
        scene.cpp
        shader.cpp vertexArray.cpp window.cpp)


target_include_directories(
        AnimeTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
        AnimeTest
        glfw
        assimp
        glm
        glad
        stb
)
#camera.cpp interfaces.cpp light.cpp model.cpp renderer.cpp timeline.cpp