NAME=VulkanEngine

CFLAGS = -std=c++17 -O2 --all-warnings -Werror -Wall
LDFLAGS = -ldl -lpthread

LibsFlags = `pkg-config --libs glfw3 vulkan`

compiler=clang++

frag_shaders = shaders/simple_shader.frag
vert_shaders = shaders/simple_shader.vert

compiled_frag_shaders = $(frag_shaders:.frag=.frag.spv)
compiled_vert_shaders = $(vert_shaders:.vert=.vert.spv)

src = src/app.cpp \
	src/lve_device.cpp \
	src/lve_model.cpp \
	src/lve_pipeline.cpp \
	src/lve_swap_chain.cpp \
	src/lve_window.cpp \
	src/lve_renderer.cpp \
	src/simple_render_system.cpp \
	src/lve_camera.cpp \
	src/main.cpp

obj = $(src:.cpp=.o)

all: $(compiled_frag_shaders) $(compiled_vert_shaders) $(NAME)

$(NAME): $(obj)
	$(compiler) $(CFLAGS) -o $@ $^ $(LibsFlags) $(LDFLAGS)

%.o: %.cpp
	$(compiler) $(CFLAGS) -c -o $@ $<

%.frag.spv: %.frag
	glslc $< -o $@

%.vert.spv: %.vert
	glslc $< -o $@ 

re: clean fclean all

fclean: clean
	rm -f VulkanEngine

clean:
	rm -f $(obj)
	rm -f $(compiled_frag_shaders)
	rm -f $(compiled_vert_shaders)

.PHONY: all $(NAME) clean fclean re