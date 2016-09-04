#pragma once

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
#define KEY_FONT_INDEX 10

bool change;
GColor textcolor;
GColor textaccentcolor;
GColor backgroundcolor;
GFont time_font;
int fontindex;

Window *window;
TextLayer* time_layer[25];

void set_style();
void inbox_received_callback(DictionaryIterator *iterator, void *context);
void inbox_dropped_callback(AppMessageResult reason, void *context);
void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
void outbox_sent_callback(DictionaryIterator *iterator, void *context);

extern const int fonts[];

extern uint32_t const segments[];
VibePattern pat;
