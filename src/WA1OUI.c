#include "pebble.h"

Window      *window;

TextLayer   *text_wa1oui_layer;
TextLayer   *text_dayname_layer;
TextLayer   *text_date_layer;
TextLayer   *text_time_layer;
TextLayer   *text_degrees_layer;
TextLayer   *text_battery_layer;

Layer       *LineLayer;
Layer       *RedLineLayer;

Layer       *BTLayer1;

GFont		     fontRobotoCondensed21;
GFont        fontRobotoBoldSubset37;
GFont        fontRobotoBoldSubset49;
GFont        fontHelvNewLight20;

static char degreesstr[] = "====";
static int  batterychargepct;
static int  batterycharging = 0;
static int  BatteryVibesDone = 0;


GPoint     Linepoint;
static int BTConnected = 1;
static int BTVibesDone = 0;

GColor TextColorHold;
GColor BGColorHold;

void red_line_layer_update_callback(Layer *RedLineLayer, GContext* ctx) {

     #if PBL_COLOR
         graphics_context_set_fill_color(ctx, GColorRed);
     #else
         //B&W
         graphics_context_set_fill_color(ctx, TextColorHold);
     #endif

     graphics_fill_rect(ctx, layer_get_bounds(RedLineLayer), 0, GCornerNone);
}

void line_layer_update_callback(Layer *LineLayer, GContext* ctx) {

     graphics_context_set_fill_color(ctx, TextColorHold);
     graphics_fill_rect(ctx, layer_get_bounds(LineLayer), 3, GCornersAll);

     if (batterycharging == 1) {

       #ifdef PBL_COLOR
          graphics_context_set_fill_color(ctx, GColorBlue);
       #else
          graphics_context_set_fill_color(ctx, GColorBlack);
       #endif

          graphics_fill_rect(ctx, GRect(2, 1, 100, 4), 3, GCornersAll);

     } else if (batterychargepct > 20) {
       #ifdef PBL_COLOR
          graphics_context_set_fill_color(ctx, GColorGreen);
       #else
          graphics_context_set_fill_color(ctx, GColorBlack);
       #endif

          graphics_fill_rect(ctx, GRect(2, 1, batterychargepct, 4), 3, GCornersAll);

     } else {
      #ifdef PBL_COLOR
          graphics_context_set_fill_color(ctx, GColorRed);
       #else
          graphics_context_set_fill_color(ctx, GColorBlack);
       #endif


          graphics_fill_rect(ctx, GRect(2, 1, batterychargepct, 4), 3, GCornersAll);
     }
}

void handle_bluetooth(bool connected) {

    if (connected) {

         BTConnected = 1;     // Connected
         BTVibesDone = 0;

    } else {
         BTConnected = 0;      // Not Connected

         if (BTVibesDone == 0) {    //Do First Vibe
             BTVibesDone = 1;

             vibes_long_pulse();
         }
    }
    layer_mark_dirty(BTLayer1);
}

//BT Logo Callback;
void BTLine_update_callback(Layer *BTLayer1, GContext* BT1ctx) {

       GPoint BTLinePointStart;
       GPoint BTLinePointEnd;

       graphics_context_set_stroke_color(BT1ctx, TextColorHold);

      if (BTConnected == 0) {

        #ifdef PBL_COLOR
            graphics_context_set_stroke_color(BT1ctx, GColorRed);
            graphics_context_set_fill_color(BT1ctx, GColorWhite);
            graphics_fill_rect(BT1ctx, layer_get_bounds(BTLayer1), 0, GCornerNone);
        #else
            graphics_context_set_stroke_color(BT1ctx, GColorBlack);
            graphics_context_set_fill_color(BT1ctx, GColorWhite);
            graphics_fill_rect(BT1ctx, layer_get_bounds(BTLayer1), 0, GCornerNone);
        #endif

        // "X"" Line 1
        BTLinePointStart.x = 1;
        BTLinePointStart.y = 1;

        BTLinePointEnd.x = 20;
        BTLinePointEnd.y = 20;
        graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);

        // "X"" Line 2
        BTLinePointStart.x = 1;
        BTLinePointStart.y = 20;

        BTLinePointEnd.x = 20;
        BTLinePointEnd.y = 1;
        graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);
      }
      else
      {
       //Line 1
       BTLinePointStart.x = 10;
       BTLinePointStart.y = 1;

       BTLinePointEnd.x = 10;
       BTLinePointEnd.y = 20;
       graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);

       //Line 2
       BTLinePointStart.x = 10;
       BTLinePointStart.y = 1;

       BTLinePointEnd.x = 16;
       BTLinePointEnd.y = 5;
       graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);

       //Line 3
       BTLinePointStart.x = 4;
       BTLinePointStart.y = 5;

       BTLinePointEnd.x = 16;
       BTLinePointEnd.y = 16;
       graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);

       //Line 4
       BTLinePointStart.x = 4;
       BTLinePointStart.y = 16;

       BTLinePointEnd.x = 16;
       BTLinePointEnd.y = 5;
       graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);

       //Line 5
       BTLinePointStart.x = 10;
       BTLinePointStart.y = 20;

       BTLinePointEnd.x = 16;
       BTLinePointEnd.y = 16;
       graphics_draw_line(BT1ctx, BTLinePointStart, BTLinePointEnd);
      }
}

