from typing import Tuple, List

ParameterType = Tuple[str, str, str, str]

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

  def __repr__(self) -> str:
    """Print a neat representation of the MQLFunction"""
    param_repr = '' if self._parameters is None or len(self._parameters) == 0 else '...'
    namespace_repr = '' if self._namespace is None else (self._namespace + '::')
    res = '{} {}{}({});'.format(self._return_type, namespace_repr, self._name, param_repr)
    return '<MQLFunction \'' + res + '\'>'

  def getName(self) -> str:
    """Returns the name of the function."""
    return self._name

  def getReturnType(self) -> str:
    """Returns the return type of the function."""
    return self._return_type

  def GetParameters(self) -> List[ParameterType]:
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