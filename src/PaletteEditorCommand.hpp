#ifndef PALETTEEDITORCOMMAND__HPP
#define PALETTEEDITORCOMMAND__HPP
#include<bio_crt.h>
#include<command.hpp>

class PaletteEditorCommand : public Command
{
	public:
    PaletteEditorCommand(MysticDrawMain* win) : Command(win) {}
		void run();
};

#endif
