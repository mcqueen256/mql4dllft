/* MT4 Properties ----------------------------------------------------------- */
{% for line in ctx.property_lines %}{{ line }}
{% endfor %}

/* Inputs ------------------------------------------------------------------- */
{% for line in ctx.input_lines %}input {{ line }};
{% endfor %}

/* Globals ------------------------------------------------------------------ */
int robot_instance;
unsigned int bar_checksum = 0;

/* Libraries ---------------------------------------------------------------- */
/* DLL Functions ----------------------------------------------------------- */
#import "{{ ctx.name }}.dll"
int initialise(int instance);
void deinitialise(int instance, const int reason);
void enableTrading(int instance);
void disableTrading(int instance);
int bar(int instance, string time, double open, double high, double low, double close, double volume);
int quote(int instance, string time, double open, double high, double low, double close, double volume);

{% for function in ctx.ft.getFunctions() %}
{% endfor %}
#import

/* Initialisation ----------------------------------------------------------- */
int OnInit() {
  /* Send settings to the dll */
  robot_instance = initialise(number);

  /* Fill instance with current bar buffer history */
  for (int i = ArraySize(Close) - 1; i > 0; i--) {
    // above loop does not fill the (first) uncompleted bar.
    send_bar(robot_instance,
      TimeToStr(Time[i]),
      Open[i],
      High[i],
      Low[i],
      Close[i],
      Volume[i]);
  }
  enableTrading(robot_instance);

  return(INIT_SUCCEEDED);
}

void OnDeinit(const int reason) {
  deinitialise(robot_instance, reason);
}

/* New Data to Analyse ------------------------------------------------------ */
void OnTick() {
  /* Track when a new bar is formed */
  static long tick_volume_tracker = 0;

  if (tick_volume_tracker < Volume[0]) {
    /* Volume is still increasing */
    tick_volume_tracker = Volume[0];
  } else {
    /* New completed bar */
    send_bar(robot_instance,
      TimeToStr(Time[1]),
      Open[1],
      High[1],
      Low[1],
      Close[1],
      Volume[1]);
    tick_volume_tracker = 0;
  }
  /* Send the latest quotation */
  send_quote(robot_instance,
    TimeToStr(Time[0]),
    Open[0],
    High[0],
    Low[0],
    Close[0],
    Volume[0]);
}

void send_bar(int instance, string time, double open, double high, double low, double close, double volume) {

}
void send_quote(int instance, string time, double open, double high, double low, double close, double volume) {

}

/*void exe() {
  int action = bar(...);
  while (action != 0) {
    switch(action) {
      case 1: 
    }
  }
}*/