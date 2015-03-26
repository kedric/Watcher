#include "../inc/Watcher.class.hpp"
#include <regex>

bool is_header(std::string const& path)
{
	std::regex reg(".*(\\.[h|t]pp|\\.h)$");
	return (std::regex_match(path,reg));
};

void my_callback(std::string const &path){
	char buff[256];
	FILE *fd;

	if (is_header(path))
		fd = popen("make re", "r");
	else
		fd = popen("make", "r");
	while(fgets(buff, 256, fd) != NULL)
	{
		std::cout << buff;
	}
	pclose(fd);
}

int main(int ac, char **av){
	Watcher w;

	w.setExclude(".*(\\.out|\\.o|\\.a|\\.so)$");
	w.setRecursive(true);
	w.setCallback(my_callback);
	if (ac < 2){
		w.addPath(".");
	} else {
		for (int i=1; av[i] != NULL; i++) {
			w.addPath(av[i]);
		}
	}
	w.watch();
}