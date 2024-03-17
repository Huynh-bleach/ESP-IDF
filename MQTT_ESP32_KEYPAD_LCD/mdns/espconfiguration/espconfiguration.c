#include "espconfiguration.h"

Keypad myKeypad = (Keypad){
    .row_pins = {13, 12, 14, 27},
    .col_pins = {26, 25, 33, 32},
    .keys = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}}};

lcd_handle_t lcd_handle = LCD_HANDLE_DEFAULT_CONFIG();

int save = 0;
int loca = 18;
int speedFAN = 30;
int brightnessLED = 50;
int speedPUMP = 10;
char tam;
char luutrutam[20];
int KT;
int selection = 0;

char inLED[10];
char inFAN[10];
char inPUMP[10];

char IPaddress[20];

unsigned char onOffled = 0;
unsigned char onOfffan = 0;
unsigned char onOffpump = 0;

void lcd_home_keypad()
{
    ESP_LOGI(TAGLCD, "Dang o task LCD_home_keypad");
    lcd_clear_screen(&lcd_handle);
    lcd_backlight(&lcd_handle);
    lcd_set_cursor(&lcd_handle, 0, 0);
    lcd_write_str(&lcd_handle, "GIAM SAT NONG NGHIEP");
    lcd_set_cursor(&lcd_handle, 0, 1);
    lcd_write_str(&lcd_handle, "ND:");
    lcd_set_cursor(&lcd_handle, 7, 1);
    lcd_write_str(&lcd_handle, "C");

    lcd_set_cursor(&lcd_handle, 10, 1);
    lcd_write_str(&lcd_handle, "NDF:");
    lcd_set_cursor(&lcd_handle, 18, 1);
    lcd_write_str(&lcd_handle, "F");

    lcd_set_cursor(&lcd_handle, 0, 2);
    lcd_write_str(&lcd_handle, "DA:");
    lcd_set_cursor(&lcd_handle, 7, 2);
    lcd_write_str(&lcd_handle, "%");

    lcd_set_cursor(&lcd_handle, 10, 2);
    lcd_write_str(&lcd_handle, "DAD:");
    lcd_set_cursor(&lcd_handle, 18, 2);
    lcd_write_str(&lcd_handle, "%");

    lcd_set_cursor(&lcd_handle, 4, 3);
    lcd_write_str(&lcd_handle, "THAS:");
}

void LCD_sensor()
{
    lcd_set_cursor(&lcd_handle, 3, 1);
    lcd_write_str(&lcd_handle, "30:");
    lcd_set_cursor(&lcd_handle, 14, 1);
    lcd_write_str(&lcd_handle, "89");

    lcd_set_cursor(&lcd_handle, 3, 2);
    lcd_write_str(&lcd_handle, "90:");
    lcd_set_cursor(&lcd_handle, 14, 2);
    lcd_write_str(&lcd_handle, "30");

    lcd_set_cursor(&lcd_handle, 9, 3);
    lcd_write_str(&lcd_handle, "67");
}

void LCD_show()
{
    sprintf(inLED, "%d", brightnessLED);
    sprintf(inFAN, "%d", speedFAN);
    sprintf(inPUMP, "%d", speedPUMP);

    ESP_LOGI(TAGLCD, "Dang o task LCD_show");
    lcd_clear_screen(&lcd_handle);
    lcd_backlight(&lcd_handle);
    lcd_set_cursor(&lcd_handle, 0, 0);
    lcd_write_str(&lcd_handle, "CONTROl SPEED SETTING");

    lcd_set_cursor(&lcd_handle, 0, 1);
    lcd_write_str(&lcd_handle, "BRIGHTNESS LED:");
    lcd_set_cursor(&lcd_handle, 16, 1);
    lcd_write_str(&lcd_handle, inLED);
    lcd_set_cursor(&lcd_handle, 19, 1);
    lcd_write_str(&lcd_handle, "%");

    lcd_set_cursor(&lcd_handle, 0, 2);
    lcd_write_str(&lcd_handle, "FAN SPEED:");
    lcd_set_cursor(&lcd_handle, 16, 2);
    lcd_write_str(&lcd_handle, inFAN);
    lcd_set_cursor(&lcd_handle, 19, 2);
    lcd_write_str(&lcd_handle, "%");

    lcd_set_cursor(&lcd_handle, 0, 3);
    lcd_write_str(&lcd_handle, "PUMP SPEED:");
    lcd_set_cursor(&lcd_handle, 16, 3);
    lcd_write_str(&lcd_handle, inPUMP);
    lcd_set_cursor(&lcd_handle, 19, 3);
    lcd_write_str(&lcd_handle, "%");
}

