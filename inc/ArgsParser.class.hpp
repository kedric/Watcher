
#ifndef ARGSPARSER_CLASS_HPP
# define ARGSPARSER_CLASS_HPP

# include <string>
# include <iostream>

class ArgsParser {
private:
	char 	**_av;
	int		_ac;
	void	_parse();
public:
	ArgsParser();
	ArgsParser(int ac, char **av);
	~ArgsParser();
};


#endif
