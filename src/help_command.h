#ifndef HELP__HPP
#define HELP__HPP
#include "bio_crt.h"
#include "command.h"

class WWIVDrawMain;

class HelpCommand : public Command
{
public:
  HelpCommand(WWIVDrawMain* win) : Command(win) {}
  void run();
};

#endif