void LCD_brightnessLED()
{
    ESP_LOGI(TAGLCD, "Dang o task LCD_brightnessLED");
    lcd_clear_screen(&lcd_handle);
    lcd_backlight(&lcd_handle);

    lcd_set_cursor(&lcd_handle, 2, 0);
    lcd_write_str(&lcd_handle, "SETTING POINT PWM");

    lcd_set_cursor(&lcd_handle, 4, 1);
    lcd_write_str(&lcd_handle, "BRIGHTNESS LED");
}

void LCD_speedFAN()
{
    ESP_LOGI(TAGLCD, "Dang o task LCD_speedFAN");
    lcd_clear_screen(&lcd_handle);
    lcd_backlight(&lcd_handle);

    lcd_set_cursor(&lcd_handle, 2, 0);
    lcd_write_str(&lcd_handle, "SETTING POINT PWM");

    lcd_set_cursor(&lcd_handle, 4, 1);
    lcd_write_str(&lcd_handle, "FAN SPEED");
}

void LCD_speedPUMP()
{
    ESP_LOGI(TAGLCD, "Dang o task LCD_speedPUMP");
    lcd_clear_screen(&lcd_handle);
    lcd_backlight(&lcd_handle);

    lcd_set_cursor(&lcd_handle, 2, 0);
    lcd_write_str(&lcd_handle, "SETTING POINT PWM");

    lcd_set_cursor(&lcd_handle, 4, 1);
    lcd_write_str(&lcd_handle, "PUMP SPEED");
}

void LCD_show_status()
{
    ESP_LOGI(TAGLCD, "Dang o task LCD_show_status");
    lcd_clear_screen(&lcd_handle);
    lcd_backlight(&lcd_handle);

    lcd_set_cursor(&lcd_handle, 0, 0);
    lcd_write_str(&lcd_handle, "IP:");
    lcd_set_cursor(&lcd_handle, 4, 0);
    lcd_write_str(&lcd_handle, IPaddress);

    lcd_set_cursor(&lcd_handle, 0, 1);
    lcd_write_str(&lcd_handle, "LED STATUS:");
    lcd_set_cursor(&lcd_handle, 12, 1);
    lcd_write_str(&lcd_handle, (onOffled == 1) ? "ON" : "OFF");

    lcd_set_cursor(&lcd_handle, 0, 2);
    lcd_write_str(&lcd_handle, "FAN STATUS:");
    lcd_set_cursor(&lcd_handle, 12, 2);
    lcd_write_str(&lcd_handle, (onOfffan == 1) ? "ON" : "OFF");

    lcd_set_cursor(&lcd_handle, 0, 3);
    lcd_write_str(&lcd_handle, "PUMP STATUS:");
    lcd_set_cursor(&lcd_handle, 13, 3);
    lcd_write_str(&lcd_handle, (onOffpump == 1) ? "ON" : "OFF");
}

