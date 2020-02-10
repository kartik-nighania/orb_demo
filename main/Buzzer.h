#include <Arduino.h>
#include <queue>

class Buzzer {
  
  private:
    byte pin;
    const byte END = 9;
    const byte START = 10;
    const byte INPROGRESS = 11;
    static const byte SINGLE_BEEP = 1;
    static const byte DOUBLE_BEEP = 2;
    static const byte TRIPLE_BEEP = 3;
    // a single pulse of buzzer ON and OFF for 250 ms each.
    static const unsigned int BUZZ_TIME = 250;
    static const byte LONG_BEEP = 12; // BUZZ_TIME * 12 = 3 sec interval.
    boolean long_beep = false;

    unsigned long int cur_time = 0;
    unsigned long int register_time;
    boolean pulse_high = true;
    byte status = END;
    byte count;
    std::queue<unsigned long int> register_time_queue;
    std::queue<byte> status_queue;
    std::queue<byte> count_queue;

    void buzz();
    void add_to_queues(byte type);
    void remove_from_queues();
    void get_new_task();
    byte blink(byte colour, unsigned long register_time, byte status, unsigned long int interval=0);

  public:

    Buzzer(byte pin);
    void register_action(const char* type);
    void update();
};