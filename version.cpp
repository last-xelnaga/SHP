
#include "version.hpp"

#include <stdio.h>


double version = {
    #include "VERSION"
};

unsigned int revision = {
    #include "REVISION"
};

const char* get_full_version (
        void)
{
    static char p_version [16] = { 0 };
    snprintf (p_version, sizeof (p_version), "%02.02f.%d", version, revision);
    return p_version;
}

unsigned int get_revision (
        void)
{
    return revision;
}
