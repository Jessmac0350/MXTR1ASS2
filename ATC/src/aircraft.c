#include <stdio.h>

#include "message.h"
// Handles ADS-B data and the aircraft you're tracking.
// Extracts aircraft ID, time, coordinates, altitude, heading and speed using sscanf()
// Stores decoded info in ADSBPacket
// Adds aircraft to tracking data structure
// Updates existing aircraft information
// Removes aircraft no longer being tracked
// Manages the memory used for aircraft records

int decode_aircraft(char message[], ADSBPacket *aircraft){
        int Aircraft_ID;
        int hour;
        int minute;
        double north_position;
        double east_position;
        int altitude;
        double heading;
        double speed;

        if(sscanf(message, "#ADS-B:%d", &Aircraft_ID) == 1) {
            //Check stored correctly
            // printf("Aircraft ID: %d\n", Aircraft_ID);
        } 
        else {
             printf("Error parsing aircraft ID.\n");
        }

        if (sscanf(message, "#ADS-B:%*d,time:%d:%d", &hour, &minute) == 2) {
            // printf("Time: %02d:%02d\n", hour, minute);
        } 
        else {
             printf("Error parsing time.\n");
        }

        if (sscanf(message, "#ADS-B:%*d,time:%*d:%*d,N:%lf,E:%lf", &north_position, &east_position) == 2) {
            // printf("Coordinates: N: %.1f, E: %.1f\n", north_position, east_position);
        } 
        else {
            printf("Error parsing coordinates.\n");
        }

        if (sscanf(message, "#ADS-B:%*d,time:%*d:%*d,N:%*lf,E:%*lf,alt:%d,head:%lf,speed:%lf", &altitude, &heading, &speed) == 3) {
            // printf("Altitude: %d\n", altitude);
            // printf("Heading: %.1f\n", heading);
            // printf("Speed: %.1f\n", speed);

        } 
        else {
            printf("Error parsing altitude, heading, or speed.\n");
        }

        //declaring packet
        aircraft->id = Aircraft_ID;
        aircraft->time = hour * 3600 + minute * 60; // Convert time to seconds??
        aircraft->north = north_position;
        aircraft->east = east_position;
        aircraft->altitude = altitude;
        aircraft->heading = heading;
        aircraft->speed = speed; // Convert speed from km/h to m/s
    // Check stored values of packet
        // printf("Aircraft ID: %d\n", aircraft->id);
        // printf("Time: %.1f\n", aircraft->time);
        // printf("North: %.1f\n", aircraft->north);
        // printf("East: %.1f\n", aircraft->east);
        // printf("Altitude: %d\n", aircraft->altitude);
        // printf("Heading: %.1f\n", aircraft->heading);
        // printf("Speed: %.1f\n", aircraft->speed);
        
        return 0;
    }
    
        