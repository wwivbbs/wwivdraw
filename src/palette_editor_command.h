#ifndef PALETTEEDITORCOMMAND__HPP
#define PALETTEEDITORCOMMAND__HPP
#include "bio_crt.h"
#include "command.h"

class PaletteEditorCommand : public Command
{
public:
  PaletteEditorCommand(WWIVDrawMain* win) : Command(win) {}
  void run();
};

#endif
