#include <stdio.h>
#include <string.h>
#include "requests.h"
#include "calculations.h"
// Handles incoming messages beginning with *.
// Decodes request time and command name
// Handles the close, est_pos, num_contacts, and check_separation commands
// Prints the required responses
// Calls functions from aircraft.c and calculations.c

int decode_req(char message[], ADSBPacket *aircraft, int count) {

    char command[30];
    int hour;
    int minute;

    // Pull out requested time and command
    if (sscanf(message, "*time:%d:%d,%29[^,\n]",
               &hour, &minute, command) != 3) {
        return 0;
    }

    // Convert request time into seconds
    double req_time = hour * 3600 + minute * 60;

    // closing COMMAND
    if (strcmp(command, "close") == 0) {
        printf("closing\n");
        return 1;
    }

    // The number of contacts command
    if (strcmp(command, "num_contacts") == 0) {

        int contacts = 0;

        for (int i = 0; i < count; i++) {

            double north;
            double east;

            // Calculate the aircraft position at the request time
            est_position(aircraft[i], req_time, &north, &east);

            // Check if aircraft is inside operating radius
            if (in_radius(north, east) == 1) {
                contacts++;
            }
        }

        printf("Currently tracking %d aircraft\n", contacts);
        return 0;
    }

    // Estimate the position command
    if (strcmp(command, "est_pos") == 0) {

        int requested_id;

        // Pull out the requested aircraft ID from the message
        if (sscanf(message,
                   "*time:%*d:%*d,est_pos,%d",
                   &requested_id) != 1) {
            return 0;
        }

        int found = 0;

        // Search for the requested aircraft
        for (int i = 0; i < count; i++) {

            if (aircraft[i].id == requested_id) {

                found = 1;

                double north;
                double east;

                // Calculate estimated position
                est_position(aircraft[i], req_time, &north, &east);

                // Check whether or not aircraft is inside the operating radius
                if (in_radius(north, east) == 1) {

                    printf(
                        "Aircraft (ID:%d): Estimated Position: N:%.1f,E:%.1f\n",
                        requested_id, north, east
                    );

                } else {

                    printf(
                        "Aircraft (ID:%d) not currently in area of operation\n",
                        requested_id
                    );
                }

                break;
            }
        }

        // Aircraft ID wasn't found
        if (found == 0) {
            printf(
                "Aircraft (ID:%d) not currently in area of operation\n",
                requested_id
            );
        }

        return 0;
    }

    // Checking seperation command
    if (strcmp(command, "check_separation") == 0) {

        int requested_id;
        double min_distance;
        int aircraft_index = -1;

        // Extract aircraft ID and minimum separation distance
        if (sscanf(message,
                   "*time:%*d:%*d,check_separation,%d,%lf",
                   &requested_id, &min_distance) != 2) {
            return 0;
        }

        // Searching for requested aircraft
        for (int i = 0; i < count; i++) {

            if (aircraft[i].id == requested_id) {
                aircraft_index = i;
                break;
            }
        }
        // If the aircraft is not found..
        if (aircraft_index == -1) {

            printf(
                "Aircraft (ID:%d) not currently in area of operation\n",
                requested_id
            );

            return 0;
        }

        // Calculate requested aircraft's estimated position
        double north;
        double east;

        est_position(aircraft[aircraft_index],
                     req_time, &north, &east);

        // Check whether aircraft is inside operating area
        if (in_radius(north, east) == 0) {

            printf(
                "Aircraft (ID:%d) not currently in area of operation\n",
                requested_id
            );

            return 0;
        }


        double earliest_time = -1.0;
        double issue_north = 0.0;
        double issue_east = 0.0;

        // Compare requested aircraft with every other aircraft
        for (int i = 0; i < count; i++) {

            // Skip the requested aircraft itself
            if (i == aircraft_index) {
                continue;
            }

            // Calculate the other aircraft's current position
            double other_north;
            double other_east;

            est_position(aircraft[i], req_time,
                        &other_north, &other_east);

            // Only compare aircraft currently inside the area
            if (in_radius(other_north, other_east) == 0) {
                continue;
            }

            double issue_time;

            // Check whether the two aircraft have a separation issue
            if (separation_check(
                    aircraft[aircraft_index],
                    aircraft[i],
                    req_time,
                    min_distance,
                    &issue_time) == 1) {

                // Calculate the requested aircraft's position
                // when the separation issue first occurs
                double future_north;
                double future_east;

                est_position(aircraft[aircraft_index],
                            req_time + issue_time,
                            &future_north, &future_east);

                // Ignore issues outside the ATC operating area
                if (in_radius(future_north, future_east) == 0) {
                    continue;
                }

                // Keep the earliest qualifying issue
                if (earliest_time < 0.0 || issue_time < earliest_time) {
                    earliest_time = issue_time;
                    issue_north = future_north;
                    issue_east = future_east;
                }
            }
        }

        // Output the separation result
        if (earliest_time >= 0.0) {
            printf("Separation issue: N:%.1f,E:%.1f\n",
                issue_north, issue_east);
        } else {
            printf("No separation issues\n");
        }


        return 0;
    }

    return 0;
}
