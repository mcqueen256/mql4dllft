def refactor_param_type_name(n):
    split_by_whitespace = n.split()
    type_segments = []
    for need_to_be_split_by_underscore in [word.capitalize() for word in split_by_whitespace]:
        split_by_underscore = need_to_be_split_by_underscore.split('_')
        new_type = ''.join([word.capitalize() for word in split_by_underscore])
        type_segments.append(new_type)
    return ''.join(type_segments)

class FunctionTranslator(object):
    """docstring for FunctionTranslator"""

    def __init__(self, context):
        self._return_types = []
        for f in context['functions']:
            if f.getReturnType() not in self._return_types:
                self._return_types.append(f.getReturnType())

        self._parameter_types = []
        for f in context['functions']:
            for t, n, d, c in f.getParameters():
                if t not in self._parameter_types:
                    self._parameter_types.append(t)

        self._paramStackTypePushMathods = {}
        for t in self._parameter_types:
            t_name = refactor_param_type_name(t)
            name = "paramStack{type}Push".format(type=t_name).replace('&', '')
            self._paramStackTypePushMathods[t] = name

        self._result_type_getters = {}
        for t in self._return_types:
            t_name = refactor_param_type_name(t)
            self._result_type_getters[t] = "get{}Result".format(t_name)

        self._references = [x for x in self._parameter_types if '&' in x]
        self._references += [x.replace('&', '') for x in self._references]
        self._reference_setters = {}
        for t in self._references:
            t_name = refactor_param_type_name(t)
            self._reference_setters[t] = 'refQueue{}'.format(t_name.replace('&', ''))

    def getFunctions(self):
        functions = []
        return functions

        # build parameter getters
        # for parameter_type in self._parameter_types:
        #  pass

    def get_param_stack_type_push_line(self, t: str, n: str) -> str:
        return self._paramStackTypePushMathods[t] + "({name});".format(name=n)

    def get_result_type(self, t: str) -> str:
        return self._result_type_getters[t]

    def get_reference_setter(self, t):
        return self._reference_setters[t]

