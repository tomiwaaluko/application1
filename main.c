/* --------------------------------------------------------------
   Application: 01 - Rev1
   Release Type: Baseline Multitask Skeleton Starter Code 
   Class: Real Time Systems - Su 2025
   AI Use: Commented inline
---------------------------------------------------------------*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN           GPIO_NUM_2
#define BLINK_HALF_MS     250        // 250 ms ON, 250 ms OFF -> 2 Hz
#define PRINT_PERIOD_MS   10000      // 10 s

// Task to blink an LED at 2 Hz (500 ms period: 250 ms ON, 250 ms OFF)
void blink_task(void *pvParameters) {
    bool led_on = false;
    TickType_t next_wake = xTaskGetTickCount(); // optional: steadier timing
    while (1) {
        gpio_set_level(LED_PIN, led_on ? 1 : 0); // <-- use the flag
        // Optional debug: printf("[LED] %s\n", led_on ? "ON" : "OFF");
        led_on = !led_on;

        // Use delay-until for lower jitter over time (either is acceptable for this lab)
        vTaskDelayUntil(&next_wake, pdMS_TO_TICKS(BLINK_HALF_MS));
        // (If you prefer the simpler approach, vTaskDelay(pdMS_TO_TICKS(BLINK_HALF_MS)); is fine)
    }
    vTaskDelete(NULL);
}

// Task to print a message every 10 seconds
void print_task(void *pvParameters) {
    TickType_t next_wake = xTaskGetTickCount();
    uint32_t beat = 0;
    while (1) {
        // Replace string with your chosen theme text if you like
        printf("[LOG] System alive (beat=%lu, tick=%lu)\n",
               (unsigned long)beat++,
               (unsigned long)xTaskGetTickCount());
        vTaskDelayUntil(&next_wake, pdMS_TO_TICKS(PRINT_PERIOD_MS));
    }
    vTaskDelete(NULL);
}

void app_main() {
    // Initialize LED GPIO     
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    
    // Create tasks (same priority as instructed)
    xTaskCreate(blink_task, "BlinkTask", 2048, NULL, 1, NULL);
    xTaskCreate(print_task, "PrintTask", 2048, NULL, 1, NULL);
}
