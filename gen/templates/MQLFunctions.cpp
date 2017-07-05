namespace mql4 {
	enum ENUM_TIMEFRAMES {
		PERIOD_CURRENT,
		PERIOD_M1,
		PERIOD_M5,
		PERIOD_M15,
		PERIOD_M30,
		PERIOD_H1,
		PERIOD_H4,
		PERIOD_D1,
		PERIOD_W1,
		PERIOD_MN1
	};

	enum ENUM_ALIGN_MODE {
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT
	};

	{% for namespace in func_dict_by_namespace.keys() %}
		namespace {{ namespace }} {
			{% for function in func_dict_by_namespace[namespace] %}
			{{ function.getReturnType() }} {{ function.getName() }}({{ function.getParameterLine() }}) {

			}
			{% endfor %}
		}

	{% endfor %}
	
}
