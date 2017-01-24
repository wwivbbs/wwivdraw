#ifndef ASCII_TABLE__HPP
#define ASCII_TABLE__HPP

#include "bio_crt.h"
#include "command.h"

class WWIVDrawMain;

class ASCIITableCommand : public Command
{
private:
  unsigned char show_ASCIITable();
public:
  ASCIITableCommand(WWIVDrawMain* win) : Command(win) {}
  void run();
};

#endif
