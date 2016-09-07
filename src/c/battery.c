#include "battery.h"
int get_battery_charge_percent() {
  BatteryChargeState charge_state = battery_state_service_peek();
  return charge_state.charge_percent;
}