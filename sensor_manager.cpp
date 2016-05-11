
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "wiringPi.h"
#include "sensor_manager.hpp"
#include "debug.hpp"

//#include "sensors/sensor_button.hpp"
#include "sensors/sensor_led.hpp"
//#include "sensors/sensor_dht11.hpp"
//#include "sensors/sensor_relay.hpp"
#include "sensors/sensor_pir.hpp"


sensor_manager_class *sensor_manager_class::p_instance = 0;

sensor_manager_class::sensor_manager_class (
        void)
{
    is_initialized = false;
}

error_code_t sensor_manager_class::setup_board (
        void)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    if (result == RESULT_OK)
    {
        if (is_initialized == true)
        {
            // early return ((
            DEBUG_LOG_TRACE_END (result)
            return result;
        }
    }

    if (result == RESULT_OK)
    {
        if (geteuid () != 0)
        {
            DEBUG_LOG_MESSAGE ("need to be root to run. exit\n");
            result = RESULT_INVALID_STATE;
        }
    }

    if (result == RESULT_OK)
    {
        int res = wiringPiSetup ();
        if (res == -1)
        {
            DEBUG_LOG_MESSAGE ("board setup has failed. exit\n");
            result = RESULT_SYSTEM_ERROR;
        }
    }

    if (result == RESULT_OK)
    {
        is_initialized = true;
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t sensor_manager_class::add_sensor (
        const char* p_name,
        const unsigned int gpio,
        const char* p_type)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    if (result == RESULT_OK)
    {
        result = setup_board ();
    }

    if (result == RESULT_OK)
    {
        if (strcmp (p_type, "LED") == 0)
        {
            sensor_led_class* p_status_led_pir = new sensor_led_class (gpio, p_name);
            p_status_led_pir->activate();
        }
        else if (strcmp (p_type, "PIR") == 0)
        {
            sensor_pir_class* p_pir = new sensor_pir_class (gpio, p_name);
            //p_pir->set_callback(pir_callback, p_status_led_pir);
            p_pir->activate();
        }
        else
        {
            DEBUG_LOG_MESSAGE ("unknown sensor type. skip\n");
        }
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}
