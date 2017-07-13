from typing import Tuple, List

ParameterType = Tuple[str, str, str, str]

class MQLFunction:
  """Stores the representation of the """
  _index_store = 1
  class MQLParameter:
    def __init__(self, param):
      self._raw_param = param

  def __init__(self):
    """Constructor for MQLFunction: creates and empty function."""
    self._return_type = None
    self._name = None
    self._parameters = None  # [(type, name, default, comment), ...]
    self._comment = ''
    self._return_comment = ''
    self._note = ''
    self._namespace = None
    self._index = MQLFunction._index_store
    MQLFunction._index_store += 1

  def __repr__(self) -> str:
    """Print a neat representation of the MQLFunction"""
    param_repr = '' if self._parameters is None or len(self._parameters) == 0 else '...'
    namespace_repr = '' if self._namespace is None else (self._namespace + '::')
    res = '{} {}{}({});'.format(self._return_type, namespace_repr, self._name, param_repr)
    return '<MQLFunction \'' + res + '\'>'

  def getIndex(self) -> int:
    """Returns the unique index of this function."""
    return self._index

  def getName(self) -> str:
    """Returns the name of the function."""
    return self._name

  def getReturnType(self) -> str:
    """Returns the return type of the function."""
    return self._return_type

  def getReturnTypeDLL(self) -> str:
    """Returns the return type of the function for the DLL."""
    return self._return_type.replace('char*', 'std::string')

  def getParameters(self) -> List[ParameterType]:
    """Returns list of tuples representing the parameters. The tuple data is
    ordered as (type, name, default, comment) where each element is a str.
    """
    return self._parameters

  def getComment(self) -> str:
    """Returns the comment of the function."""
    return self._comment

  def getReturnComment(self) -> str:
    """Returns the return comment of the function."""
    return self._return_comment

  def getNote(self) -> str:
    """Returns the note of the function."""
    return self._note

  def getNamespace(self) -> str:
    """Returns the namespace/category of the function."""
    return self._namespace

  def setName(self, newName):
    """Sets the name of the function."""
    self._name = newName

  def setReturnType(self, newReturnType):
    """Sets the return type of the function."""
    if newReturnType is not None:
      self._return_type = newReturnType # .replace('string', 'char*')
    else:
      self._return_type = None

  def setParameters(self, newParameters):
    """Sets the list of tuples representing the parameters. The tuple data is
    ordered as (type, name, default, comment) where each element is a str.
    """
    self._parameters = []
    for t, n, d, c in newParameters:
      if t is None: continue
      # t = t.replace('string', 'char*')
      param = t, n, d, c
      self._parameters.append(param)



  def setComment(self, newComment):
    """Sets the comment of the function."""
    self._comment = newComment

  def setReturnComment(self, newReturnComment):
    """Sets the return comment of the function."""
    self._return_comment = newReturnComment

  def setNote(self, newNote):
    """Sets the note of the function."""
    self._note = newNote

  def setNamespace(self, newNamespace):
    """Sets the namespace/category of the function."""
    self._namespace = newNamespace

  def getParameterLine(self) -> str:
    """Returns C++ code for the parameters."""
    def form_param(param):
      t, n, d, c = param
      #t = t.replace('char*', 'std::string')
      if d is None:
        return '{} {}'.format(t, n)
      else:
        return '{} {} = {}'.format(t, n, d)
    params = [ form_param(param) for param in self._parameters ]
    return ', '.join(params)

  def copy(self):
    """Create a unique copy based of this parameter."""
    function = MQLFunction()
    function._return_type = self._return_type
    function._name = self._name
    function._parameters = self._parameters[:]
    function._comment = self._comment
    function._return_comment = self._return_comment
    function._note = self._note
    function._namespace = self._namespace
    return function
