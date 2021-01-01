add_executable(
	AnimeTest
	main.cpp
	buffer.cpp
	scene.cpp
	shader.cpp
	vertexArray.cpp
	window.cpp

#	gui/gui.cpp

	objects/camera.cpp
	objects/interfaces.cpp
	objects/light.cpp
	objects/model.cpp
	objects/renderer.cpp
	objects/timeline.cpp
)

target_include_directories(AnimeTest PRIVATE .)
target_link_libraries(AnimeTest ${linked_lib})

