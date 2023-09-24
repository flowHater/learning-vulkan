NAME=VulkanEngine

CFLAGS = -std=c++17 -O2 --all-warnings -Werror -Wall
SRCFLAGS = -Isrc
VENDORSFLAGS = -Ivendors/tiny_obj_loader
IMGUIFLAGS = -Iimgui -Iimgui/backends -Iimgui/misc/cpp


LDFLAGS = -ldl -lpthread
LibsFlags = `pkg-config --libs glfw3 vulkan`

compiler=clang++

frag_shaders = shaders/simple_shader.frag \
	shaders/point_light.frag
vert_shaders = shaders/simple_shader.vert \
	shaders/point_light.vert


compiled_frag_shaders = $(frag_shaders:.frag=.frag.spv)
compiled_vert_shaders = $(vert_shaders:.vert=.vert.spv)

src = src/app.cpp \
	src/lve_device.cpp \
	src/lve_model.cpp \
	src/lve_pipeline.cpp \
	src/lve_swap_chain.cpp \
	src/lve_window.cpp \
	src/lve_renderer.cpp \
	src/systems/simple_render_system.cpp \
	src/systems/point_light_system.cpp \
	src/lve_camera.cpp \
	src/lve_game_object.cpp \
	src/lve_buffer.cpp \
	src/lve_descriptors.cpp\
	src/gui.cpp\
	src/keyboard_movement_controller.cpp \
	src/main.cpp

obj = $(src:.cpp=.o)

imguiSrc = imgui/imgui.cpp \
imgui/imgui_demo.cpp \
imgui/imgui_draw.cpp \
imgui/imgui_tables.cpp \
imgui/imgui_widgets.cpp \
imgui/misc/cpp/imgui_stdlib.cpp \
imgui/backends/imgui_impl_vulkan.cpp \
imgui/backends/imgui_impl_glfw.cpp

imguiObj = $(imguiSrc:.cpp=.o)

all: $(NAME) $(compiled_frag_shaders) $(compiled_vert_shaders)

$(NAME): $(obj) $(imguiObj)
	$(compiler) $(CFLAGS) $(SRCFLAGS) $(VENDORSFLAGS) $(IMGUIFLAGS) -o $@ $^ $(LibsFlags) $(LDFLAGS)

%.o: %.cpp
	$(compiler) $(CFLAGS) $(SRCFLAGS) $(VENDORSFLAGS) $(IMGUIFLAGS) -c -o $@ $<

%.frag.spv: %.frag
	glslc $< -o $@

%.vert.spv: %.vert
	glslc $< -o $@ 

re: fclean all

fclean: clean
	rm -f VulkanEngine

clean:
	rm -f $(obj)
	rm -f $(compiled_frag_shaders)
	rm -f $(compiled_vert_shaders)

.PHONY: all $(NAME) clean fclean re 