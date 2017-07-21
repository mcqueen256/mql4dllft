/* MT4 Properties ----------------------------------------------------------- */
{% for line in common["properties"] -%}
#property {{ line }}
{% endfor %}

/* Inputs ------------------------------------------------------------------- */
{% for line in expert["inputs"] -%}
input {{ line }};
{% endfor %}
input string robot_name = "{{ name }}";

/* Globals ------------------------------------------------------------------ */
#define RobotReferenceType long
RobotReferenceType robot_instance;

/* Libraries ---------------------------------------------------------------- */
/* DLL Functions ----------------------------------------------------------- */
#import "{{ name }}.dll"
RobotReferenceType initialise(uchar &robot_name[]);
void deinitialise(RobotReferenceType instance, const int reason);

#import

/* Initialisation ----------------------------------------------------------- */
int OnInit() {
	/* Send settings to the dll */
	uchar robot_name_uchar_array[];
	StringToCharArray( robot_name, robot_name_uchar_array );
	robot_instance = initialise( robot_name_uchar_array );
	return( INIT_SUCCEEDED );
}

void OnDeinit(const int reason) {
	deinitialise(robot_instance, reason);
}

/* New Data to Analyse ------------------------------------------------------ */
void OnTick() {
}