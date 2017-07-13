/* CPP file*/

// mql::MQLInterface

{% for t, n in ft.get_result_dict().items() %}
{{ t }} mql::MQLInterface::get{{ n }}Result() {
    return _result{{ n }};
}
{% endfor %}

{% for t, n in ft.get_result_dict().items() %}
void mql::MQLInterface::set{{ n }}Result({{ t }} result) {
    _result{{ n }} = result;
}
{% endfor %}

void mql::MQLInterface::setAction(int a) {
    action = a;
}

int mql::MQLInterface::getAction() {
    return action;
}

void mql::MQLInterface::switchToCalling() {
    toCalling();
}

void mql::MQLInterface::setSwitchToCalling(std::function<void(void)> func) {
    toCalling = func;
}

{% for namespace in namespaces.keys() -%}
// mql::{{ namespace | title }}
mql::{{ namespace | title }}::{{ namespace | title }}(MQL4APIInstance* api, RobotReferenceType instance) {
    this->api = api;
    this->instance = instance;
}

mql::{{ namespace | title }}::~{{ namespace | title }}() {
    // nothing to do
}

{% for function in namespaces[namespace] -%}
{{ function.getReturnTypeDLL() }} mql::{{ namespace | title }}::{{ function.getName() }}({{ function.getParameterLine() }}) {
    std::unique_lock<std::mutex> lck(*(this->api)); // only have ony mlq function run at a time
    //push parameters onto the stack
    {% for t, n, d, c in function.getParameters() -%}
        this->api->getInterface().param{{ ft.get_type_to_param_stack_name(t) }}Queue.push({{ n }});
    {% endfor -%}
    this->api->getInterface().setAction({{ function.getIndex() }});
    this->api->getInterface().switchToCalling();
    {% for t, n, d, c in function.getReferenceParameters() -%}
        this->api->getInterface().ref{{ ft.get_type_to_reference_name(t) }}Queue.push({{ n }});
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

mql::MQLInterface& mql::MQL4Functions::getInterface() {
    return interface;
}

RobotReferenceType mql::MQL4Functions::getInstance() {
    return instance;
}


