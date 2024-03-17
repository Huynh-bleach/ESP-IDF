
#ifndef ESPCONFIGURATION_H
#define ESPCONFIGURATION_H

#include "button_and_pwm.h"
#include "dht.h"
#include "lcd.h"
#include "nvs_memory.h"
#include "keypad.h"

#include "library_mqtt_broker.h"

extern Keypad myKeypad;
extern lcd_handle_t lcd_handle;

static const char *TAGLCD = "lcd_example";

#define buttonled 15
#define buttonfan 2
#define buttonpump 4

#define pinled 5
#define pinfan 18
#define pinpump 19





extern int save;
extern int loca;
extern int speedFAN;
extern int brightnessLED;
extern int speedPUMP;
extern char tam;
extern char luutrutam[20];
extern int KT;
extern int selection;

extern char inLED[10];
extern char inFAN[10];
extern char inPUMP[10];

extern char IPaddress[20];


extern unsigned char onOffled;
extern unsigned char onOfffan;
extern unsigned char onOffpump;


void read_keypad(void *pvParameter);
void lcd_home_keypad();
void LCD_sensor();
void LCD_brightnessLED();
void LCD_speedFAN();
void LCD_speedPUMP();
void LCD_show_status();
void LCD_show();


void Task_readbuttonled(void *pvParameters);
void Task_readbuttonfan(void *pvParameters);
void Task_readbuttonpump(void *pvParameters);

void config_button_control();
void take_data_nvs();





#endif