void read_keypad(void *pvParameter)
{
    while (1)
    {
        char key = keypad_scan(&myKeypad);
        if (key != '\0')
        {
            printf("Pressed: %c\n", key);
            if (key == 'A')
            {
                lcd_home_keypad();
                LCD_sensor();
                save = 0;
            }

            if (key == 'B')
            {
                LCD_show();
                save = 0;
            }

            if (key == 'C')
            {
                lcd_clear_screen(&lcd_handle);
                save = 1;
                tam = '\0';
            }

            if (key == '*')
            {
                LCD_show_status();
                save = 0;
            }

            if (save == 1)
            {
                if (key == '#')
                {
                    if (selection == 0)
                    {
                        selection = 1;
                    }
                    else if (selection == 1)
                    {
                        selection = 2;
                    }
                    else
                    {
                        selection = 0;
                    }
                }
            }

            if (save == 1)
            {
                if (selection == 0)
                {
                    LCD_speedFAN();
                    if (key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9' || key == '0')
                    {
                        KT = 1;
                        if (loca == 18)
                        {
                            lcd_set_cursor(&lcd_handle, loca, 3);
                            lcd_write_char(&lcd_handle, key);
                        }
                        loca++;

                        if (loca == 19 || loca == 20)
                        {
                            if (loca == 19)
                            {
                                tam = key;
                            }
                            else
                            {
                                sprintf(luutrutam, "%c%c", tam, key);
                                lcd_set_cursor(&lcd_handle, 18, 3);
                                lcd_write_str(&lcd_handle, luutrutam);
                            }
                        }

                        if (loca == 21)
                        {
                            lcd_set_cursor(&lcd_handle, 0, 3);
                            lcd_write_str(&lcd_handle, "                    ");
                            loca = 18;
                            tam = '\0';
                        }
                    }
                }

                if (selection == 1)
                {
                    LCD_brightnessLED();
                    if (key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9' || key == '0')
                    {
                        KT = 1;
                        if (loca == 18 || loca == 19)
                        {
                            lcd_set_cursor(&lcd_handle, loca, 3);
                            lcd_write_char(&lcd_handle, key);
                        }
                        loca++;

                        if (loca == 19 || loca == 20)
                        {
                            if (loca == 19)
                            {
                                tam = key;
                            }
                            else
                            {
                                sprintf(luutrutam, "%c%c", tam, key);
                                lcd_set_cursor(&lcd_handle, 18, 3);
                                lcd_write_str(&lcd_handle, luutrutam);
                            }
                        }

                        if (loca == 21)
                        {
                            lcd_set_cursor(&lcd_handle, 0, 3);
                            lcd_write_str(&lcd_handle, "                    ");
                            loca = 18;
                            tam = '\0';
                        }
                    }
                }

                if (selection == 2)
                {
                    LCD_speedPUMP();
                    if (key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9' || key == '0')
                    {
                        KT = 1;
                        if (loca == 18 || loca == 19)
                        {
                            lcd_set_cursor(&lcd_handle, loca, 3);
                            lcd_write_char(&lcd_handle, key);
                        }
                        loca++;

                        if (loca == 19 || loca == 20)
                        {
                            if (loca == 19)
                            {
                                tam = key;
                            }
                            else
                            {
                                sprintf(luutrutam, "%c%c", tam, key);
                                lcd_set_cursor(&lcd_handle, 18, 3);
                                lcd_write_str(&lcd_handle, luutrutam);
                            }
                        }

                        if (loca == 21)
                        {
                            lcd_set_cursor(&lcd_handle, 0, 3);
                            lcd_write_str(&lcd_handle, "                    ");
                            loca = 18;
                            tam = '\0';
                        }
                    }
                }

                if (key == 'D')
                {
                    if (selection == 0)
                    {
                        if (KT == 1)
                        {
                            speedFAN = atoi(luutrutam);
                            nvs_save_and_check((int)speedFAN, "pwm_fan");
                            mqtt_send_data(15,speedFAN);

                            KT = 0;
                        }
                    }
                    if (selection == 1)
                    {
                        if (KT == 1)
                        {
                            brightnessLED = atoi(luutrutam);
                            nvs_save_and_check((int)brightnessLED, "pwm_led");
                            mqtt_send_data(14,brightnessLED);
                            KT = 0;
                        }
                    }
                    if (selection == 2)
                    {
                        if (KT == 1)
                        {
                            speedPUMP = atoi(luutrutam);
                            nvs_save_and_check((int)speedPUMP, "pwm_pump");
                            mqtt_send_data(16,speedPUMP);

                            KT = 0;
                        }
                    }
                }
            }
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
