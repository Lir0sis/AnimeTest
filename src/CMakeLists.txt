add_executable(
	AnimeTest
	main.cpp
	buffer.cpp
	scene.cpp
	shader.cpp
	vertexArray.cpp
	window.cpp

	gui/gui.cpp

	objects/camera.cpp
	objects/interfaces.cpp
	objects/light.cpp
	objects/model.cpp
	objects/renderer.cpp
	objects/timeline.cpp

	imgui/imgui.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
	imgui/backends/imgui_impl_glfw.cpp
	imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(AnimeTest PRIVATE . imgui)
target_link_libraries(AnimeTest ${linked_lib})

set_property(TARGET AnimeTest PROPERTY CXX_STANDARD 20)
