
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "sensor_manager.hpp"
#include "../external/wiringPi/wiringPi.h"

sensor_manager_class *sensor_manager_class::p_instance = 0;

sensor_manager_class::sensor_manager_class(
    void)
{

}

int sensor_manager_class::setup (
    void)
{
    int result = 0;

    if (geteuid () != 0)
    {
        printf ("Need to be root to run\n");
        result = -1;
    }

    if (wiringPiSetup () == -1)
    {
        printf ("wiringPiSetup failed\n");
        result = -1;
    }

    return result;
} 
