#include <xc.h>
#include "clcd.h"

// Replace long delays with shorter simulation-friendly ones
#define CLCD_DELAY_US(x) __delay_us(10)
#define CLCD_DELAY_MS(x) __delay_ms(1)

void clcd_write(unsigned char byte, unsigned char mode)
{
    CLCD_RS = mode;
    CLCD_DATA_PORT = byte;

    CLCD_EN = HI;
    CLCD_DELAY_US(10);
    CLCD_EN = LOW;

    CLCD_DELAY_US(100); // Reduced from 4100
}

static void init_display_controller(void)
{
    /* Startup Time for the CLCD controller */
    CLCD_DELAY_MS(5); // Reduced from 30

    /* The CLCD Startup Sequence */
    clcd_write(EIGHT_BIT_MODE, INST_MODE);
    CLCD_DELAY_US(100); // Reduced from 4100

    clcd_write(EIGHT_BIT_MODE, INST_MODE);
    CLCD_DELAY_US(50);  // Reduced

    clcd_write(EIGHT_BIT_MODE, INST_MODE);
    CLCD_DELAY_US(1);   // Kept same

    clcd_write(TWO_LINES_5x8_8_BIT_MODE, INST_MODE);
    CLCD_DELAY_US(50);

    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    CLCD_DELAY_US(100); // Reduced from 500

    clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
    CLCD_DELAY_US(50);
}

void init_clcd(void)
{
    /* Setting the CLCD Data Port as Output */
    CLCD_DATA_PORT_DDR = 0x00;

    /* Setting the RS and EN lines as Output */
    CLCD_RS_DDR = 0;
    CLCD_EN_DDR = 0;

    init_display_controller();
}

void clcd_putch(const char data, unsigned char addr)
{
    clcd_write(addr, INST_MODE);
    clcd_write(data, DATA_MODE);
}

void clcd_print(const char *str, unsigned char addr)
{
    clcd_write(addr, INST_MODE);
    while (*str != '\0')
    {
        clcd_write(*str, DATA_MODE);
        str++;
    }
}
