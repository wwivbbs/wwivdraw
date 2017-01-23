#ifndef SELECTCOLORCOMMAND__HPP
#define SELECTCOLORCOMMAND__HPP

#include "MysticDrawMain.hpp"

class MysticDrawMain;

class SelectColorCommand : Command
{
	public:
    SelectColorCommand(MysticDrawMain* win) : Command(win) {}
		void run();
};

#endif
