#ifndef VIEWMODE_HPP
#define VIEWMODE_HPP

#include<MysticDrawMain.hpp>

class ViewModeCommand : public Command
{
	private:
		void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
	public:
    ViewModeCommand(MysticDrawMain* win) : Command(win) {}
		void run();
};

#endif
