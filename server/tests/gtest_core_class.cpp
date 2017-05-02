#include <gtest/gtest.h>
#include "core_class.hpp"

TEST (register_probe, positive_register_probe)
{
    core_class core;
    core.register_probe (0, "test probe", 0);
}