void handle_battery(BatteryChargeState charge_state) {

 static char BatteryPctTxt[] = "+100%";

  batterychargepct = charge_state.charge_percent;

  if (charge_state.is_charging) {
    batterycharging = 1;
  } else {
    batterycharging = 0;
  }

  // Reset if Battery > 20% ********************************
  if (batterychargepct > 20) {
     if (BatteryVibesDone == 1) {     //OK Reset to normal
         BatteryVibesDone = 0;
     }
     #ifdef PBL_COLOR
         text_layer_set_text_color(text_battery_layer, TextColorHold);
      #endif
  }

  //
  if (batterychargepct < 30) {
     if (BatteryVibesDone == 0) {            // Do Once
         BatteryVibesDone = 1;
         vibes_long_pulse();
      }
  }

   if (charge_state.is_charging) {
     strcpy(BatteryPctTxt, "Chrg");
  } else {
     snprintf(BatteryPctTxt, 5, "%d%%", charge_state.charge_percent);
  }
   text_layer_set_text(text_battery_layer, BatteryPctTxt);

  layer_mark_dirty(LineLayer);
}

void handle_appfocus(bool in_focus){
    if (in_focus) {
        handle_bluetooth(bluetooth_connection_service_peek());
        handle_battery(battery_state_service_peek());
    }
}
//       ******************** Main Loop *******************

void handle_tick(struct tm *tick_time, TimeUnits units_changed) {

  // Need to be static because they're used by the system later.
  static char dayname_text[] = "XXXXXXXXXX";
  static char time_text[] = "00:00";
  static char date_text[] = "Xxx 00       0000";

  int FirstTime = 0;

  char *time_format;

  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  // Set Time of Day
  strftime(time_text, sizeof(time_text), time_format, tick_time);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

  // Set day and date
  strftime(dayname_text, sizeof(dayname_text), "%A",        tick_time);
  strftime(date_text,    sizeof(date_text),    "%b %e,  %Y" , tick_time);

  //Initialize
  if (FirstTime == 0) {
    text_layer_set_text(text_dayname_layer, dayname_text);
    text_layer_set_text(text_date_layer, date_text);
    FirstTime = 1;
    }

  if (units_changed & DAY_UNIT) {
   // Only update the day name & date when it's changed.
    text_layer_set_text(text_dayname_layer, dayname_text);
    text_layer_set_text(text_date_layer, date_text);
  }

  // Get weather update every 5 minutes ************************
  if(tick_time->tm_min % 5 == 0) {

    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

    // Send the message!
    app_message_outbox_send();
  }


  //Always set time  *****************************************************
  text_layer_set_text(text_time_layer, time_text);
}

//Receive Temperature * * *
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {

  // Read Temperature
  Tuple *t = dict_read_first(iterator);

  strcpy(degreesstr,(t->value->cstring));

  int tempint = 100;

   tempint = atoi(degreesstr);

  // Assemble full string and display
  snprintf(degreesstr, 5, "%dF", tempint);

  //strcpy(degreesstr, "100F");   // For layout test

  text_layer_set_text(text_degrees_layer, degreesstr);
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

void handle_deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  app_focus_service_unsubscribe();

  text_layer_destroy(text_time_layer);
  text_layer_destroy(text_date_layer);
  text_layer_destroy(text_wa1oui_layer);
  text_layer_destroy(text_dayname_layer);
  text_layer_destroy(text_degrees_layer);
  text_layer_destroy(text_battery_layer);

  layer_destroy(LineLayer);
  layer_destroy(BTLayer1);

  fonts_unload_custom_font(fontHelvNewLight20);
  fonts_unload_custom_font(fontRobotoCondensed21);
  fonts_unload_custom_font(fontRobotoBoldSubset37);
  fonts_unload_custom_font(fontRobotoBoldSubset49);

  window_destroy(window);
}

