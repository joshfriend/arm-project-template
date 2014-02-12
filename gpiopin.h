#include <stdint.h>

typedef enum {
    GPIO_PIN_DIR_IN = 0,
    GPIO_PIN_DIR_OUT,
    GPIO_PIN_DIR_TOTAL
} gpio_pin_dir_t;

typedef enum {
    GPIO_PIN_MODE_STD = 0,
    GPIO_PIN_MODE_STD_WPU,
    GPIO_PIN_MODE_STD_WPD,
    GPIO_PIN_MODE_OD,
    GPIO_PIN_MODE_OD_WPU,
    GPIO_PIN_MODE_OD_WPD,
    GPIO_PIN_MODE_ANALOG,
    GPIO_PIN_MODE_TOTAL,
} gpio_pin_mode_t;

typedef enum {
    GPIO_PIN_DRIVE_2MA = 0,
    GPIO_PIN_DRIVE_4MA,
    GPIO_PIN_DRIVE_8MA,
    GPIO_PIN_DRIVE_8MA_SC,
    GPIO_PIN_DRIVE_TOTAL
} gpio_pin_drive_t;

typedef enum {
    GPIO_PIN_INT_NONE = 0,
    GPIO_PIN_INT_LOW,
    GPIO_PIN_INT_HIGH,
    GPIO_PIN_INT_RISING,
    GPIO_PIN_INT_FALLING,
    GPIO_PIN_INT_BOTH,
    GPIO_PIN_INT_TOTAL
} gpio_pin_int_type_t;

typedef struct {
    gpio_pin_dir_t      dir;
    gpio_pin_mode_t     mode;
    gpio_pin_drive_t    drive;
    gpio_pin_int_type_t int_type;
} gpio_pin_cfg_t;

typedef void (*gpio_pin_int_cb_t)(void);

class GPIOPin {
  private:
    // Private variables
    uint32_t port_base, pin_mask;
    uint32_t port_num, pin_num;
    gpio_pin_cfg_t config;

  public:
    // Public variables

    // Constructors
    GPIOPin(uint32_t _port, uint32_t _pin);

    // Public methods
    void configure(gpio_pin_cfg_t *cfg);
    void set_direction(gpio_pin_dir_t dir);
    void set_mode(gpio_pin_mode_t mode);
    void set_drive_strength(gpio_pin_drive_t drive);
    void write(uint32_t x);
    void toggle(void);
    uint32_t read(void);
    void attach_callback(gpio_pin_int_type_t event, void(*callback)(void));
    void detach_callback(void);
};
