#include <xparameters.h>
#include <xgpio.h>
#include <xstatus.h>
#include <xil_printf.h>

/*
Definitions
*/

#define GPIO_LEDS_ID XPAR_LED_DEVICE_ID
#define GPIO_INPUTS_ID XPAR_INPUTS_DEVICE_ID
/*
GPIO device that LEDs are connected to
*/

#define WAIT_VAL 10000000

int delay(void);

int main()
{

    int count;
    int count_masked;
    XGpio leds;
    XGpio inputs;
    int inputs;
    int statusLeds;
    int statusInputs;

    statusLeds = XGpio_Initialize(&leds, GPIO_LEDS_ID);
    XGpio_SetDataDirection(&leds, 1, 0x00);

    statusInputs = XGpio_Initialize(&inputs, GPIO_INPUTS_ID);
    XGpio_SetDataDirection(&inputs, 1, 0xFF);

    if (statusLeds != XST_SUCCESS)
    {
        xil_printf("leds Initialization failed");
    }

    if (statusInputs != XST_SUCCESS)
    {
        xil_printf("inputs Initialization failed");
    }

    count = 0;

    while (1)
    {

        // read from inputs

        inputs = XGpio_DiscreteRead(&inputs, 1);

        // check if button0 is pressed, it is the 5th bit.

        if (inputs & 0x10)
        {
            count++;
            xil_printf("Button 0 pressed\n\r");
        }

        // if buttton0 is on increment count by 1

        if (inputs & 0x20)
        {
            count--;
            xil_printf("Button 1 pressed\n\r");
        }

        // if button1 is pressed decrement count by 1

        if (inputs & 0x40)
        {
            xil_printf("Button 2 pressed\n\r");
            int switches = inputs & 0x0F;
            xil_printf("Switches value = 0x%x\n\r", switches);
        }

        // if button2 is pressed dislay the status of the switches

        if (inputs & 0x80)
        {
            xil_printf("Button 4 pressed\n\r");
            count_masked = count & 0xF;
            XGpio_DiscreteWrite(&leds, 1, count_masked);
            xil_printf("count displayed on leds 0x%x\n\r", switches);
        }
        // if button4 is pressed dislay count on the leds

        // console should display the current action and leds value

        xil_printf("LEDs value = 0x%x\n\r", count_masked);

        delay();
    }

    return 0;
}

int delay(void)
{
    volatile int delay_count = 0;
    while (delay_count < WAIT_VAL)
        delay_count++;

    return 0;
}