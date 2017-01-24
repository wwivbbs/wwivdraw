#ifndef TABULATORCOMMAND__HPP
#define TABULATORCOMMAND__HPP

#include "command.hpp"

class MysticDrawMain;

class TabulatorSetupCommand : public Command
{
	public:
    TabulatorSetupCommand(MysticDrawMain* win) : Command(win) {}
		void run();
};

extern int getPrevTab(int caretPosition);
extern int getNextTab(int caretPosition);

#endif
