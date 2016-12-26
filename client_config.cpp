
#include "client_config.hpp"
#include "debug.hpp"
#include <libconfig.h>
#include <string.h>
#include <stdlib.h>

#define CFG_FILE_NAME       "shp_client.cfg"

static unsigned char is_initialized = 0;
static general_settings_t general_settings =
{
    1, // client_id
    "test client" // p_name
};

static network_settings_t network_settings =
{
    "127.0.0.1", // p_address
    3456, // port;
    3, //connect_retry_count;
    2, //connect_retry_sleep;
    10, //write_timeout;
    10, //read_timeout;
    0, //keep_alive_timeout;
};

static sensors_settings_t sensors_settings =
{
    0, // sensors_count;
    NULL, // p_sensors;
};


static error_code_t read_string (
        const config_t* p_cfg,
        const char* p_param_name,
        char* p_holder,
        const unsigned int holder_length)
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
        strncpy (p_holder, str, holder_length);
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

static error_code_t read_number (
        const config_t* p_cfg,
        const char* p_param_name,
        unsigned int* p_holder)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    int value;
    int res = config_lookup_int (p_cfg, p_param_name, &value);
    if (res == 0)
    {
        DEBUG_LOG_MESSAGE ("%s is not set", p_param_name);
        result = RESULT_INVALID_PARAM;
    }
    else
    {
        *p_holder = (unsigned int) value;
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}

static error_code_t read_general (
        const config_t* p_cfg)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    read_number (p_cfg, "general.client_id", &general_settings.client_id);
    read_string (p_cfg, "general.name", general_settings.p_name, NAME_LENGTH);

    DEBUG_LOG_TRACE_END (result)
    return result;
}

static error_code_t read_network (
        const config_t* p_cfg)
{
    error_code_t result = RESULT_OK;
    DEBUG_LOG_TRACE_BEGIN

    read_string (p_cfg, "network.address", network_settings.p_address, IP4_ADDRESS_LENGTH);
    read_number (p_cfg, "network.port", &network_settings.port);
    read_number (p_cfg, "network.keep_alive_timeout", &network_settings.keep_alive_timeout);
    read_number (p_cfg, "network.connect_retry_count", &network_settings.connect_retry_count);
    read_number (p_cfg, "network.connect_retry_sleep", &network_settings.connect_retry_sleep);
    read_number (p_cfg, "network.write_timeout", &network_settings.write_timeout);
    read_number (p_cfg, "network.read_timeout", &network_settings.read_timeout);

    DEBUG_LOG_TRACE_END (result)
    return result;
}

static error_code_t read_sensors (
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
            DEBUG_LOG_MESSAGE ("bad config file. sensors are not listed");
            result = RESULT_INVALID_PARAM;
        }
    }

    if (result == RESULT_OK)
    {
        int count = config_setting_length (p_setting);
        sensors_settings.p_sensors = (sensor_settings_t*) malloc (count * sizeof (sensor_settings_t));
        sensors_settings.sensors_count = count;
    }

    if (result == RESULT_OK)
    {
        for (unsigned int i = 0; i < sensors_settings.sensors_count; i ++)
        {
            config_setting_t* item = config_setting_get_elem (p_setting, i);

            const char* name, *type;
            int gpio;

            if (!(config_setting_lookup_string (item, "name", &name)
                    && config_setting_lookup_int (item, "gpio", &gpio)
                    && config_setting_lookup_string (item, "type", &type)))
                continue;

            sensor_settings_t* p_sensor = &sensors_settings.p_sensors[i];

            p_sensor->is_initialized = 1;
            p_sensor->gpio = gpio;
            strncpy (p_sensor->p_name, name, NAME_LENGTH);
            strncpy (p_sensor->p_type, type, TYPE_LENGTH);
        }
    }

    DEBUG_LOG_TRACE_END (result)
    return result;
}


error_code_t read_config (
        void)
{
    error_code_t result = RESULT_OK;
    config_t cfg;
    config_init (&cfg);
    DEBUG_LOG_TRACE_BEGIN

    //
    if (result == RESULT_OK)
    {
        int res = config_read_file (&cfg, CFG_FILE_NAME);
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

general_settings_t* get_general_settings (
        void)
{
    if (is_initialized == 0)
    {
        read_config ();
        is_initialized = 1;
    }
    return &general_settings;
}

network_settings_t* get_network_settings (
        void)
{
    if (is_initialized == 0)
    {
        read_config ();
        is_initialized = 1;
    }
    return &network_settings;
}

sensors_settings_t* get_sensor_settings (
        void)
{
    if (is_initialized == 0)
    {
        read_config ();
        is_initialized = 1;
    }
    return &sensors_settings;
}

void destroy_config (
        void)
{
    free (sensors_settings.p_sensors);
    sensors_settings.p_sensors = NULL;
}
