#ifndef FONTEDITORCOMMAND__HPP
#define FONTEDITORCOMMANDP__HPP
#include <bio_crt.h>
#include <command.hpp>

class MysticDrawMain;

class FontEditorCommand : public Command
{
	private:
		char cursorX;
		char cursorY;
		unsigned char currentSelectedChar;
		bool selectMode;
		
		void drawCharTable(int x, int y);
		void drawTileSet(int posX, int posY, int width, int height);
		void drawFontEditor(int posX, int posY);
		void cursorLeft();
		void cursorRight();
	public:
	
		FontEditorCommand(MysticDrawMain* win) : Command(win) {
			selectMode = true;
			cursorX = 0;
			cursorY = 0;
			currentSelectedChar = 0;
		}
		
		void run();
};

#endif
