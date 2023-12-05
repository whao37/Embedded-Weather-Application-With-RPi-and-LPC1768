# Embedded Weather View
Developed and Maintained By: Luke Weidner, Gillian Kearny, William Hao

This project features an integration of a Raspberry Pi Zero W with the OPEN-METEO API to acquire real-time weather data for the Atlanta area. The collected data is transmitted to an mbed LPC1768 board. The current temperature is displayed through a SparkFun uLCD-144-G2 LCD and an RGB LED to indicate various temperature ranges through color changes, providing an intuitive and visual representation of the weather conditions.

![unnamed](https://github.com/whao37/Embedded-Weather-Application-With-RPi-and-LPC1768/assets/86330766/6de370ce-77b8-496b-98f6-f332a0d26d52)


## The Big Picture / The Idea
Our team wanted to create an embedded weather application that displays Atlanta's weather with a quick look (instantly - without the hassle of asking Google Home, or opening the Weather app!).
 
At the core of the technology, the application fetches current weather data for free through the OPEN-METEO API, specifically tailored for the Atlanta area, and displays the info with a nice and user-friendly interface.
The processing unit of the Pi Zero W executes the weather_fetcher.c program responsible for asking OPEN-METEO API to give us the temperature at regular intervals (30 seconds, can be configured easily with sleep()) ensuring that the user gets up-to-date with their data. This is made possible with the Libcurl (client-side URL transfer library) which supports a large range of protocols like HTTP, HTTPS, etc. The libcurl allows us to fetch the OPEN-METEO API data without much hassle and knowledge of network-comms - allowing the user to get the Atlanta Tmperature. When the data is received, the program outputs a text file named weather_data.txt - a raw file that we need to parse through. The program parses through just to find the temperature value and converts the string into a float value for it to be read on the mBed LPC1768. We also needed to open serial communication between the LPC1768 and Pi through UART. Once the temperature is transferred over, we display the current information on the uLCD-144-G2 and through the RGB LED. The font of the temperature on the LCD is made as big as possible for the user to be able to read it easily.


## Hardware Requirements
- Raspberry Pi Zero W
- mbed LPC1768
- uLCD-144-G2 graphical LCD
- RGB LED
- Connecting wires

## Software Requirements
- Raspbian OS
- Geany
- mbed compiler for the LPC1768 (Keil Studio)
- libcurl for network communication handling

## Setup Instructions
### Enabling UART on the Raspberry Pi
The Raspberry Pi's UART (Universal Asynchronous Receiver/Transmitter) is disabled by default. To enable it, the hardware will need to be reconfigured.
The user would need to open terminal and then run:
- `sudo raspi-config`
- Navigate to `Interfacing Options` > `Serial`
- Disable the login shell over serial
- Enable the serial port hardware
- Save and Reboot the Pi

### Installing libcurl to the Raspberry Pi
libcurl is a great way to fetch our weather data from the Open-Meteo API. It's a free client-side URL transfer library that handles the network communication between the Raspberry Pi and the Open-Meteo API.
To install, simply run this line in the terminal:
`sudo apt-get install libcurl4-openssl-dev`

There is a libcurl website with documentation that helps the user create their own client side url transfers.
https://curl.se/libcurl/c/libcurl.html

### Wiring

**On the Pi:**  
![image](https://github.com/whao37/Embedded-Weather-Application-With-RPi-and-LPC1768/assets/86330766/9076bf5c-c88a-48bc-a365-9c3cd9aad327)

**LCD Wiring:**  
![image](https://github.com/whao37/Embedded-Weather-Application-With-RPi-and-LPC1768/assets/86330766/24d942be-bbfa-4c0a-beac-401587355438)  
*Table from https://os.mbed.com/users/4180_1/notebook/ulcd-144-g2-128-by-128-color-lcd/

**RGB LED on PWM Pins:**  
We used PWM capable pins p23, p24, and p25 for controlling the RGB LED colors.  
![image](https://github.com/whao37/Embedded-Weather-Application-With-RPi-and-LPC1768/assets/86330766/c855a858-0c41-489c-86dc-7272b3307855)  
![image](https://github.com/whao37/Embedded-Weather-Application-With-RPi-and-LPC1768/assets/86330766/fe378309-1c12-46fd-b401-2a10f3ed55ed)

*Figure from https://os.mbed.com/users/4180_1/notebook/rgb-leds/


## How to Run
- Connect the Pi UART to the LPC1768 UART
- Power on both devices
- Compile and upload mbed code to the LPC1768
- Execute the weather_fetcher.c code on Geany (or any other C compiler)

The user should then see the temperature of Atlanta updating in real time on the uLCD every 30 seconds.
```c
while(1) {
        // Fetch data, parse, and send over UART...
        sleep(30); // Check every 30 seconds
    }
```

