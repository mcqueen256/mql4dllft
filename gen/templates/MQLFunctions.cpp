namespace mql4 {
	{% for namespace in func_dict_by_namespace.keys() %}
		namespace {{ namespace }} {
			{% for function in func_dict_by_namespace[namespace] %}
			{{ function.getReturnType() }} {{ function.getName() }}({{ function.getParameterLine() }}) {

			}
			{% endfor %}
		}
	{% endfor %}
	
}
