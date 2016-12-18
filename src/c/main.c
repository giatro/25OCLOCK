#include <pebble.h>
#include "configuration.h"
#include "draw.h"
#include "healt.h"
#include "battery.h"

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  set_style();
  draw_time();
}

void handle_bluetooth(bool connected) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "BT");
  if(persist_exists(KEY_BT)) {
    if(persist_read_int(KEY_BT)) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "NOTIFYBT");
      vibes_enqueue_custom_pattern(pat);
      draw_time();  
    } else {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "IGNOREBT");
    };
  }
}

static void init() {
  time_font = fonts_load_custom_font(resource_get_handle(fonts[0]));
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = handle_bluetooth
  });
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  // Color config
  backgroundcolor  = GColorBlack;
  textcolor  = GColorWhite;  
  textaccentcolor = GColorWhite;

  window = window_create();
  window_stack_push(window, true);
  window_set_background_color(window, backgroundcolor);
  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  int boundw = (int)bounds.size.w - 4;
  int boundh = (int)bounds.size.h - 3;
  for(int i = 0; i<5; i++){
    for(int j = 0; j<5; j++){
      time_layer[i*5+j] = text_layer_create(GRect(j*boundw/5+1, i*boundh/5-4, boundw/5+2, boundh/5+3));
      text_layer_set_text_color(time_layer[i*5+j], textcolor);
      text_layer_set_font(time_layer[i*5+j], time_font);
      text_layer_set_text_alignment(time_layer[i*5+j], GTextAlignmentCenter);
      text_layer_set_background_color(time_layer[i*5+j], GColorClear);
      layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer[i*5+j]));
    }
  }
  for(int i=0; i<25; i++){
    strcpy(grid[i], " ");
  }
  
  draw_time();
  set_style();
  //battery_init();
  
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}

static void deinit() {
  connection_service_unsubscribe();
  for(int i = 0; i<25; i++){
    text_layer_destroy(time_layer[i]);
  }
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}