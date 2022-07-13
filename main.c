#include "dht.h"
#include "lcd_1602_i2c.h"
#include "string.h"

int main(){
    // This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    lcd_init();

    stdio_init_all();
    gpio_init(DHT_PIN);
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

while(1){
    dht_reading reading;
    read_from_dht(&reading);

    char message[2][20];
    char temp[20], humi[20];
    sprintf(temp, "%.1f", reading.temp_celsius);
    sprintf(humi, "%.1f", reading.humidity);

    strncpy(message[0], temp, sizeof(temp));
    strncpy(message[1], humi, sizeof(humi));

    for (int m = 0; m < sizeof(message) / sizeof(message[0]); m += MAX_LINES) {
        for (int line = 0; line < MAX_LINES; line++) {
            lcd_set_cursor(line, (MAX_CHARS / 2) - strlen(message[m + line]) / 2);
            lcd_string(message[m + line]);
        }
        sleep_ms(2000);
        lcd_clear();
    }

}
    return 0;
}
