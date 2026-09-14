#define F_CPU 16000000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN PB5
#define BUZZER_PIN PB0
#define BUTTON_PIN PD2

#define LCD_PORT PORTC
#define LCD_DDR DDRC
#define LCD_RS PC0
#define LCD_EN PC1
#define LCD_D4 PC2
#define LCD_D5 PC3
#define LCD_D6 PC4
#define LCD_D7 PC5

void lcd_pulse(void) {
  LCD_PORT |= (1 << LCD_EN);
  _delay_us(1);
  LCD_PORT &= ~(1 << LCD_EN);
  _delay_us(100);
}

void lcd_nibble(uint8_t data) {
  LCD_PORT &= ~((1 << LCD_D4) | (1 << LCD_D5) | (1 << LCD_D6) | (1 << LCD_D7));

  if (data & 0x01)
    LCD_PORT |= (1 << LCD_D4);
  if (data & 0x02)
    LCD_PORT |= (1 << LCD_D5);
  if (data & 0x04)
    LCD_PORT |= (1 << LCD_D6);
  if (data & 0x08)
    LCD_PORT |= (1 << LCD_D7);

  lcd_pulse();
}

void lcd_command(uint8_t cmd) {
  LCD_PORT &= ~(1 << LCD_RS);
  lcd_nibble(cmd >> 4);
  lcd_nibble(cmd & 0x0F);
  _delay_ms(2);
}

void lcd_char(char data) {
  LCD_PORT |= (1 << LCD_RS);
  lcd_nibble(data >> 4);
  lcd_nibble(data & 0x0F);
  _delay_ms(2);
}

void lcd_init(void) {
  LCD_DDR |= (1 << LCD_RS) | (1 << LCD_EN) | (1 << LCD_D4) | (1 << LCD_D5) |
             (1 << LCD_D6) | (1 << LCD_D7);
  _delay_ms(20);

  LCD_PORT &= ~(1 << LCD_RS);

  lcd_nibble(0x03);
  _delay_ms(5);
  lcd_nibble(0x03);
  _delay_us(150);
  lcd_nibble(0x03);
  _delay_ms(5);
  lcd_nibble(0x02);
  _delay_ms(5);

  lcd_command(0x28);
  lcd_command(0x0C);
  lcd_command(0x06);
  lcd_command(0x01);
  _delay_ms(5);
}

void lcd_string(const char *str) {
  while (*str) {
    lcd_char(*str++);
  }
}

ISR(INT0_vect) {}

void setup_hardware(void) {
  DDRB |= (1 << LED_PIN) | (1 << BUZZER_PIN);

  DDRD &= ~(1 << BUTTON_PIN);
  PORTD |= (1 << BUTTON_PIN);

  EICRA |= (1 << ISC01);
  EICRA &= ~(1 << ISC00);

  EIMSK |= (1 << INT0);
}

void enter_sleep_mode(void) {
  SMCR &= ~(1 << SM2);
  SMCR |= (1 << SM1);
  SMCR &= ~(1 << SM0);

  SMCR |= (1 << SE);

  lcd_command(0x01);
  lcd_string("Zzz... Sleeping");
  _delay_ms(100);

  sei();

  __asm__ __volatile__("sleep");

  SMCR &= ~(1 << SE);
}

int main(void) {
  setup_hardware();
  lcd_init();

  while (1) {
    lcd_command(0x01);
    lcd_string("System: NORMAL");
    lcd_command(0xC0);
    lcd_string("LED ON");

    PORTB |= (1 << LED_PIN);
    PORTB &= ~(1 << BUZZER_PIN);

    _delay_ms(3000);

    PORTB &= ~(1 << LED_PIN);

    enter_sleep_mode();

    lcd_command(0x01);
    lcd_string("WAKING UP!");

    PORTB |= (1 << BUZZER_PIN);
    PORTB |= (1 << LED_PIN);
    _delay_ms(500);
    PORTB &= ~(1 << BUZZER_PIN);
    _delay_ms(1500);
  }
}
