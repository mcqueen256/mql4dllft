/* HPP file*/
#ifndef MQLFUNCTIONS_HPP
#define MQLFUNCTIONS_HPP

namespace MQLTypes {
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

	struct MqlDateTime {
		int year;           // Year
		int mon;            // Month
		int day;            // Day
		int hour;           // Hour
		int min;            // Minutes
		int sec;            // Seconds
		int day_of_week;    // Day of week (0-Sunday, 1-Monday, ... ,6-Saturday)
		int day_of_year;    // Day number of the year (January 1st is assigned the number value of zero)
	};

}
namespace MQLInterface {
	{% for namespace in func_dict_by_namespace.keys() %}
	class {{ namespace | title }} {
	private:
		InstanceReferenceType instance;
	public:
		{{ namespace | title }}(InstanceReferenceType instance);
		~{{ namespace | title }}();
		{% for function in func_dict_by_namespace[namespace] %}{{ function.getReturnType() }} {{ function.getName() }}({{ function.getParameterLine() }});
		{% endfor %}

	};
	{% endfor %}

	class MQL4Functions {
	private:
		InstanceReferenceType instance;
		{% for namespace in func_dict_by_namespace.keys() %}{{ namespace | title }}* _{{ namespace }}_ref;
		{% endfor %}
	public:
		MQL4Functions(InstanceReferenceType instance);
		~MQL4Functions();
		{% for namespace in func_dict_by_namespace.keys() %}{{ namespace | title }}& {{ namespace }}();
		{% endfor %}
	};
}

#endif
/* CPP file*/

{% for namespace in func_dict_by_namespace.keys() %}
MQLInterface::{{ namespace | title }}::{{ namespace | title }}(InstanceReferenceType instance) {

}

MQLInterface::{{ namespace | title }}::~{{ namespace | title }}() {

}
{% for function in func_dict_by_namespace[namespace] %}
{{ function.getReturnType() }} MQLInterface::{{ namespace | title }}::{{ function.getName() }}({{ function.getParameterLine() }}) {

}
{% endfor %}
{% endfor %}


MQLInterface::MQL4Functions::MQL4Functions(InstanceReferenceType instance) {

}

MQLInterface::MQL4Functions::~MQL4Functions() {

}

{% for namespace in func_dict_by_namespace.keys() %}
MQLInterface::{{ namespace | title }}& MQLInterface::MQL4Functions::{{ namespace }}() {
	return *_{{ namespace }}_ref;
}
{% endfor %}
