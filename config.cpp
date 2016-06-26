
#include <string.h>
#include "libconfig.h"

#include "config.hpp"
#include "debug.hpp"

#define NAME_LENGTH         32
#define IP4_ADDRESS_LENGTH  15
#define TYPE_LENGTH         16

config_class* config_class::p_instance = 0;

config_class::config_class (
        void)
{
    // general
    general.name = "test client 1";

    // network settings
    network.address = "127.0.0.1";
    network.port = 3456;
    network.keep_alive_timeout = 0;
    network.connect_retry_count = 3;
    network.connect_retry_sleep = 2;
    network.write_timeout = 10;
    network.read_timeout = 10;
}

error_code_t config_class::read_string (
        config_t* p_cfg,
        const char* p_param_name,
        /*char* p_holder*/std::string* p_holder,
        unsigned int holder_length)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    const char* str;
    int res = config_lookup_string (p_cfg, p_param_name, &str);
    if (res == 0)
    {
        DEBUG_LOG_MESSAGE ("%s is not set\n", p_param_name);
        result = RESULT_INVALID_PARAM;
    }
    else
    {
        //strncpy (p_holder, str, holder_length);
        *p_holder = str;
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t config_class::read_number (
        config_t* p_cfg,
        const char* p_param_name,
        unsigned int* p_holder)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    int value;
    int res = config_lookup_int (p_cfg, p_param_name, &value);
    if (res == 0)
    {
        DEBUG_LOG_MESSAGE ("%s is not set\n", p_param_name);
        result = RESULT_INVALID_PARAM;
    }
    else
    {
        *p_holder = (unsigned int) value;
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t config_class::read_general (
        config_t* p_cfg)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    read_string (p_cfg, "general.name", &general.name, NAME_LENGTH);

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t config_class::read_network (
        config_t* p_cfg)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    read_string (p_cfg, "network.address", &network.address, IP4_ADDRESS_LENGTH);
    read_number (p_cfg, "network.port", &network.port);
    read_number (p_cfg, "network.keep_alive_timeout", &network.keep_alive_timeout);
    read_number (p_cfg, "network.connect_retry_count", &network.connect_retry_count);
    read_number (p_cfg, "network.connect_retry_sleep", &network.connect_retry_sleep);
    read_number (p_cfg, "network.write_timeout", &network.write_timeout);
    read_number (p_cfg, "network.read_timeout", &network.read_timeout);

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t config_class::read_sensors (
        config_t* p_cfg)
{
    error_code_t result = RESULT_OK;
    config_setting_t* p_setting = NULL;

    DEBUG_LOG_TRACE_BEGIN

    if (result == RESULT_OK)
    {
        p_setting = config_lookup (p_cfg, "sensors");
        if (p_setting == NULL)
        {
            DEBUG_LOG_MESSAGE ("bad config file. sensors are not listed\n");
            result = RESULT_INVALID_PARAM;
        }
    }

    if (result == RESULT_OK)
    {
        int count = config_setting_length (p_setting);

        for (int i = 0; i < count; ++i)
        {
            config_setting_t* item = config_setting_get_elem (p_setting, i);

            const char* name, *type;
            int gpio;

            if (!(config_setting_lookup_string (item, "name", &name)
                && config_setting_lookup_int (item, "gpio", &gpio)
                && config_setting_lookup_string (item, "type", &type)))
                continue;

            sensor_settings_t sensor;

            sensor.name = name;
            sensor.gpio = gpio;
            sensor.type = type;
            sensors.push_back (sensor);
        }

        /*sensor_settings_t sensor;

        sensor.name = "motion sensor";
        sensor.gpio = 3;
        sensor.type = "PIR";
        sensors.push_back (sensor);

        sensor.name = "alarm";
        sensor.gpio = 22;
        sensor.type = "LED";
        sensors.push_back (sensor);*/
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

error_code_t config_class::read_config (
        const char* p_cfg_filename)
{
    error_code_t result = RESULT_OK;
    config_t cfg;
    config_init (&cfg);

    DEBUG_LOG_TRACE_BEGIN

    //
    if (result == RESULT_OK)
    {
        int res = config_read_file (&cfg, p_cfg_filename);
        if (res == 0)
        {
            DEBUG_LOG_MESSAGE ("%s:%d - %s\n", config_error_file (&cfg),
                    config_error_line (&cfg), config_error_text (&cfg));
            result = RESULT_SYSTEM_ERROR;
        }
    }

    if (result == RESULT_OK)
    {
        result = read_general (&cfg);
    }

    if (result == RESULT_OK)
    {
        result = read_network (&cfg);
    }

    if (result == RESULT_OK)
    {
        result = read_sensors (&cfg);
    }

    config_destroy (&cfg);

    DEBUG_LOG_TRACE_END (result)
    return result;
}

config_class::~config_class (
        void)
{

}
