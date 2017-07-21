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
RobotReferenceType initialise(int robot_name_ref);
void deinitialise(RobotReferenceType instance, const int reason);
int createStringReference();
void stringAddChar(int ref, char c);

#import

/* Initialisation ----------------------------------------------------------- */
int OnInit() {
	/* Send settings to the dll */
	int robot_name_ref = stringToReference(robot_name);
	robot_instance = initialise(robot_name_ref);
	return( INIT_SUCCEEDED );
}

void OnDeinit(const int reason) {
	deinitialise(robot_instance, reason);
}

/* New Data to Analyse ------------------------------------------------------ */
void OnTick() {
}

/**
 * Puts the string in the DLL and returns a reference to the string.
 */
int stringToReference(string s) {
	int ref = createStringReference();
	for (int i=0; i<StringBufferLen(s); i++) {
		stringAddChar(ref, s[i]);
	}
	return ref;
}