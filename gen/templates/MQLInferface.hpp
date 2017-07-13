/* HPP file*/
#ifndef MQLINTERFACE_HPP
#define MQLINTERFACE_HPP

namespace MQLInterface {
	{%- for namespace in namespaces.keys() %}
	class {{ namespace | title }} {
	private:
		RobotReferenceType instance;
		MQL4APIInstance* api;
	public:
		{{ namespace | title }}(MQL4APIInstance* api, RobotReferenceType instance);
		~{{ namespace | title }}();
		{% for function in namespaces[namespace] -%}
		    {{ function.getReturnTypeDLL() }} {{ function.getName() }}({{ function.getParameterLine() }});
		{% endfor %}
		void destroyAPIInstance();
	};
	{% endfor %}

	class MQL4APIInstance : std::mutex {
	private:
		{% for namespace in namespaces.keys() -%}
		    {{ namespace | title }}* _{{ namespace }}_ref;
		{%- endfor %}
	public:
		MQL4APIInstance(RobotReferenceType instance);
		~MQL4APIInstance();
		{% for namespace in namespaces.keys() -%}
		    {{ namespace | title }}& {{ namespace }}();
		{%- endfor %}
		RobotReferenceType getInstance();
		{% for t, n in ft._paramStackTypePushMathods.items() -%}
            void {{ ft.get_param_stack_type_push_line(t, t) }}
		{% endfor %}
		{% for t, n in ft._result_type_getters.items() -%}
            void {{ ft.get_result_type(t) }}({{ t }});
		{% endfor %}
		{% for t, n in ft._reference_setters.items() -%}
            void {{ ft.get_reference_setter(t) }}({{ t }});
		{% endfor %}
	};
}

#endif