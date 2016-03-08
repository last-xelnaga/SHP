
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../external/libconfig/libconfig.h"
#include "../external/wiringPi/wiringPi.h"

void setup (
    void)
{
    if (geteuid() != 0)
    {
        printf("Need to be root to run\n");
        exit(0);
    }

    if (wiringPiSetup() == -1)
        exit(1);

    printf("OK\n") ;
}



int main (
    int argc,
    char **argv)
{
    config_t cfg;
    config_init(&cfg);

    // Read the file. If there is an error, report it and exit.
    if (!config_read_file(&cfg, "shp_client.cfg"))
    {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
                config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        return -1;
    }

    setup();

    config_destroy(&cfg);
    return 0;
}
