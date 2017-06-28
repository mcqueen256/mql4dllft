/* MT4 Properties ----------------------------------------------------------- */
#property copyright "Copyright 2017, Nicholas Buckeridge."
#property link      "https://mcqueen256.com/"
#property version   "1.00"
#property strict

/* Indicator Properties ----------------------------------------------------- */
//#property indicator_separate_window
#property indicator_chart_window

#property indicator_buffers 1

#property indicator_color1  Red
#property indicator_width1 1
#property indicator_style1 STYLE_SOLID
#property indicator_label1 "High"

{% for buffer in buffers %}{% for line in buffer %}
{{ line }}{% endfor %}{% endfor %}

/* Inputs ------------------------------------------------------------------- */
input int number;

/* Globals ------------------------------------------------------------------ */
int robot_instance;

/* DLL Functions ------------------------------------------------------------ */
#import "NBRobot.dll"
int initialise(int instance);
void deinitialise(int instance)
#import

/* Buffer Definitions ------------------------------------------------------- */
#define BUFFER_COUNT 1
#define TestIndicator 0

double BufferTest[];

/* Initialisation ----------------------------------------------------------- */
int OnInit() {
	/* Send settings to the dll */
	robot_instance = initialise(number);

	/* Setup display buffers */
	SetIndexBuffer(TestIndicator, BufferTest);
	return(INIT_SUCCEEDED);
}

/* Deinitialisation ----------------------------------------------------------- */
void OnDeinit(const int reason) {
	/* Send settings to the dll */
	deinitialise(robot_instance);
}

/* New Data to Analyse ------------------------------------------------------ */
int OnCalculate(const int rates_total,  const int prev_calculated,
                const datetime &time[], const double &open[],
                const double &high[],   const double &low[],
                const double &close[],  const long &tick_volume[],
                const long &volume[],   const int &spread[]) {
  /* number or bars that have been calculated thus far */
  int bars_calculated = prev_calculated;

  
  /* Return the value for the next 'prev_calculated' */
  return bars_calculated;
}
