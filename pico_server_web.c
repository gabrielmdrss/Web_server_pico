#include "pico/stdlib.h"
#include <string.h>
#include <stdio.h>
#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"
#include "DEFINES_FUNCTIONS.h"
#include "lwip/apps/httpd.h"

int main() {
    stdio_init_all();  // Initializes standard output
    sleep_ms(10000);

    // Initializing the pins
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);


    printf("Starting HTTP Server\n");

    // Starts HTTP Server
    if (cyw43_arch_init()) {
        printf("Error initializing Wi-Fi\n");
        return 1;
    }

    cyw43_arch_enable_sta_mode();
    printf("Connecting to Wi-Fi...\n");

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Failed to connect to Wi-Fi\n");
        return 1;
    }else {
        printf("Connected.\n");
        // Read the ip address in a human readable way
        uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
        //printf("Endereço IP %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    }

    printf("Wi-Fi connected!\n");
    printf("To turn the LED on or off, access the IP Address followed by /?led=on or /?led=off\n");

    // Start the HTTP server
    start_http_server();
    
    // Main loop
    while (true) {
        if (!gpio_get(BUTTON_A_PIN))
            button_state = "1";
            //button_state = "Button is pressioned"; 
        else
            button_state = "0";
            //button_state = "Button is not pressioned"; 

        cyw43_arch_poll();  // Required to keep Wi-Fi active
        sleep_ms(100);
    }

    cyw43_arch_deinit();  // Turns off Wi-Fi (it won't be called as the loop is infinite)
    return 0;
}