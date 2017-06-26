class MQLFunction:
    """Stores the representation of the """
    def __init__(self):
        """Constructor for MQLFunction: creates and empty function."""
        self._return_type = None
        self._name = None
        self._parameters = None  # [(type, name, default, comment), ...]
        self._comment = ''
        self._return_comment = ''
        self._note = ''
        self._namespace = None
        self._filename = None

    def __repr__(self) -> str:
        """Print a neat representation of the MQLFunction"""
        param_repr = '' if self._parameters is None or len(self._parameters) == 0 else '...'
        namespace_repr = '' if self._namespace is None else (self._namespace + '::')
        res = '{} {}{}({});'.format(self._return_type, namespace_repr, self._name, param_repr)
        return '<MQLFunction \'' + res + '\'>'

    def getName(self) -> str:
        return self._name