//********************************** Handle Init **************************
void handle_init(void) {
  //Ideas Brass BG, Black Text
  //      Black BG, Yellow Text (ARRL?)
  //      DarkGreen BG, PastelYellow Text * * *
  //      ArmyGreen BG, PastelYellow Text

  GColor BGCOLOR   = COLOR_FALLBACK(GColorDukeBlue, GColorBlack);
  BGColorHold = BGCOLOR;

  GColor TEXTCOLOR = COLOR_FALLBACK(GColorWhite, GColorWhite);
  TextColorHold = TEXTCOLOR;

  window = window_create();
  window_set_background_color(window, BGCOLOR);
  window_stack_push(window, true /* Animated */);

  fontHelvNewLight20 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HELV_NEW_LIGHT_20));
  fontRobotoCondensed21  = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21));
  fontRobotoBoldSubset37 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_37));
  fontRobotoBoldSubset49 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_49));

  Layer *window_layer = window_get_root_layer(window);

  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  //degrees
  text_degrees_layer = text_layer_create(GRect(25, 1, 55, 200));
  text_layer_set_text_alignment(text_degrees_layer, GTextAlignmentRight);
  text_layer_set_text(text_degrees_layer, degreesstr);
  text_layer_set_font(text_degrees_layer, fontRobotoCondensed21);
  text_layer_set_background_color(text_degrees_layer, BGCOLOR);
  text_layer_set_text_color(text_degrees_layer, TEXTCOLOR);
  layer_add_child(window_layer, text_layer_get_layer(text_degrees_layer));


  // WA1OUI
  text_wa1oui_layer = text_layer_create(GRect(1,25, 144, 148));
  text_layer_set_text_alignment(text_wa1oui_layer, GTextAlignmentCenter);
  text_layer_set_text(text_wa1oui_layer, "WA1OUI");
  text_layer_set_font(text_wa1oui_layer, fontRobotoBoldSubset37);
  text_layer_set_text_color(text_wa1oui_layer, TEXTCOLOR);
  text_layer_set_background_color(text_wa1oui_layer, BGCOLOR);
  layer_add_child(window_layer, text_layer_get_layer(text_wa1oui_layer));


  // Dayname
  text_dayname_layer = text_layer_create(GRect(8, 65, 144-8, 168-65));
  text_layer_set_text_alignment(text_dayname_layer, GTextAlignmentCenter);
  text_layer_set_font(text_dayname_layer, fontRobotoCondensed21);
  text_layer_set_text_color(text_dayname_layer, TEXTCOLOR);
  text_layer_set_background_color(text_dayname_layer, BGCOLOR);
  layer_add_child(window_layer, text_layer_get_layer(text_dayname_layer));


  // Date
  text_date_layer = text_layer_create(GRect(8, 88, 144-8, 168-88));
  text_layer_set_text_alignment(text_date_layer, GTextAlignmentCenter);
  text_layer_set_font(text_date_layer, fontRobotoCondensed21);
  text_layer_set_text_color(text_date_layer, TEXTCOLOR);
  text_layer_set_background_color(text_date_layer, BGCOLOR);
  layer_add_child(window_layer, text_layer_get_layer(text_date_layer));


  // Time of Day
  text_time_layer = text_layer_create(GRect(7, 116, 144-7, 168-116));
  text_layer_set_text_alignment(text_time_layer, GTextAlignmentCenter);
  text_layer_set_font(text_time_layer, fontRobotoBoldSubset49);
  text_layer_set_text_color(text_time_layer, TEXTCOLOR);
  text_layer_set_background_color(text_time_layer, BGCOLOR);
  layer_add_child(window_layer, text_layer_get_layer(text_time_layer));

  // Red Line
  GRect red_line_frame = GRect(10, 28, 124, 2);
  RedLineLayer = layer_create(red_line_frame);
  layer_set_update_proc(RedLineLayer, red_line_layer_update_callback);
  layer_add_child(window_layer, RedLineLayer);

  // Battery Line
  GRect line_frame = GRect(22, 118, 104, 6);
  LineLayer = layer_create(line_frame);
  layer_set_update_proc(LineLayer, line_layer_update_callback);
  layer_add_child(window_layer, LineLayer);

  tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);

  //Bluetooth Logo Setup area
  GRect BTArea = GRect(1, 5, 20, 20);
  BTLayer1 = layer_create(BTArea);

  layer_add_child(window_layer, BTLayer1);

  layer_set_update_proc(BTLayer1, BTLine_update_callback);

  bluetooth_connection_service_subscribe(&handle_bluetooth);

  //Battery Text Layer
  text_battery_layer = text_layer_create(GRect(85, 2, 55, 26));
  text_layer_set_text_color(text_battery_layer, TEXTCOLOR);
  text_layer_set_background_color(text_battery_layer, BGCOLOR);
  text_layer_set_font(text_battery_layer, fontHelvNewLight20);
  text_layer_set_text_alignment(text_battery_layer, GTextAlignmentRight);

  battery_state_service_subscribe(&handle_battery);

  layer_add_child(window_layer, text_layer_get_layer(text_battery_layer));
  
  handle_bluetooth(bluetooth_connection_service_peek());
  handle_battery(battery_state_service_peek());

  //app focus service subscribe
  app_focus_service_subscribe(&handle_appfocus);
}

int main(void) {
   handle_init();

   app_event_loop();

   handle_deinit();
}