#include <stdint.h>

class GPIOPin {
  private:
    // Private variables
    uint32_t port_base, pin_mask, type, dir, drive;

  public:
    // Public variables

    // Constructors
    GPIOPin(uint32_t _port, uint32_t _pin);

    // Public methods
    void configure(uint32_t _type, uint32_t _dir, uint32_t _drive);
    void set_direction(uint32_t _dir);
    void set_type(uint32_t _type);
    void set_drive_strength(uint32_t _drive);
    void write(uint32_t x);
    void toggle(void);
    uint32_t read(void);
    void attach_callback(uint32_t event, void(*callback)(void));
};
