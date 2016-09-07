#include <pebble.h>
#include "configuration.h"
#include "draw.h"

const int fonts[] = {
  RESOURCE_ID_FONT_SCIFLY_30, // http://www.dafont.com/it/scifly.font
  RESOURCE_ID_FONT_RABBIT_30,
  RESOURCE_ID_FONT_SINNER_30, // http://www.dafont.com/sinner.font
  RESOURCE_ID_FONT_TWODE_30,
};

int layout[5] = {ROW_BATTERY,ROW_CURRENT_TIME,ROW_DAY_DATE,ROW_MONTH_DATE,ROW_STEPS};

uint32_t const segments[] = { 1000, 500, 1000 };
VibePattern pat = {
  .durations = segments,
  .num_segments = ARRAY_LENGTH(segments),
};

void set_style() {
  backgroundcolor = GColorBlack;
  textcolor = GColorWhite;  
  textaccentcolor = GColorWhite;
  
  int random = 0;
  if(persist_exists(KEY_FONT_INDEX)) {
    fontindex = persist_read_int(KEY_FONT_INDEX);
    time_font = fonts_load_custom_font(resource_get_handle(fonts[fontindex]));
  }
  if(persist_exists(KEY_LAYOUT_ROW_0)) {
    layout[0] = persist_read_int(KEY_LAYOUT_ROW_0);
  }
  if(persist_exists(KEY_LAYOUT_ROW_1)) {
    layout[1] = persist_read_int(KEY_LAYOUT_ROW_1);
  }
  if(persist_exists(KEY_LAYOUT_ROW_2)) {
    layout[2] = persist_read_int(KEY_LAYOUT_ROW_2);
  }
  if(persist_exists(KEY_LAYOUT_ROW_3)) {
    layout[3] = persist_read_int(KEY_LAYOUT_ROW_3);
  }
  if(persist_exists(KEY_LAYOUT_ROW_4)) {
    layout[4] = persist_read_int(KEY_LAYOUT_ROW_4);
  }
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
    textcolor  = GColorFromRGB(stred, stgreen, stblue);
    textaccentcolor = GColorFromRGB(ndred, ndgreen, ndblue);
  }
  #endif
  window_set_background_color(window, backgroundcolor);
  for(int i = 0; i<5; i++){
    change = !change;
    for(int j = 0; j<5; j++){
      text_layer_set_font(time_layer[i*5+j], time_font);
      if (!(j % 2)) {
        change = !change;
      }
      change = random ? rand() % 2 : false;
      if (i==1 || change) {
        text_layer_set_text_color(time_layer[i*5+j], textaccentcolor);
      } else {
        text_layer_set_text_color(time_layer[i*5+j], textcolor);
      }
    }
  }
}

void inbox_received_callback(DictionaryIterator *iterator, void *context) {
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
  Tuple *font_index_t = dict_find(iterator, KEY_FONT_INDEX);
  Tuple *layout_row_0 = dict_find(iterator, KEY_LAYOUT_ROW_0);
  Tuple *layout_row_1 = dict_find(iterator, KEY_LAYOUT_ROW_1);
  Tuple *layout_row_2 = dict_find(iterator, KEY_LAYOUT_ROW_2);
  Tuple *layout_row_3 = dict_find(iterator, KEY_LAYOUT_ROW_3);
  Tuple *layout_row_4 = dict_find(iterator, KEY_LAYOUT_ROW_4);
  Tuple *random_t = dict_find(iterator, KEY_RANDOM);
    
  if(color_st_red_t) {
    int fontindex  = font_index_t->value->int32;
    layout[0] = layout_row_0->value->int32;
    layout[1] = layout_row_1->value->int32;
    layout[2] = layout_row_2->value->int32;
    layout[3] = layout_row_3->value->int32;
    layout[4] = layout_row_4->value->int32;
    time_font = fonts_load_custom_font(resource_get_handle(fonts[fontindex]));
    persist_write_int(KEY_FONT_INDEX, fontindex);
    persist_write_int(KEY_LAYOUT_ROW_0, layout[0]);
    persist_write_int(KEY_LAYOUT_ROW_1, layout[1]);
    persist_write_int(KEY_LAYOUT_ROW_2, layout[2]);
    persist_write_int(KEY_LAYOUT_ROW_3, layout[3]);
    persist_write_int(KEY_LAYOUT_ROW_4, layout[4]);
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
      textcolor = GColorFromRGB(stred, stgreen, stblue);
      textaccentcolor = GColorFromRGB(ndred, ndgreen, ndblue);
    
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
  draw_time();
  set_style();
}
void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}