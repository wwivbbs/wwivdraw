#ifndef COMMAND__HPP
#define COMMAND__HPP

class WWIVDrawMain;
class ScreenBuffer;
class Caret;

class Command {
public:
  Command(WWIVDrawMain* main);
  virtual ~Command() {}

  virtual void run() = 0;

protected:
  WWIVDrawMain& draw_main() { return *main_; }
  ScreenBuffer& screen_buffer() { return *screen_buffer_; }
  Caret& caret() { return *caret_; }

private:
  WWIVDrawMain* main_ = nullptr;
  ScreenBuffer* screen_buffer_ = nullptr;
  Caret* caret_ = nullptr;
};

#endif
