#include "pico/stdlib.h"  
#include "hardware/pwm.h" 

// Definições de pinos e configurações PWM
#define SERVO_PIN 22  // Pino de controle do servo motor
#define LED_RGB_PIN 12  // Pino de controle do LED RGB
#define PWM_FREQ 50  // Frequência do PWM (50Hz para servos)
#define CLOCK_FREQ 125000000  // Frequência do clock da CPU (125 MHz)
#define PWM_DIVIDER 64.0  // Reduz a frequência do clock para PWM
#define PWM_WRAP (CLOCK_FREQ / (PWM_DIVIDER * PWM_FREQ))  // Valor de "wrap" do PWM para 50Hz

/**
 * @brief Define o ângulo do servo com base no pulso em microssegundos.
 *
 * @param slice_num Número do slice PWM usado pelo servo
 * @param channel Canal PWM do servo
 * @param pulse_us Largura do pulso em microssegundos (us)
 */
void set_servo_angle(uint slice_num, uint channel, float pulse_us) {
    uint16_t level = (pulse_us / 20000.0) * PWM_WRAP;  // Converte tempo de pulso para nível de PWM
    pwm_set_chan_level(slice_num, channel, level);  // Define o nível do PWM para o servo
}

int main() {
    stdio_init_all();  // Inicializa entrada e saída padrão
    sleep_ms(500);  // Pequeno atraso para estabilização do sistema

    // Configuração do pino do servo para saída PWM
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num_servo = pwm_gpio_to_slice_num(SERVO_PIN);  // Obtém o slice PWM do pino do servo
    uint channel_servo = pwm_gpio_to_channel(SERVO_PIN);  // Obtém o canal PWM do pino do servo

    pwm_set_clkdiv(slice_num_servo, PWM_DIVIDER);  // Define o divisor de clock do PWM
    pwm_set_wrap(slice_num_servo, PWM_WRAP);  // Define o valor de "wrap" do PWM
    pwm_set_enabled(slice_num_servo, true);  // Habilita o PWM no slice correspondente

    // Configuração do pino do LED RGB para saída PWM
    gpio_set_function(LED_RGB_PIN, GPIO_FUNC_PWM);
    uint slice_num_led = pwm_gpio_to_slice_num(LED_RGB_PIN);  // Obtém o slice PWM do pino do LED
    uint channel_led = pwm_gpio_to_channel(LED_RGB_PIN);  // Obtém o canal PWM do pino do LED

    pwm_set_clkdiv(slice_num_led, PWM_DIVIDER);  // Define o divisor de clock do PWM para o LED
    pwm_set_wrap(slice_num_led, PWM_WRAP);  // Define o valor de "wrap" do PWM para o LED
    pwm_set_enabled(slice_num_led, true);  // Habilita o PWM no LED

    // Define o servo para a posição inicial de 270° (sul) e ajusta o brilho do LED proporcionalmente
    set_servo_angle(slice_num_servo, channel_servo, 2400);
    pwm_set_chan_level(slice_num_led, channel_led, 2400 * PWM_WRAP / 20000);
    sleep_ms(5000);

    // Move o servo para 0° (leste) e ajusta o LED
    set_servo_angle(slice_num_servo, channel_servo, 1500);
    pwm_set_chan_level(slice_num_led, channel_led, 1500 * PWM_WRAP / 20000);
    sleep_ms(5000);

    // Move o servo para 90° (norte) e ajusta o LED
    set_servo_angle(slice_num_servo, channel_servo, 500);
    pwm_set_chan_level(slice_num_led, channel_led, 500 * PWM_WRAP / 20000);
    sleep_ms(5000);

    while (1) {
        // Movimento suave do servo de 90° para 270° passando por 0°
        for (float pulse = 500; pulse <= 2400; pulse += 5) {
            set_servo_angle(slice_num_servo, channel_servo, pulse);
            pwm_set_chan_level(slice_num_led, channel_led, pulse * PWM_WRAP / 20000);
            sleep_ms(10);  // Pequeno atraso para suavizar o movimento
        }

        // Movimento suave do servo de 270° para 90° passando por 0°
        for (float pulse = 2400; pulse >= 500; pulse -= 5) {
            set_servo_angle(slice_num_servo, channel_servo, pulse);
            pwm_set_chan_level(slice_num_led, channel_led, pulse * PWM_WRAP / 20000);
            sleep_ms(10);  // Pequeno atraso para suavizar o movimento
        }
    }
}
