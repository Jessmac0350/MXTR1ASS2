
#include <stdio.h>
#include <stdlib.h>
#include "message.h"
#include "aircraft.h"
#include "requests.h"

// Main function: reads incoming messages from stdin.
// Decodes ADS-B messages and stores the latest information
// for each aircraft in a dynamically allocated array.

int main(void) {

    char message[256];

    // Initial array size
    int num_of_aircraft = 5;
    int count = 0;

    // Allocate memory
    ADSBPacket *aircraft;
    aircraft = malloc(num_of_aircraft * sizeof(ADSBPacket));

    if (aircraft == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Read incoming messages
    while (fgets(message, sizeof(message), stdin) != NULL) {

        if (message[0] == '#') {

            // Decode incoming aircraft message
            ADSBPacket incoming;

            if (decode_aircraft(message, &incoming) == 0) {

                // Check if aircraft already exists
                int found = 0;

                for (int i = 0; i < count; i++) {

                    if (aircraft[i].id == incoming.id) {

                        // Update existing aircraft
                        aircraft[i] = incoming;
                        found = 1;
                        break;
                    }
                }

                // If aircraft does not exist, add new aircraft
                if (found == 0) {

                    // Check whether the array is full
                    if (count == num_of_aircraft) {

                        // Increase capacity
                        int new_num_of_aircraft = num_of_aircraft * 2;

                        ADSBPacket *temp = realloc(
                            aircraft,
                            new_num_of_aircraft * sizeof(ADSBPacket)
                        );

                        if (temp == NULL) {
                            fprintf(stderr, "Memory reallocation failed\n");
                            free(aircraft);
                            return 1;
                        }

                        aircraft = temp;
                        num_of_aircraft = new_num_of_aircraft;
                    }

                    // Store new aircraft
                    aircraft[count] = incoming;
                    count++;
                }
            }
        }
        else if (message[0] == '*') {
            if (decode_req(message, aircraft, count) == 1) {
            break;
            }
        }
    }
    // printf("\nCurrently stored aircraft: %d\n", count);
// cheackng stored aircraft
// for (int i = 0; i < count; i++) {
//     printf("Aircraft ID: %d\n", aircraft[i].id);
//     printf("Time: %.1f\n", aircraft[i].time);
//     }
    // Free allocated memory
    free(aircraft);

    return 0;
}
