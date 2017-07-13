/* CPP file*/

{% for namespace in namespaces.keys() -%}
mql::{{ namespace | title }}::{{ namespace | title }}(MQL4APIInstance* api, RobotReferenceType instance) {
    this->api = api;
    this->instance = instance;
}

mql::{{ namespace | title }}::~{{ namespace | title }}() {

}

{% for function in namespaces[namespace] -%}
{{ function.getReturnTypeDLL() }} mql::{{ namespace | title }}::{{ function.getName() }}({{ function.getParameterLine() }}) {
    std::unique_lock<std::mutex> lck(*(this->api)); // only have ony mlq function run at a time
    //push parameters onto the stack
    {% for t, n, d, c in function.getParameters() -%}
        this->api->paramStack{{ ft.get_type_to_param_stack_name(t) }}Push({{ n }});
    {% endfor -%}
    this->api->setRequestedAction({{ function.getIndex() }});
    this->api->switchToCalling();
    {% for t, n, d, c in function.getReferenceParameters() -%}
        this->api->refQueue{{ ft.get_type_to_reference_name(t) }}({{ n }});
    {% endfor %}
    {%- if function.getReturnType() != 'void' -%}
        return this->api->get{{ ft.get_type_to_result_name(function.getReturnType()) }}Result();
    {%- else -%}
        return;
    {%- endif %}
}

{% endfor %}
{% endfor %}


mql::MQL4APIInstance::MQL4APIInstance(RobotReferenceType instance) {
    {% for namespace in namespaces.keys() -%}
        _{{ namespace }}_ref = new mql::{{ namespace | title }}(this, instance);
    {% endfor %}
}

mql::MQL4APIInstance::~MQL4APIInstance() {
    this->lock(); // wait then lock the api up so that nothing is in the middle of using it
    {% for namespace in namespaces.keys() -%}
        delete _{{ namespace }}_ref;
    {% endfor %}
}

{% for namespace in namespaces.keys() %}
	mql::{{ namespace | title }}& mql::MQL4Functions::{{ namespace }}() {
		return *_{{ namespace }}_ref;
	}
{% endfor %}


{% for t, name in ft.get_param_dict().items() %}
void mql::MQL4APIInstance::paramStack{{ name }}Push ({{ t }} x){
    this->paramStack{{ name }}.push(p);
}
{% endfor %}