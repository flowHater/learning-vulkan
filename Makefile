CFLAGS = -std=c++17 -O2 --all-warnings -Werror -Wall

LDFLAGS = -ldl -lpthread

LibsFlags = `pkg-config --libs glfw3 vulkan`

VulkanEngine: *.cpp *.hpp
	echo $(LibsFlags) 
	clang++ $(CFLAGS) -o VulkanEngine *.cpp $(LibsFlags) $(LDFLAGS)


re: clean fclean VulkanEngine

fclean: clean
	rm -f VulkanEngine

clean: