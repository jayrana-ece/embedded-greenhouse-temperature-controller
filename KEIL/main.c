#include <LPC213x.h>

/* =========================================================
   LPC2138 GREENHOUSE TEMPERATURE CONTROLLER
   LM35 -> ADC0804 -> LPC2138

   LCD  : 16x2, 4-bit mode
   UART : UART0, 9600 baud
   FAN  : PWM2 on P0.7

   ADC0804:
   WR   -> P0.6
   RD   -> P0.8
   INTR -> P0.9
   D0-D7 -> P1.16-P1.23
   ========================================================= */


/* ---------------- LCD PINS ---------------- */

#define LCD_RS      (1UL << 16)
#define LCD_EN      (1UL << 17)
#define LCD_D4      (1UL << 18)
#define LCD_D5      (1UL << 19)
#define LCD_D6      (1UL << 20)
#define LCD_D7      (1UL << 21)

#define LCD_DATA    (LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7)


/* -------------- ADC0804 PINS -------------- */

#define ADC_WR      (1UL << 6)
#define ADC_RD      (1UL << 8)
#define ADC_INTR    (1UL << 9)


/* =========================================================
   DELAY
   ========================================================= */

void delay(unsigned int count)
{
    volatile unsigned int i, j;

    for(i = 0; i < count; i++)
    {
        for(j = 0; j < 1000; j++);
    }
}


/* =========================================================
   LCD FUNCTIONS
   ========================================================= */

void lcd_enable(void)
{
    IOSET0 = LCD_EN;
    delay(1);

    IOCLR0 = LCD_EN;
    delay(1);
}


void lcd_nibble(unsigned char data)
{
    IOCLR0 = LCD_DATA;

    if(data & 0x01)
        IOSET0 = LCD_D4;

    if(data & 0x02)
        IOSET0 = LCD_D5;

    if(data & 0x04)
        IOSET0 = LCD_D6;

    if(data & 0x08)
        IOSET0 = LCD_D7;

    lcd_enable();
}


void lcd_cmd(unsigned char cmd)
{
    IOCLR0 = LCD_RS;

    lcd_nibble(cmd >> 4);
    lcd_nibble(cmd & 0x0F);

    delay(2);
}


void lcd_data(unsigned char data)
{
    IOSET0 = LCD_RS;

    lcd_nibble(data >> 4);
    lcd_nibble(data & 0x0F);

    delay(2);
}


void lcd_string(char *str)
{
    while(*str)
    {
        lcd_data(*str);
        str++;
    }
}


void lcd_number(unsigned int num)
{
    char buffer[6];
    unsigned char i = 0;

    if(num == 0)
    {
        lcd_data('0');
        return;
    }

    while(num > 0)
    {
        buffer[i] = (num % 10) + '0';
        num = num / 10;
        i++;
    }

    while(i > 0)
    {
        i--;
        lcd_data(buffer[i]);
    }
}


void lcd_init(void)
{
    IODIR0 |= LCD_RS | LCD_EN | LCD_DATA;

    IOCLR0 = LCD_RS | LCD_EN | LCD_DATA;

    delay(20);

    lcd_cmd(0x02);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);

    delay(5);
}


/* =========================================================
   UART0 FUNCTIONS
   ========================================================= */

void uart0_init(void)
{
    /* P0.0 = TXD0, P0.1 = RXD0 */

    PINSEL0 &= ~0x0000000FUL;
    PINSEL0 |=  0x00000005UL;

    /*
       VPBDIV = 0
       PCLK = CCLK / 4

       Crystal/CCLK = 10 MHz
       Therefore PCLK = 2.5 MHz
    */

    VPBDIV = 0x00;

    /* 8-bit, 1 stop bit, DLAB = 1 */

    U0LCR = 0x83;

    /*
       Baud = PCLK / (16 × DLL)

       DLL = 2,500,000 / (16 × 9600)
           = 16.27

       Use 16
    */

    U0DLL = 16;
    U0DLM = 0;

    /* DLAB = 0 */

    U0LCR = 0x03;
}


void uart0_tx(unsigned char ch)
{
    while((U0LSR & 0x20) == 0);

    U0THR = ch;
}


void uart0_string(char *str)
{
    while(*str)
    {
        uart0_tx(*str);
        str++;
    }
}


void uart0_number(unsigned int num)
{
    char buffer[6];
    unsigned char i = 0;

    if(num == 0)
    {
        uart0_tx('0');
        return;
    }

    while(num > 0)
    {
        buffer[i] = (num % 10) + '0';
        num = num / 10;
        i++;
    }

    while(i > 0)
    {
        i--;
        uart0_tx(buffer[i]);
    }
}


/* =========================================================
   ADC0804 FUNCTIONS
   ========================================================= */

void adc0804_init(void)
{
    /*
       WR and RD = OUTPUT
    */

    IODIR0 |= ADC_WR | ADC_RD;

    /*
       INTR = INPUT
    */

    IODIR0 &= ~ADC_INTR;

    /*
       P1.16-P1.23 = INPUT
    */

    IODIR1 &= ~(0xFFUL << 16);

    /*
       Idle state
       WR = HIGH
       RD = HIGH
    */

    IOSET0 = ADC_WR | ADC_RD;
}


unsigned int adc0804_read(void)
{
    unsigned int value;

    /*
       Start conversion
       WR pulse LOW -> HIGH
    */

    IOCLR0 = ADC_WR;
    delay(1);

    IOSET0 = ADC_WR;


    /*
       Wait until conversion completes
       INTR becomes LOW
    */

    while(IOPIN0 & ADC_INTR);


    /*
       Enable ADC output
       RD = LOW
    */

    IOCLR0 = ADC_RD;
    delay(1);


    /*
       Read D0-D7
       ADC D0 -> P1.16
       ADC D7 -> P1.23
    */

    value = (IOPIN1 >> 16) & 0xFF;


    /*
       Disable output
       RD = HIGH
    */

    IOSET0 = ADC_RD;

    return value;
}


/* =========================================================
   PWM2 FUNCTIONS
   ========================================================= */

void pwm_init(void)
{
    /* P0.7 = PWM2 */

    PINSEL0 &= ~(3UL << 14);
    PINSEL0 |=  (2UL << 14);

    /* Make sure PWM counter is stopped */

    PWMTCR = 0x02;

    /* PCLK assumed = 10 MHz / 4 = 2.5 MHz */

    PWMPR = 0;

    /* PWM period */

    PWMMR0 = 2500;

    /* Start with fan OFF */

    PWMMR2 = 0;

    /* Reset counter when MR0 matches */

    PWMMCR = (1UL << 1);

    /* Enable PWM2 output */

    PWMPCR = (1UL << 10);

    /* Latch values */

    PWMLER = (1UL << 0) | (1UL << 2);

    /* Enable PWM counter and PWM mode */

    PWMTCR = (1UL << 0) | (1UL << 3);
}


void fan_speed(unsigned int percent)
{
    unsigned long duty;

    if(percent > 100)
        percent = 100;

    if(percent == 0)
    {
        PWMMR2 = 0;
    }
    else if(percent == 100)
    {
        PWMMR2 = PWMMR0 - 1;
    }
    else
    {
        duty = ((unsigned long)PWMMR0 * percent) / 100;
        PWMMR2 = duty;
    }

    PWMLER = (1UL << 2);
}


/* =========================================================
   MAIN PROGRAM
   ========================================================= */

int main(void)
{
    unsigned int adc_value;
    unsigned int temperature;
    unsigned int fan_percent;


    /* Initialize peripherals */

    lcd_init();

    uart0_init();

    adc0804_init();

    pwm_init();
	
		fan_speed(100);


    /* Startup message */

    lcd_cmd(0x80);
    lcd_string("GREENHOUSE TEMP");

    lcd_cmd(0xC0);
    lcd_string("CONTROLLER");

    uart0_string("\r\n");
    uart0_string("GREENHOUSE TEMPERATURE CONTROLLER\r\n");

    delay(100);


    while(1)
    {
        /*
           Read ADC0804
        */

        adc_value = adc0804_read();


        /*
           Temperature calculation

           ADC reference = 5V
           LM35 = 10mV per degree Celsius

           Temperature =
           ADC_value * 500 / 255
        */

        temperature = (adc_value * 500UL) / 255;


        /*
           FAN SPEED CONTROL
        */

        if(temperature < 25)
        {
            fan_percent = 0;
        }
        else if(temperature < 30)
        {
            fan_percent = 30;
        }
        else if(temperature < 35)
        {
            fan_percent = 50;
        }
        else if(temperature < 40)
        {
            fan_percent = 75;
        }
        else
        {
            fan_percent = 100;
        }


        /* Apply PWM */

        fan_speed(fan_percent);


        /*
           LCD DISPLAY
        */

        lcd_cmd(0x01);

        lcd_cmd(0x80);
        lcd_string("TEMP:");
        lcd_number(temperature);
        lcd_string(" C");


        lcd_cmd(0xC0);

        if(fan_percent == 0)
        {
            lcd_string("FAN: OFF");
        }
        else if(fan_percent == 30)
        {
            lcd_string("FAN: LOW 30%");
        }
        else if(fan_percent == 50)
        {
            lcd_string("FAN: MED 50%");
        }
        else if(fan_percent == 75)
        {
            lcd_string("FAN: HIGH75%");
        }
        else
        {
            lcd_string("FAN: MAX 100%");
        }


        /*
           UART OUTPUT
        */
				
        uart0_string("Temperature = ");
        uart0_number(temperature);
        uart0_string(" C | ");

				
        if(fan_percent == 0)
        {
            uart0_string("Fan = OFF");
        }
        else
        {
            uart0_string("Fan Speed = ");
            uart0_number(fan_percent);
            uart0_string("%");
        }

        uart0_string("\r\n");


        delay(100);
    }
}