/* HPP file*/
#ifndef MQLINTERFACE_HPP
#define MQLINTERFACE_HPP

namespace mql {
    class MQLInterface {
    private:
		{% for t, n in ft.get_result_dict().items() -%}
            {{ t }} _result{{ n }};
		{% endfor %}
		int action;
		std::function<void(void)> toCalling;
    public:
		{% for t, n in ft.get_param_dict_without_refs().items() -%}
            std::queue<{{ t }}> param{{ n }}Queue;
		{% endfor %}
		{% for t, n in ft.get_reference_dict_without_refs().items() -%}
            std::queue<{{ t }}> ref{{ n }}Queue;
		{% endfor %}
		{% for t, n in ft.get_result_dict().items() -%}
            {{ t }} get{{ n }}Result();
		{% endfor %}
		{% for t, n in ft.get_result_dict().items() -%}
            void set{{ n }}Result({{ t }} result);
		{% endfor %}
		void setAction(int a);
		int getAction();
		void switchToCalling();
		void setSwitchToCalling(std::function<void(void)> func);
    };

    class MQL4APIInstance;

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
	};
	{% endfor %}

	class MQL4APIInstance : std::mutex {
	private:
		{% for namespace in namespaces.keys() -%}
		    {{ namespace | title }}* _{{ namespace }}_ref;
		{% endfor %}
		MQLInterface interface;
	public:
		MQL4APIInstance(RobotReferenceType instance);
		~MQL4APIInstance();
		{% for namespace in namespaces.keys() -%}
		    {{ namespace | title }}& {{ namespace }}();
		{% endfor %}
		MQLInterface& getInterface();
		RobotReferenceType getInstance();
	};
}

#endif