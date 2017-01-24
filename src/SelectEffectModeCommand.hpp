#ifndef EFFECTMODECOMMAND__HPP
#define EFFECTMODECOMMAND__HPP

#include<command.hpp>
#include "mdraw.h"

class MysticDrawMain;

class SelectEffectModeCommand : public Command
{
	private:
		void draweffekt(int effekt,char *blabla,int highlite);
		void changecolor(int Effekt);
	public:
    SelectEffectModeCommand(MysticDrawMain* win) : Command(win) {}
		void run();
};

#endif
