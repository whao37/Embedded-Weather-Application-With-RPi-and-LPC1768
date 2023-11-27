#include "mbed.h"
#include "uLCD_4DGL.h"

//pwm outputs for the led
PwmOut redLed(p23);
PwmOut greenLed(p24);
PwmOut blueLed(p25);

Serial pc(USBTX, USBRX); // Serial connection to the windows pc
Serial device(p28, p27); // UART connection to Raspberry Pi
uLCD_4DGL uLCD(p9, p10, p11); // uLCD connection pins

void setColor(float temperature) {
    if (temperature < 7.0) {
        // Set LED to blue when it's less than 7 degrees Celius
        redLed = 0.0;
        greenLed = 0.0;
        blueLed = 1.0;
    } else if (temperature > 10.0) {
        // Set LED to red when it's greater than 10 degrees celcius 
        redLed = 1.0;
        greenLed = 0.0;
        blueLed = 0.0;
    } else {
        // Set LED to green when it's between 7-10 degrees celsius
        redLed = 0.0;
        greenLed = 1.0;
        blueLed = 0.0;
    }
}

int main() {
    device.baud(9600);
    uLCD.baudrate(9600);

    // Static text
    uLCD.background_color(BLACK);
    uLCD.textbackground_color(BLACK);
    uLCD.color(GREEN);
    uLCD.locate(0,2);
    uLCD.printf("Temperature\n   in Atlanta:");

    char buffer[256];
    int index = 0;
    float temperature;

    while(1) {
        if(device.readable()) {
            char c = device.getc();
            pc.putc(c); // Echo to PC for debugging

            if (c != '\n') {
                buffer[index++] = c;
                if (index >= 255) {
                    index = 0; // Prevent buffer overflow
                }
            } else {
                buffer[index] = '\0';
                temperature = atof(buffer);
                index = 0; // Reset index for next reading

                // Set the color of the RGB LED based on temperature
                setColor(temperature);

                // Update only the temperature part on the LCD
                uLCD.text_width(2);
                uLCD.text_height(2);
                uLCD.locate(2,4);
                uLCD.color(BLACK);
                uLCD.printf("      ");
                uLCD.color(GREEN);
                uLCD.locate(2,4);
                uLCD.printf("%.2f%cC", temperature, 0xDF);
            }
        }
    }
}
