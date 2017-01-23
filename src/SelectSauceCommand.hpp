#ifndef SELECTSAUCECOMMAND__HPP
#define SELECTSAUCECOMMAND__HPP

#include<bio_crt.h>
#include<command.hpp>
#include<sauce.hpp>
#include<MysticDrawMain.hpp>

class MysticDrawMain;

class SelectSauceCommand : Command
{
	public:
    SelectSauceCommand(MysticDrawMain* win) : Command(win) {}
		void run();
};

#endif
