#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <curl/curl.h>

//write response data function
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int main(void) {
    CURL *curl;
    CURLcode res;
    FILE *fp;

    //OPEN-METEO API: current coordinates is set for ATLANTA (simply change values)
    char *url = "https://api.open-meteo.com/v1/forecast?latitude=33.75605&longitude=-84.36386&current=temperature_2m";

    curl_global_init(CURL_GLOBAL_ALL);
    
    while(1) {

    //Create curl session
    curl = curl_easy_init();
    if(curl) {
        FILE *fp = fopen("weather_data.txt", "wb");
        if(!fp) {
            perror("Cannot open file");
            return 1;
        }

        //set url for curl session
        curl_easy_setopt(curl, CURLOPT_URL, url);

        //set write function to save data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

        //the file to write the data to:
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        //perform request, return code
        res = curl_easy_perform(curl);

        //error checks
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        //cleanup
        curl_easy_cleanup(curl);

        fclose(fp);
    }

    curl_global_cleanup();

    //read the file
    fp = fopen("weather_data.txt", "r");
    if (!fp) {
        perror("Unable to open the file");
        return 1;
    }

    char buffer[512];
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        //temperature is always last in the parsable text file
        char *tempStr = strrchr(buffer, ':');
        if(tempStr && strlen(tempStr) > 2) {
            float temperature = atof(tempStr + 1);
            printf("Temperature: %.2f C\n", temperature); //print what gets sent out

            //create UART and send temps
            int uart0_filestream = -1;
            uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);

            if (uart0_filestream == -1) {
                perror("Unable to open UART");
                return 1;
            }
            //more uart configs
            struct termios options;
            tcgetattr(uart0_filestream, &options);
            options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; //serial stuff
            options.c_iflag = IGNPAR | ICRNL; //ignore parity error and CR -> newline for the text 
            tcflush(uart0_filestream, TCIFLUSH); //flush the stream
            tcsetattr(uart0_filestream, TCSANOW, &options);
            //send the temperature
            char tempBuffer[256];
            sprintf(tempBuffer, "%f\n", temperature);
            int count = write(uart0_filestream, &tempBuffer[0], strlen(tempBuffer));
            if (count < 0) {
                perror("UART TX error");
                return 1;
            }

            //close teh UART
            close(uart0_filestream);
        }
    }
    fclose(fp);
    sleep(30); //check every 30 seconds for a new update on the temperature
}
	curl_global_cleanup();

    return 0;
}
