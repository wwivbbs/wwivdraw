#ifndef SELECTSAUCECOMMAND__HPP
#define SELECTSAUCECOMMAND__HPP

#include<bio_crt.h>
#include<command.hpp>
#include<sauce.hpp>

class MysticDrawMain;
class ScreenBuffer;

class SelectSauceCommand : Command
{
	public:
    SelectSauceCommand(MysticDrawMain* win);
		void run();
private:
  ScreenBuffer* screen_buffer_ = nullptr;
};

#endif
