#ifndef ASCII_TABLE__HPP
#define ASCII_TABLE__HPP

#include<bio_crt.h>
#include<command.hpp>

class MysticDrawMain;

class ASCIITableCommand : public Command
{
	private:
		unsigned char show_ASCIITable();
	public:
    ASCIITableCommand(MysticDrawMain* win) : Command(win) {}
		void run();
};

#endif
