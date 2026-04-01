#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static GBitmap *s_bmp_mount;
static BitmapLayer *s_bmp_mount_layer;
static GFont s_font_time;
static GFont s_font_body;

static void update_time()
{
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_time_buffer[8];
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_time_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  update_time();
}

static void main_window_load(Window *window)
{
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_font_time = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_RENOGARE_44));
  s_font_body = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_RENOGARE_24));

  // Create the time TextLayer
  s_time_layer = text_layer_create(GRect(0, 24, bounds.size.w, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_font(s_time_layer, s_font_time);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Create the mountain bmp
  s_bmp_mount = gbitmap_create_with_resource(RESOURCE_ID_IMG_MOUNT);
  s_bmp_mount_layer = bitmap_layer_create(GRect(5, 5, bounds.size.w, bounds.size.h));
  bitmap_layer_set_compositing_mode(s_bmp_mount_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_bmp_mount_layer, s_bmp_mount);

  // Add layers to the Window
  layer_add_child(window_layer, bitmap_layer_get_layer(s_bmp_mount_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window)
{
  text_layer_destroy(s_time_layer);
  gbitmap_destroy(s_bmp_mount);
  bitmap_layer_destroy(s_bmp_mount_layer);
  fonts_unload_custom_font(s_font_time);
  fonts_unload_custom_font(s_font_body);
}

static void init()
{
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set the background color
  window_set_background_color(s_main_window, GColorBlack);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers){
                                                .load = main_window_load,
                                                .unload = main_window_unload});

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  update_time();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit()
{
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
}
