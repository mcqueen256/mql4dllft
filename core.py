from decoder import decode_functions, decode_buffers

context = {}

def init():
  global context

def require_buffers(func, *args, **kwargs):
  """Decorator for ensuring that the MQL buffer is in the context."""
  def inner(*args, **kwargs):
    global context
    if 'buffers' not in context.keys():
      context['buffers'] = decode_buffers()
    return func(*args, **kwargs)
  return inner

def require_functions(func, *args, **kwargs):
  """Decorator for ensuring that the MQL functions is in the context."""
  def inner(*args, **kwargs):
    global context
    if 'functions' not in context.keys():
      context['functions'] = decode_functions()
    return func(*args, **kwargs)
  return inner

def require_context(func, *args, **kwargs):
  """Decorator that gives a function the context as the first parameter."""
  def f(*args, **kwargs):
    global context
    return func(context, *args, **kwargs)
  return f

@require_functions
@require_buffers
@require_context
def generate_mlq_expert(ctx):
  """Generates the full MQL4 code for the expert"""
  pass
def generate_mlq_indicator(): pass
def generate_dll_expert(): pass
def generate_dll_indicator(): pass
def generate_mql_buffers(): pass
def generate_dll_buffers(): pass
def generate_mql_ft(): pass
def generate_dll_ft(): pass

@require_functions
@require_buffers
@require_context
def generate_all(ctx):
  
  pass
