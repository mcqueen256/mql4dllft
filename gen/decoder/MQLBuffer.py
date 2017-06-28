
class MQLBuffer:
	_global_index = 0
	def __init__(self):
		self._index = MQLBuffer = MQLBuffer._global_index
		MQLBuffer._global_index += 1
		self._name = None
		self._description = None
		self._style = None
		self._width = None
		self._color = None
		self._label = None
		self._arrow = None
		self._level_style = None
		self._level_witdh = None
		self._level_color = None