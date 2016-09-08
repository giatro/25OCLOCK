#include <pebble.h>
#include "configuration.h"
#include "draw.h"
#include "healt.h"
#include "battery.h"


char grid[25][2];
char textrows[5][32] = {"ABCDE","FGHIJ","KLMNO","PQRST","UVWXY"};

void draw_time(){
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);
  /*
  snprintf(textrows[0], sizeof(textrows[0]), "%04d%%", get_battery_charge_percent());
  if(clock_is_24h_style()==true){
    strftime(textrows[1], sizeof(textrows[0]), "%H:%M", tick_time);
  }else{
    strftime(textrows[1], sizeof(textrows[0]), "%I:%M", tick_time);
  }
  strftime(textrows[2], sizeof(textrows[0]), "%a%d", tick_time);
  strftime(textrows[3], sizeof(textrows[0]), "%B", tick_time);
 
  if(!connection_service_peek_pebble_app_connection()) {
    strftime(textrows[4], sizeof(textrows[4]), "NOBT!", tick_time);
  } else {
    snprintf(textrows[4], sizeof(textrows[4]), "%05d", health_get_sum_today(HealthMetricStepCount));
  }
  
  snprintf(textrows[3], sizeof(textrows[3]), "%05d", get_step_goal());
  */
  
  
  for(int rowindex = 0; rowindex < 5; rowindex++) {
    switch(layout[rowindex]) {
      case ROW_CURRENT_TIME:
        if(clock_is_24h_style()==true){
          strftime(textrows[rowindex], sizeof(textrows[rowindex]), "%H:%M", tick_time);
        }else{
          strftime(textrows[rowindex], sizeof(textrows[rowindex]), "%I:%M", tick_time);
        }
        break;
      case ROW_DAY_DATE:
        strftime(textrows[rowindex], sizeof(textrows[rowindex]), "%a%d", tick_time);
        break;
      case ROW_MONTH_DATE:
        strftime(textrows[rowindex], sizeof(textrows[rowindex]), "%b%d", tick_time);
        break;
      case ROW_MONTH:
        strftime(textrows[rowindex], sizeof(textrows[rowindex]), "%B", tick_time);
        break;
      case ROW_YEAR:
        strftime(textrows[rowindex], sizeof(textrows[rowindex]), "Y%Y", tick_time);
        break;
      case ROW_BATTERY:
        snprintf(textrows[rowindex], sizeof(textrows[rowindex]), "%04d%%", get_battery_charge_percent());
        break;
      case ROW_STEPS:
        snprintf(textrows[rowindex], sizeof(textrows[rowindex]), "%05d", health_get_sum_today(HealthMetricStepCount));
        break;
    }
    for(int i=0; i<(int)(strlen(textrows[rowindex])); i++){
      if(textrows[rowindex][i]>96)textrows[rowindex][i]-=32;
    }
  }
  
  if(!connection_service_peek_pebble_app_connection()) {
    strftime(textrows[4], sizeof(textrows[4]), "NOBT!", tick_time);
  } 
  
  for(int rowindex = 0; rowindex < 5; rowindex++) {
    for(int i=0; i<5; i++){
      char s[] = " ";
      s[0] = textrows[rowindex][i];
      strcpy(grid[5*rowindex+i], s);
      text_layer_set_text(time_layer[5*rowindex+i], grid[5*rowindex+i]);
    }
  }
}