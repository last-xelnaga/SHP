
#include <string.h>

#include "config.hpp"
#include "external/libconfig/libconfig.h"

Config *Config::p_instance = 0;

Config::Config(
    void)
{
    strcpy(p_network_address, "127.0.0.1");
    network_port = 3500;
}

int Config::read_config(
    const char* p_cfg_filename)
{
    config_t cfg;
    config_init(&cfg);

    // Read the file. If there is an error, report it and exit.
    if (!config_read_file(&cfg, p_cfg_filename))
    {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
                config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        return -1;
    }

    config_destroy(&cfg);
    return 0;
}

char* Config::read_network_address(
    void)
{
    return p_network_address;
}

unsigned int Config::read_network_port(
    void)
{
    return network_port;
}
