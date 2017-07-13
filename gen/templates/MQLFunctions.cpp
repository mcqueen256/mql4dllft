/* CPP file*/

{% for namespace in namespaces.keys() -%}
MQLInterface::{{ namespace | title }}::{{ namespace | title }}(MQL4APIInstance* api, RobotReferenceType instance) {
    this->api = api;
    this->instance = instance;
}

MQLInterface::{{ namespace | title }}::~{{ namespace | title }}() {

}

{% for function in namespaces[namespace] -%}
{{ function.getReturnTypeDLL() }} MQLInterface::{{ namespace | title }}::{{ function.getName() }}({{ function.getParameterLine() }}) {
    std::unique_lock<std::mutex> lck(*(this->api)); // only have ony mlq function run at a time
    //push parameters onto the stack
    {% for t, n, d, c in function.getParameters() -%}
        this->api->{{ ft.get_param_stack_type_push_line(t, n) }}
    {% endfor -%}
    this->api->setRequestedAction({{ function.getIndex() }});
    this->api->switchToCalling();
    {% for t, n, d, c in function.getReferenceParameters() -%}
        this->api->{{ ft.get_reference_setter(t) }}({{ n }});
    {% endfor %}
    {%- if function.getReturnType() != 'void' -%}
        return this->api->{{ ft.get_result_type(function.getReturnType()) }}();
    {%- else -%}
        return;
    {%- endif %}
}

{% endfor %}
{% endfor %}


MQLInterface::MQL4APIInstance::MQL4APIInstance(RobotReferenceType instance) {
    {% for namespace in namespaces.keys() -%}
        _{{ namespace }}_ref = new MQLInterface::{{ namespace | title }}(this, instance);
    {% endfor %}
}

MQLInterface::MQL4APIInstance::~MQL4APIInstance() {
    this->lock(); // wait then lock the api up so that nothing is in the middle of using it
    {% for namespace in namespaces.keys() -%}
        delete _{{ namespace }}_ref;
    {% endfor %}
}

{% for namespace in namespaces.keys() %}
	MQLInterface::{{ namespace | title }}& MQLInterface::MQL4Functions::{{ namespace }}() {
		return *_{{ namespace }}_ref;
	}
{% endfor %}
