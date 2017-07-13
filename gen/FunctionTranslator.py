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

        self._param_stack_type_to_names = {}
        for t in self._parameter_types:
            self._param_stack_type_to_names[t] = self.refactor(t.replace('&', ''))

        self._result_type_to_names = {}
        for t in self._return_types:
            self._result_type_to_names[t] = self.refactor(t)

        self._references = [x for x in self._parameter_types if '&' in x]
        self._references += [x.replace('&', '') for x in self._references]
        self._reference_type_to_name = {}
        for t in self._references:
            self._reference_type_to_name[t] = self.refactor(t)

    def get_param_stack_type_names(self):
        return self._param_stack_type_to_names.values()

    def get_result_type_names(self):
        return self._result_type_to_names.values()

    def get_reference_type_names(self):
        return self._reference_type_to_name.values()

    def get_type_to_param_stack_name(self, t: str) -> str:
        return self._param_stack_type_to_names[t]

    def get_type_to_result_name(self, t: str) -> str:
        return self._result_type_to_names[t]

    def get_type_to_reference_name(self, t: str) -> str:
        return self._reference_type_to_name[t]

    def get_param_dict(self):
        return self._param_stack_type_to_names

    def get_result_dict(self):
        return self._result_type_to_names

    def get_reference_dict(self):
        return self._reference_type_to_name

    def get_param_dict_without_refs(self):
        keys = self._param_stack_type_to_names
        new_keys = []
        for key in keys:
            if '&' not in key:
                new_keys.append(key)
        new_dict = {}
        for key in new_keys:
            new_dict[key] = self._param_stack_type_to_names[key]
        return new_dict

    def get_reference_dict_without_refs(self):
        keys = self._reference_type_to_name
        new_keys = []
        for key in keys:
            if '&' not in key:
                new_keys.append(key)
        new_dict = {}
        for key in new_keys:
            new_dict[key] = self._reference_type_to_name[key]
        return new_dict

    def refactor(self, n):
        split_by_whitespace = n.split()
        type_segments = []
        for need_to_be_split_by_underscore in [word.capitalize() for word in split_by_whitespace]:
            split_by_underscore = need_to_be_split_by_underscore.split('_')
            new_type = ''.join([word.capitalize() for word in split_by_underscore])
            type_segments.append(new_type)
        return ''.join(type_segments)


