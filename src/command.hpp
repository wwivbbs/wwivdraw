#ifndef COMMAND__HPP
#define COMMAND__HPP

class MysticDrawMain;

class Command {
	public:
    Command(MysticDrawMain* main) : main_(main) {}
    virtual ~Command() {}

		virtual void run() = 0;
private:
  MysticDrawMain* main_ = nullptr;
};

#endif
