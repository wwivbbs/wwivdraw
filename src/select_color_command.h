#ifndef SELECTCOLORCOMMAND__HPP
#define SELECTCOLORCOMMAND__HPP

#include "command.h"

class WWIVDrawMain;

class SelectColorCommand : Command
{
public:
  SelectColorCommand(WWIVDrawMain* win) : Command(win) {}
  void run();
};

#endif
