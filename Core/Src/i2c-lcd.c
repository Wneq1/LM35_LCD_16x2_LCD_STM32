
/** Put this in the src folder **/
#include <string.h>
#include "i2c-lcd.h"
extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly

#define SLAVE_ADDRESS_LCD 0x4E // change this according to ur setup

void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_clear(void)
{
  lcd_send_cmd(0x01); // Polecenie czyszczenia ekranu
  HAL_Delay(2); // Poczekaj na zakończenie czyszczenia ekranu
}

void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd (col);
}


void lcd_init (void)
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_send_cmd (0x30);
	HAL_Delay(1);  // wait for >100us
	lcd_send_cmd (0x30);
	HAL_Delay(10);
	lcd_send_cmd (0x20);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	lcd_send_cmd (0x01);  // clear display
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}


void lcd_print_char_by_char(char *str, uint16_t tim,uint16_t del) //  uint16_t t1 = HAL_GetTick(); powinno byc zainiciowane w funkcji main i pobiera ona aktualny czas zegara i dziala jak delay
{
    while (*str)
    {
    	if((HAL_GetTick()- tim)>del){
    		tim = HAL_GetTick();
        lcd_send_data(*str++);
        HAL_Delay(100);
  }
 }
}


void lcd_scroll_text_loop(const char *text, int line, uint16_t delay_ms)
{
  int text_length = strlen(text);
  int display_width = 16;  // Szerokość wyświetlacza LCD - dostosuj do swojego ekranu

  // Sprawdź, czy numer linii jest poprawny (0 lub 1)
  if (line < 0 || line > 1)
  {
    return;  // Zakończ funkcję, jeśli numer linii jest niepoprawny
  }

  // Przesuwanie tekstu w lewo na podanej linii
  for (int i = 0; i <= text_length + display_width; i++)
  {
    lcd_clear();  // Wyczyść ekran LCD przed każdym przesunięciem tekstu

    if (i >= display_width)
    {
      int text_index = i - display_width;
      int num_chars = (text_index < text_length) ? display_width : (text_length - text_index);
      lcd_put_cur(line, 0);  // Ustaw kursor na podanej linii

      for (int j = 0; j < num_chars; j++) {
        lcd_send_data(text[text_index + j]);
      }
    }

    HAL_Delay(delay_ms);  // Poczekaj przez określony czas
  }
}
