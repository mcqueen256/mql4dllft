context = {}

def init(ctx={}):
	context = ctx

def require_buffers(func, *args, **kwargs):
  """Decorator for ensuring that the MQL buffer is in the context."""
  global context
  if 'buffers' not in context.keys():
    context['buffers'] = decode_buffers()
  return func(context, *args, **kwargs)

def require_functions(func, *args, **kwargs):
  """Decorator for ensuring that the MQL functions is in the context."""
  global context
  if 'functions' not in context.keys():
    context['functions'] = decode_functions()
  return func(context, *args, **kwargs)


def generate_mlq_expert():
  """Generates the full MQL4 code for the expert"""
  pass
def generate_mlq_indicator(): pass
def generate_dll_expert(): pass
def generate_dll_indicator(): pass
def generate_mql_buffers(): pass
def generate_dll_buffers(): pass
def generate_mql_ft(): pass
def generate_dll_ft(): pass
def generate_(): pass