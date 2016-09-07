#pragma once
#include <pebble.h>

int health_get_step_average();
int health_get_sum_today(HealthMetric metric);
int get_step_goal();