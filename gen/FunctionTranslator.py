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

    def getFunctions(self):
        functions = []
        return functions

        # build parameter getters
        # for parameter_type in self._parameter_types:
        #  pass
