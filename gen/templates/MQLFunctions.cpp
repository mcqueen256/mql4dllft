namespace mql4 {
	{% for function in functions %}{% if function is not none %}
	{{ function._return_type }} {{ function.getName() }}();
	{{function}}
	{% endif %}{% endfor %}
}
