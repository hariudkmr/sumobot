/* ========================================================================= */
/**
 * @file gpio.c
 * @author Hari Udayakumar
 * @date 06-07-2024
 */
/* ========================================================================= */

/** @defgroup GPIO gpio.c
 * The GPIO Modules handles all the initialization of the IO Pins, configures P13.7 USER LED
 * which toggles every second. This Led acts as the Heart Beat of the System
 * @{
 */

#include "gpio.h"

#include "FreeRTOS.h"
#include "task.h"

uint32_t delay = 0;

st_gpioconfig gpio_configuration_array[TOTAL_IO_PINS] = {
#if PSOC62 || PSOCWB
    { GPIO_PRT13, P13_7_PIN, CY_GPIO_DM_STRONG_IN_OFF, HSIOM_SEL_GPIO },
    { GPIO_PRT5, P5_1_PIN, CY_GPIO_DM_STRONG_IN_OFF, P5_1_SCB5_UART_TX },
    { GPIO_PRT5, P5_0_PIN, CY_GPIO_DM_HIGHZ, P5_0_SCB5_UART_RX },

#endif

#if PSOCS3
    { GPIO_PRT11, P11_1_PIN, CY_GPIO_DM_STRONG_IN_OFF, HSIOM_SEL_GPIO },
    { GPIO_PRT10, P10_1_PIN, CY_GPIO_DM_STRONG_IN_OFF, P10_1_SCB1_UART_TX },
    { GPIO_PRT10, P10_0_PIN, CY_GPIO_DM_HIGHZ, P10_0_SCB1_UART_RX },

#endif
};

/*! \fn     GPIO Initialization
    \brief  Configures all the GPIO Pins specfied using GPIO_PIN's. Configurations of the each Pins
   is stored separaately in a structure array ( st_gpioconfig ). \return no return value
*/
void gpio_init()
{

    cy_stc_gpio_pin_config_t pinCfg = { 0 };

    for (int i = 0; i < TOTAL_IO_PINS; i++) {

        pinCfg.driveMode = gpio_configuration_array[i].drive_mode;
        pinCfg.hsiom = gpio_configuration_array[i].hsio;
        Cy_GPIO_Pin_Init(gpio_configuration_array[i].port, gpio_configuration_array[i].pin,
                         &pinCfg);
    }
}

/*! \fn     GPIO Write
    \brief  Controls the GPIO State using the below functions.
    \param  gpio_pin pin -> Index of the GPIO Configuration in GPIO
    \param  gpio_state state -> State of the GPIO pin
    \return no return value
*/
void gpio_write(gpio_pin pin, gpio_state state)
{
    if (state == IO_LOW) {
        Cy_GPIO_Set(gpio_configuration_array[pin].port, gpio_configuration_array[pin].pin);
    } else if (state == IO_HIGH) {
        Cy_GPIO_Clr(gpio_configuration_array[pin].port, gpio_configuration_array[pin].pin);
    }
}

/*! \fn     GPIO Read
    \brief  Reads the state of the GPIO PIN using the below functions.
    \param  gpio_pin pin -> Index of the GPIO Configuration in st_gpioconfig structure array
    \return -> State of the GPIO pin
*/
gpio_state gpio_read(gpio_pin pin)
{
    return IO_LOW;
}

/*! \fn     GPIO Toggle
    \brief  Toggles the stato of GPIO pin using the below functions.
    \param  gpio_pin pin -> Index of the GPIO Configuration in GPIO
    \return no return value
*/
void gpio_toggle(gpio_pin pin)
{
    Cy_GPIO_Inv(gpio_configuration_array[pin].port, gpio_configuration_array[pin].pin);
}

/*! \fn     user_led_gpiotask(void *arg)
    \brief  Toggle User Led using RTOS Task.
*/
void user_led_gpiotask(void *arg)
{
    (void)arg;

    for (;;) {
        /* Toggle the LED periodically */
        gpio_toggle(USER_LED);
        vTaskDelay(500);
    }
}
