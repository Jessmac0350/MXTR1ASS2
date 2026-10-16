#include <stdio.h>
// Contains calculations used by the other files.
// Calculates aircraft velocity components, estimated positions, distances, and separation issues.
// - Is aircraft is inside the 350 km radius?
// - The relative aircraft positions and velocities.

#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include "message.h"
int in_radius(double north, double east);

void est_position(ADSBPacket aircraft, double req_time,double *north, double *east);

int separation_check(
    ADSBPacket aircrafta,
    ADSBPacket aircraftb,
    double req_time,
    double min_distance,
    double *issue_time
);


#endif