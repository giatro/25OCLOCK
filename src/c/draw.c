#include <pebble.h>
#include "configuration.h"
#include "draw.h"

char grid[25][2];
char textrows[5][32] = {"ABCDE","FGHIJ","KLMNO","PQRST","UVWXY"};

void draw_time(){
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);
  
  if(clock_is_24h_style()==true){
    strftime(textrows[0], sizeof(textrows[0]), "%H:%M", tick_time);
  }else{
    strftime(textrows[0], sizeof(textrows[0]), "%I:%M", tick_time);
  }
  // strftime(textrows[1], sizeof(textrows[0]), "%d%a", tick_time);
  // strftime(textrows[2], sizeof(textrows[0]), "%b%y", tick_time);
  //strftime(textrows[1], sizeof(textrows[0]), "%d%b", tick_time);
  //strftime(textrows[2], sizeof(textrows[0]), "%A", tick_time);
  strftime(textrows[1], sizeof(textrows[0]), "%a%d", tick_time);
  strftime(textrows[2], sizeof(textrows[0]), "%B", tick_time);
  if(!connection_service_peek_pebble_app_connection()) {
    strftime(textrows[4], sizeof(textrows[0]), "NOBT!", tick_time);
  } else {
    strftime(textrows[4], sizeof(textrows[0]), "UVWXY", tick_time);
  }
  
  for(int rowindex = 0; rowindex < 5; rowindex++) {
    for(int i=0; i<(int)(strlen(textrows[rowindex])); i++){
      if(textrows[rowindex][i]>96)textrows[rowindex][i]-=32;
    }
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