#ifndef HELP__HPP
#define HELP__HPP
#include<bio_crt.h>
#include<command.hpp>

class MysticDrawMain;

class HelpCommand : public Command
{
	public:
    HelpCommand(MysticDrawMain* win) : Command(win) {}
		void run();
};

#endif
