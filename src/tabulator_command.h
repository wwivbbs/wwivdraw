#ifndef TABULATORCOMMAND__HPP
#define TABULATORCOMMAND__HPP

#include "command.h"

class WWIVDrawMain;

class TabulatorSetupCommand : public Command
{
public:
  TabulatorSetupCommand(WWIVDrawMain* win) : Command(win) {}
  void run();
};

extern int getPrevTab(int caretPosition);
extern int getNextTab(int caretPosition);

#endif
