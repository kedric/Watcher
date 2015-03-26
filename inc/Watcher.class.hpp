#ifndef WATCHER_CLASS_HPP
#define WATCHER_CLASS_HPP

#include <cerrno>       // for errnos
#include <fcntl.h>       // for O_RDONLY
// #include <cstdio>       // for fprintf()
#include <list>
#include <cstdlib>      // for EXIT_SUCCESS
#include <string>      // for strerror()
#include <sys/event.h>   // for kqueue() etc.
#include <unistd.h>
#include <iostream>
#include <regex>

// for recurse
#include <dirent.h>
#include <sys/stat.h> 

struct watcherData {
	int				fd;
	std::string 	path;
	struct kevent	pathEvent;
};

class Watcher
{
private:
	int 						_kq;

	bool						_debug;
	bool						_recursive;

	struct kevent 				_sigEvent;

	std::list<std::string>		_entryPath;
	std::list<watcherData> 		_all;
	std::regex                  _exclude;
	void						(*_callback)(std::string const &path);
	
	void 						_init();
	void						_readPath();
	void						_addPath(std::string const &path);
	void						_initWatch();
	void						_addFile(std::string const &path);
	void						_addDirectory(std::string const &path);
	bool						_isBinary(int fd);

public:
			Watcher();
			Watcher(Watcher const &);
			~Watcher();
Watcher		&operator=(Watcher const &);

void		setRecursive(bool Recursive);
void		setCallback(void (*)(std::string const &));
void		addPath(std::string const &path);
void		watch();
void		setExclude(std::string const &regex);
};
#endif