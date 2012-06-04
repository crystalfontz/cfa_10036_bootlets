#ifndef INCLUDE_MXS_GPIO_H
#define INCLUDE_MXS_GPIO_H
extern void mxs_gpio_init(void);
extern int gpio_get_value(int gp);
extern void gpio_set_value(int gp, int value);
extern int gpio_direction_input(int gp);
extern int gpio_direction_output(int gp, int value);
extern int gpio_request(int gp, const char *label);
extern void gpio_toggle_value(int gp);

#endif //INCLUDE_MXS_GPIO_H
