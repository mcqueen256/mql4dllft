/* MT4 Properties ---------------------------------------------------------- */
{% for line in ctx.property_lines %}{{ line }}
{% endfor %}

/* Indicator Properties ---------------------------------------------------- */
#property indicator_buffers {{ ctx.property_buffers | length }}
{% for buffer in ctx.property_buffers %}{% for line in buffer %}{{ line }}
{% endfor %}{% endfor %}


/* Inputs ------------------------------------------------------------------ */
{% for line in ctx.input_lines %}input {{ line }};
{% endfor %}


/* Globals ----------------------------------------------------------------- */
int robot_instance;

/* DLL Functions ----------------------------------------------------------- */
#import "{{ name }}.dll"
int initialise(int instance);
void deinitialise(int instance)
#import

/* Buffer Definitions ------------------------------------------------------ */
#define BUFFER_COUNT {{ ctx.property_buffers | length }}
#define TestIndicator 0

double BufferTest[];

/* Initialisation ---------------------------------------------------------- */
int OnInit() {
	/* Send settings to the dll */
	robot_instance = initialise(number);

	/* Setup display buffers */
	SetIndexBuffer(TestIndicator, BufferTest);
	return(INIT_SUCCEEDED);
}

/* Deinitialisation -------------------------------------------------------- */
void OnDeinit(const int reason) {
	/* Send settings to the dll */
	deinitialise(robot_instance);
}

/* New Data to Analyse ----------------------------------------------------- */
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
