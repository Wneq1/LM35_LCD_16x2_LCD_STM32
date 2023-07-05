#include "stm32l1xx_hal.h"

void lcd_init (void);   // initialize lcd

void lcd_send_cmd (char cmd);  // send command to the lcd

void lcd_send_data (char data);  // send data to the lcd

void lcd_send_string (char *str);  // send string to the lcd

void lcd_put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear (void);

void lcd_print_char_by_char(char *str, uint16_t currtim,uint16_t del);

void lcd_scroll_text_loop(const char *text, int line, uint16_t delay_ms);

