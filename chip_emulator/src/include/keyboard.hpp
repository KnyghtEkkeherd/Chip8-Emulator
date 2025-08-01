#ifndef KEYBOARD_H
#define KEYBOARD_H

class Keyboard{
  private:
  const char* chip8_keys[16] = {
      "1", "2", "3", "C",
      "4", "5", "6", "D",
      "7", "8", "9", "E",
      "A", "0", "B", "F"
  };

  public:
  void handle_key_press(int key);
  void handle_key_release(int key);
};

#endif
