/* HPP file*/
#ifndef MQLFUNCTIONS_HPP
#define MQLFUNCTIONS_HPP

namespace MQLInterface {
	{% for namespace in func_dict_by_namespace.keys() %}
	class {{ namespace | title }} {
	private:
		InstanceReferenceType instance;
	public:
		{{ namespace | title }}(InstanceReferenceType instance);
		~{{ namespace | title }}();
		{% for function in func_dict_by_namespace[namespace] %}{{ function.getReturnTypeDLL() }} {{ function.getName() }}({{ function.getParameterLine() }});
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
{{ function.getReturnTypeDLL() }} MQLInterface::{{ namespace | title }}::{{ function.getName() }}({{ function.getParameterLine() }}) {

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
