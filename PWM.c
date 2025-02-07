#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22
#define LED_RGB_PIN 12
#define PWM_FREQ 50
#define CLOCK_FREQ 125000000
#define PWM_DIVIDER 64.0  // Reduz a frequência do clock para o PWM
#define PWM_WRAP (CLOCK_FREQ / (PWM_DIVIDER * PWM_FREQ))

void set_servo_angle(uint slice_num, uint channel, float pulse_us) {
    uint16_t level = (pulse_us / 20000.0) * PWM_WRAP;
    pwm_set_chan_level(slice_num, channel, level);
}

int main() {
    stdio_init_all();
    sleep_ms(500); // Pequeno atraso para estabilização

    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num_servo = pwm_gpio_to_slice_num(SERVO_PIN);
    uint channel_servo = pwm_gpio_to_channel(SERVO_PIN);

    pwm_set_clkdiv(slice_num_servo, PWM_DIVIDER);
    pwm_set_wrap(slice_num_servo, PWM_WRAP);
    pwm_set_enabled(slice_num_servo, true);

    gpio_set_function(LED_RGB_PIN, GPIO_FUNC_PWM);
    uint slice_num_led = pwm_gpio_to_slice_num(LED_RGB_PIN);
    uint channel_led = pwm_gpio_to_channel(LED_RGB_PIN);

    pwm_set_clkdiv(slice_num_led, PWM_DIVIDER);
    pwm_set_wrap(slice_num_led, PWM_WRAP);
    pwm_set_enabled(slice_num_led, true);

    // Começa em 270° (sul)
    set_servo_angle(slice_num_servo, channel_servo, 2400);
    pwm_set_chan_level(slice_num_led, channel_led, 2400 * PWM_WRAP / 20000);
    sleep_ms(5000);

    // Move para 0° (leste)
    set_servo_angle(slice_num_servo, channel_servo, 1500);
    pwm_set_chan_level(slice_num_led, channel_led, 1500 * PWM_WRAP / 20000);
    sleep_ms(5000);

    // Move para 90° (norte)
    set_servo_angle(slice_num_servo, channel_servo, 500);
    pwm_set_chan_level(slice_num_led, channel_led, 500 * PWM_WRAP / 20000);
    sleep_ms(5000);

    while (1) {
        // Movimento suave de 90° para 270° passando por 0°
        for (float pulse = 500; pulse <= 2400; pulse += 5) {
            set_servo_angle(slice_num_servo, channel_servo, pulse);
            pwm_set_chan_level(slice_num_led, channel_led, pulse * PWM_WRAP / 20000);
            sleep_ms(10);
        }
        // Movimento suave de 270° para 90° passando por 0°
        for (float pulse = 2400; pulse >= 500; pulse -= 5) {
            set_servo_angle(slice_num_servo, channel_servo, pulse);
            pwm_set_chan_level(slice_num_led, channel_led, pulse * PWM_WRAP / 20000);
            sleep_ms(10);
        }
    }
} 
