#include <pebble.h>
#define KEY_BG_COLOR_RED 0
#define KEY_BG_COLOR_GREEN 1 
#define KEY_BG_COLOR_BLUE 2
#define KEY_ST_COLOR_RED 3
#define KEY_ST_COLOR_GREEN 4 
#define KEY_ST_COLOR_BLUE 5
#define KEY_ND_COLOR_RED 6
#define KEY_ND_COLOR_GREEN 7 
#define KEY_ND_COLOR_BLUE 8
#define KEY_RANDOM 9

Window *window;
TextLayer* time_layer[25];
char grid[25][2];
char textrows[5][32] = {"ABCDE","FGHIJ","KLMNO","PQRST","UVWXY"};

GFont time_fonts[5];
static const int fonts[5] = {
  RESOURCE_ID_FONT_OPS_30,
  RESOURCE_ID_FONT_ORBITRON_24,
  RESOURCE_ID_FONT_SCIFLY_30,
  RESOURCE_ID_FONT_SINNER_30,
//  RESOURCE_ID_FONT_ORG_33  
};
int fontindex = 3;

bool change = true;
GColor foregroundcolor;
GColor foregroundcolor2;
GColor backgroundcolor;

uint32_t const segments[] = { 1000, 500, 1000 };
VibePattern pat = {
  .durations = segments,
  .num_segments = ARRAY_LENGTH(segments),
};

char *upcase(char *str)
{
for (int i = 0; str[i] != 0; i++) {
if (str[i] >= 'a' && str[i] <= 'z') {
str[i] -= 0x20;
}
}

return str;
}

static void set_color() {
  backgroundcolor = GColorBlack;
  foregroundcolor = GColorWhite;  
  foregroundcolor2 = GColorWhite;
  
  int random = 0;
  #ifdef PBL_COLOR
  if(persist_exists(KEY_ST_COLOR_RED)) {
    int bgred   = persist_read_int(KEY_BG_COLOR_RED);
    int bggreen = persist_read_int(KEY_BG_COLOR_GREEN);
    int bgblue  = persist_read_int(KEY_BG_COLOR_BLUE);
    int stred   = persist_read_int(KEY_ST_COLOR_RED);
    int stgreen = persist_read_int(KEY_ST_COLOR_GREEN);
    int stblue  = persist_read_int(KEY_ST_COLOR_BLUE);
    int ndred   = persist_read_int(KEY_ND_COLOR_RED);
    int ndgreen = persist_read_int(KEY_ND_COLOR_GREEN);
    int ndblue  = persist_read_int(KEY_ND_COLOR_BLUE);
    random  = persist_read_int(KEY_RANDOM);
    backgroundcolor  = GColorFromRGB(bgred, bggreen, bgblue);
    foregroundcolor  = GColorFromRGB(stred, stgreen, stblue);
    foregroundcolor2 = GColorFromRGB(ndred, ndgreen, ndblue);
  }
  #endif
  window_set_background_color(window, backgroundcolor);
  for(int i = 0; i<5; i++){
    change = !change;
    for(int j = 0; j<5; j++){
      if (!(j % 2)) {
        change = !change;
      }
      change = random ? rand() % 2 : change;
      if (change) {
        text_layer_set_text_color(time_layer[i*5+j], foregroundcolor);
      } else {
        text_layer_set_text_color(time_layer[i*5+j], foregroundcolor2);
      }
    }
  }
}

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

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  set_color();
  draw_time();
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Color
  Tuple *color_bg_red_t = dict_find(iterator, KEY_BG_COLOR_RED);
  Tuple *color_bg_green_t = dict_find(iterator, KEY_BG_COLOR_GREEN);
  Tuple *color_bg_blue_t = dict_find(iterator, KEY_BG_COLOR_BLUE);
  Tuple *color_st_red_t = dict_find(iterator, KEY_ST_COLOR_RED);
  Tuple *color_st_green_t = dict_find(iterator, KEY_ST_COLOR_GREEN);
  Tuple *color_st_blue_t = dict_find(iterator, KEY_ST_COLOR_BLUE);
  Tuple *color_nd_red_t = dict_find(iterator, KEY_ND_COLOR_RED);
  Tuple *color_nd_green_t = dict_find(iterator, KEY_ND_COLOR_GREEN);
  Tuple *color_nd_blue_t = dict_find(iterator, KEY_ND_COLOR_BLUE);
  Tuple *random_t = dict_find(iterator, KEY_RANDOM);
  if(color_st_red_t) {
    #ifdef PBL_COLOR
      int bgred   = color_bg_red_t->value->int32;
      int bggreen = color_bg_green_t->value->int32;
      int bgblue  = color_bg_blue_t->value->int32;
      int stred   = color_st_red_t->value->int32;
      int stgreen = color_st_green_t->value->int32;
      int stblue  = color_st_blue_t->value->int32;
      int ndred   = color_nd_red_t->value->int32;
      int ndgreen = color_nd_green_t->value->int32;
      int ndblue  = color_nd_blue_t->value->int32;
      int random  = random_t->value->int32;;
      backgroundcolor = GColorFromRGB(bgred, bggreen, bgblue);
      foregroundcolor = GColorFromRGB(stred, stgreen, stblue);
      foregroundcolor2 = GColorFromRGB(ndred, ndgreen, ndblue);
    
      // Persist values
      persist_write_int(KEY_BG_COLOR_RED, bgred);
      persist_write_int(KEY_BG_COLOR_GREEN, bggreen);
      persist_write_int(KEY_BG_COLOR_BLUE, bgblue);
      persist_write_int(KEY_ST_COLOR_RED, stred);
      persist_write_int(KEY_ST_COLOR_GREEN, stgreen);
      persist_write_int(KEY_ST_COLOR_BLUE, stblue);
      persist_write_int(KEY_ND_COLOR_RED, ndred);
      persist_write_int(KEY_ND_COLOR_GREEN, ndgreen);
      persist_write_int(KEY_ND_COLOR_BLUE, ndblue);
      persist_write_int(KEY_RANDOM, random);
    #endif
  }
  set_color();
}
static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void handle_bluetooth(bool connected) {
  vibes_enqueue_custom_pattern(pat);
  draw_time();
}

static void init() {
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
  foregroundcolor  = GColorWhite;  
  foregroundcolor2 = GColorWhite;

  window = window_create();
  window_stack_push(window, true);
  window_set_background_color(window, backgroundcolor);
  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  int boundw = (int)bounds.size.w - 4;
  int boundh = (int)bounds.size.h - 3;
  for(int i = 0; i<5; i++){
    for(int j = 0; j<5; j++){
      time_layer[i*5+j] = text_layer_create(GRect(j*boundw/5, i*boundh/5, boundw/5, boundh/5));
      text_layer_set_text_color(time_layer[i*5+j], foregroundcolor);
      text_layer_set_font(time_layer[i*5+j], fonts_load_custom_font(resource_get_handle(fonts[fontindex])));
      text_layer_set_text_alignment(time_layer[i*5+j], GTextAlignmentCenter);
      text_layer_set_background_color(time_layer[i*5+j], GColorClear);
      layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer[i*5+j]));
    }
  }
  for(int i=0; i<25; i++){
    strcpy(grid[i], " ");
  }
  
  draw_time();
  set_color();
  
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