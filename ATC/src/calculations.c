#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "calculations.h"


void est_position(ADSBPacket aircraft, double req_time,
                       double *north, double *east) {

    // Convert heading from degrees to radians
    double radians = aircraft.heading * (3.141592653589793 / 180.0);

    // Calculate velocity components (m/s)
    double v_North= aircraft.speed * sin(radians);
    double v_East = aircraft.speed * cos(radians);

    // Calculate elapsed time (seconds)
    double time_change = req_time - aircraft.time;

    // Calculate estimated position (km)
    *north = aircraft.north + (v_North * time_change) / 1000.0;
    *east = aircraft.east + (v_East * time_change) / 1000.0;
}

int in_radius(double north, double east) {

    double distance = sqrt(north * north + east * east);

    if (distance <= 350.0) {
        return 1;
    }

    return 0;
}

int separation_check(
    ADSBPacket aircrafta,
    ADSBPacket aircraftb,
    double req_time,
    double min_distance,
    double *issue_time
){
    if (abs(aircrafta.altitude - aircraftb.altitude) > 750) {
        return 0;
    }
    // Estimate both aircraft positions at request time
    double n1, e1, n2, e2;

    est_position(aircrafta, req_time, &n1, &e1);
    est_position(aircraftb, req_time, &n2, &e2);

    // Calculate relative position (km)
    double dn = n2 - n1;
    double de = e2 - e1;

    // Convert headings to radians
    double theta1 = aircrafta.heading * M_PI / 180.0;
    double theta2 = aircraftb.heading * M_PI / 180.0;

    // Calculate velocity components (km/s)
    double vn1 = aircrafta.speed * sin(theta1) / 1000.0;
    double ve1 = aircrafta.speed * cos(theta1) / 1000.0;
    double vn2 = aircraftb.speed * sin(theta2) / 1000.0;
    double ve2 = aircraftb.speed * cos(theta2) / 1000.0;

    // Relative velocity
    double dvn = vn2 - vn1;
    double dve = ve2 - ve1;

    // Quadratic coefficients
    double a = dvn * dvn + dve * dve;
    double b = 2.0 * (dn * dvn + de * dve);
    double c = dn * dn + de * de
               - min_distance * min_distance;

    // Already within minimum separation
    if (c <= 0.0) {
        *issue_time = 0.0;
        return 1;
    }

    // No relative movement
    if (a < 1e-15) {
        return 0;
    }

    // Calculate discriminant
    double discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0.0) {
        return 0;
    }

    // First intersection time
    double t = (-b - sqrt(discriminant)) / (2.0 * a);

    if (t < 0.0) {
        return 0;
    }

    *issue_time = t;
    return 1;
}

