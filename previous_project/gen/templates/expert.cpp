/* MT4 Properties ----------------------------------------------------------- */
{% for line in expert.property_lines -%}
{{ line }}
{% endfor %}

/* Inputs ------------------------------------------------------------------- */
{% for line in expert.input_lines -%}
input {{ line }};
{%- endfor %}
input string robot_name = "{{ expert.name }}";

/* Globals ------------------------------------------------------------------ */
#define RobotReferenceType long
RobotReferenceType robot_instance;
unsigned int bar_checksum = 0;

/* Libraries ---------------------------------------------------------------- */
/* DLL Functions ----------------------------------------------------------- */
#import "{{ expert.name }}.dll"
RobotReferenceType initialise(string robot_name);
void deinitialise(RobotReferenceType instance, const int reason);
int bar(RobotReferenceType instance, string time, double open, double high, double low, double close, double volume);
int quote(RobotReferenceType instance, string time, double open, double high, double low, double close, double volume);

{% for t, name in ft.get_param_dict_without_refs().items() -%}
{{ t }} getParam{{ name }}(RobotReferenceType instance);
{% endfor %}
{% for t, name in ft.get_reference_dict_without_refs().items() -%}
void set{{ name }}Reference(RobotReferenceType instance, {{ t }} parameter);
{% endfor %}
{% for t, name in ft.get_result_dict().items() -%}
{{ t }} set{{ name }}Return(RobotReferenceType instance, {{ t }} result);
{% endfor %}
void setActionEnd(RobotReferenceType instance);

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

void process_action(int action) {
	while(true) {
		switch(action) {
		    case ACTION_END:
		        setActionEnd(robot_instance);
		        return;
            {% for namespace, function_list in namespaces.items() -%}
                {% for function in function_list -%}
                case {{ function.getIndex() }}:
                	{
                    {% for t, n, d, c in function.getParameters() -%}
                        {{ t.replace('&', '') }} p{{ loop.index }} = getParam{{ ft.get_type_to_param_name(t) }}(robot_instance);
                    {% endfor %}
                    {%- if function.getReturnType() == 'void' -%}
                        {{ function.getName() }}({{ function.getMQLSwitchInlineParametersline() }});
                    {%- else -%}
                        {{ function.getReturnType() }} result = {{ function.getName() }}({{ function.getMQLSwitchInlineParametersline() }});
                    {%- endif %}
                    {%- for t, n, d, c in function.getParameters() -%}
                    {%- if  '&' in t %}
                    set{{ ft.get_type_to_param_name(t) }}Reference(robot_instance, p{{ loop.index }});
                    {%- endif -%}
                    {%- endfor %}
                    action = set{{ ft.get_type_to_result_name(function.getReturnType()) }}Return(robot_instance, result);
                    return;
                	}
                {% endfor %}
            {% endfor %}
		}
	}
}
