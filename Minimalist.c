// First Pebble project

#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_day_layer;

static void update_time(){
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";
  static char date_buffer[10];
  static char day_buffer[10];

  // Write the current hours and minutes into the buffer
  if (clock_is_24h_style() == true) {
      // Use 24h format
      strftime (buffer, sizeof("00:00"), "%H:%M", tick_time);
    } else {
    // Use 12h format
    strftime (buffer, sizeof("00:00"), "%I:%M", tick_time);
    strftime (day_buffer, sizeof(day_buffer), "%a" , tick_time);
    strftime (date_buffer, sizeof(date_buffer), "%b %e", tick_time);
    text_layer_set_text(s_date_layer, date_buffer);
  }
    // Display this time on the textlayer
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(s_date_layer, date_buffer);
  text_layer_set_text(s_day_layer, day_buffer);

}

static void main_window_load(Window *window) {
  // Create time text layer
  s_time_layer = text_layer_create(GRect(0, 0 , 144, 250));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorClear);
  text_layer_set_text(s_time_layer, "00:00");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

  // Day stuff
  s_day_layer = text_layer_create(GRect(0, 60, 144 , 80));
  text_layer_set_background_color(s_day_layer, GColorBlack);
  text_layer_set_text_color(s_day_layer, GColorClear);

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_day_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(s_day_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_day_layer));

  // Date stuff
  s_date_layer = text_layer_create(GRect(0, 112, 144, 80));
  text_layer_set_background_color(s_date_layer, GColorBlack);
  text_layer_set_text_color(s_date_layer, GColorClear);

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));

}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_unload(Window *window) {
  // Destroy textlayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_day_layer);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload

  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Register with TimeTickerService
  tick_timer_service_subscribe (MINUTE_UNIT, tick_handler);

  // Make sure the time is displayed from the start
  update_time();
}

static void deinit() {
  //Destroy Window
  window_destroy(s_main_window);
}

int main (void) {
  init();
  app_event_loop();
  deinit();
}
