#include <Arduino.h>

class Led {
  
  private:
    byte red_pin;
    byte blue_pin;
    byte green_pin;
    unsigned long int cur_time = 0;
    const byte START = 8;
    const byte INPROGRESS = 9;
    const byte END = 10;

    std::vector<byte> colour_queue;
    std::vector<byte> type_queue;
    std::vector<unsigned long int> register_time_queue;
    std::vector<byte> status_queue;
    std::vector<unsigned long int> interval_queue;

    void add_to_queues(byte type, byte colour, unsigned long int interval=0);
    void remove_from_queues(unsigned long int pos);
    byte blink(byte colour, unsigned long register_time, byte status, unsigned long int interval=0);
    void on(byte colour);
    void off(byte colour);
    void init();
    byte colour_to_pin(byte colour);

  public:
    static const byte RED = 1;
    static const byte BLUE = 2;
    static const byte GREEN = 4;

    static const byte BLINK = 5;
    static const byte ON = 6;
    static const byte OFF = 7;
    static const byte BLINK_TIME = 500;

    Led(byte red_pin, byte green_pin, byte blue_pin);
    void register_action(const char* type, const char* colour, unsigned long int interval=0);
    void update();
    void all_off();
    void all_on();
    void printall();
};