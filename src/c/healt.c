#include "healt.h"
int health_get_sum_today(HealthMetric metric) {
  return (int)health_service_sum_today(metric);
}

int health_get_step_average() {
  const time_t start = time_start_of_today();
  const time_t end = time(NULL);
  return (int)health_service_sum_averaged(HealthMetricStepCount, start, end, HealthServiceTimeScopeDaily);
}

int get_step_goal() {
  const time_t start = time_start_of_today();
  const time_t end = start + SECONDS_PER_DAY;
  return (int)health_service_sum_averaged(HealthMetricStepCount, start, end, HealthServiceTimeScopeDaily);
}