#include "../inc/Watcher.class.hpp"
Watcher::Watcher() : _debug(false), _recursive(false) {
	_init();
};

Watcher::Watcher(Watcher const &) : _debug(false), _recursive(false) {
	_init();
};

Watcher::~Watcher(){
	close(_kq);
};

Watcher		&Watcher::operator=(Watcher const &) {
	return(*this);
};

bool	Watcher::_isBinary(int fd){
	int buff;
	read(fd, &buff, 1);
	if(!((buff >= 0) && (buff < 128))){
		return false;
	}
	return true;
};

void		Watcher::setRecursive(bool recursive) {
	_recursive = recursive;
};

void		Watcher::_addFile(std::string const &path) {
	if (std::regex_match(path, _exclude))
		return;
	_addPath(path);
};

void		Watcher::_readPath() {
    struct stat st;
	
	for(auto path : _entryPath){
		if (stat(path.c_str(), &st) == -1){
    		printf("impossible de recuperer les info %s\n", path.c_str());
    		return;
    	}
    	if (st.st_mode & S_IFDIR){
    		_addDirectory(path);
    	} else {
    		_addFile(path);
    	}
	}

};

void		Watcher::_addDirectory(std::string const &path) {
	if (!_recursive) {
		_addPath(path);
	} else {
	    DIR *dir;
		struct dirent *ent;
		dir = opendir(path.c_str());
		std::string tmp;
		while ((ent = readdir(dir)) != NULL){
			tmp = ent->d_name;
			if (tmp[0] != '.')
				addPath(path + "/" + ent->d_name);
		}
	}
};

void		Watcher::_addPath(std::string const &path){
	watcherData tmp;
	tmp.path = path;
	tmp.fd = open(tmp.path.c_str(), O_RDONLY);
	if (_isBinary(tmp.fd)){
		close(tmp.fd);
		return;
	};
	_all.push_back(tmp);
	EV_SET(&_all.back().pathEvent, _all.back().fd, EVFILT_VNODE, EV_ADD | EV_CLEAR | EV_ENABLE,
            NOTE_WRITE, 0, (void *)_all.back().path.c_str());
	// if (_debug)
		printf("add Path : %s\n", _all.back().path.c_str());
};

void		Watcher::addPath(std::string const &path)
{
    _entryPath.push_back(path);
};

void Watcher::_initWatch() {
	EV_SET(&_sigEvent, SIGINT, EVFILT_SIGNAL, EV_ADD | EV_ENABLE, 0, 0, NULL);
    signal(SIGINT, SIG_IGN);
    kevent(_kq, &_sigEvent, 1, NULL, 0, NULL);
	for(auto toWatch : _all){
		kevent(_kq, &toWatch.pathEvent, 1, NULL, 0, NULL);
	}
};

void Watcher::watch() {
	_readPath();
	_initWatch();
	struct kevent change;
	while(42){
		if (kevent(_kq, NULL, 0, &change, 1, NULL) == -1) { exit(1); }
		if (change.udata == NULL) {
	        break;
	    } else {
            printf("file Modified : %s\n", (char*)change.udata);
	    	if (_callback) {
            	_callback(std::string((char*)change.udata));
	    	}
	    }
    }
};

void		Watcher::setCallback(void (*callback)(std::string const &))
{
	_callback = callback;
};
void		Watcher::setExclude(std::string const &regex){
	_exclude = regex;
};
void		Watcher::_init(){
	_kq = kqueue ();
};