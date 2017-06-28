from decoder import decode_functions, decode_buffers, decode_properties
from jinja2 import Template

context = {}

def init():
  global context

# Decorators ##################################################################

def require_buffers(func, *args, **kwargs):
  """Decorator for ensuring that the MQL buffer is in the context."""
  def inner(*args, **kwargs):
    global context
    if 'buffers' not in context.keys():
      context['buffers'] = decode_buffers()
    return func(*args, **kwargs)
  return inner

def require_properties(func, *args, **kwargs):
  """Decorator for ensuring that the MQL properties are in the context."""
  def inner(*args, **kwargs):
    global context
    if 'properties' not in context.keys():
      context['properties'] = decode_properties()
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

# Utility #####################################################################
class ExpertContext:
  """Class to be passed to the template"""
  def __init__(self, name):
    self.property_buffers = []
    self.property_lines = []
    self.name = name
    self.input_lines = []

@require_properties
@require_context
def get_property_c_lines(ctx, category):
  # Prepare properties lines
  property_lines = []
  for key, val in ctx['properties'][category].items():
    prop = '_'.join(key.split())
    if val is not None:
      prop += ' ' + str(val)
    line = '#property {}'.format(prop)
    property_lines.append(line)
  return property_lines

def get_common_property_c_lines():
  return get_property_c_lines('common')

def get_indicator_property_c_lines():
  return get_property_c_lines('indicator')

def get_expert_property_c_lines():
  return get_property_c_lines('expert')

@require_buffers
@require_context
def get_buffer_property_c_lines(ctx):
  property_buffers = []
  for i, buffer in enumerate(ctx['buffers']):
    lines = []
    for key in buffer.keys():
      val = buffer[key]
      # special conditions
      if val is None: continue
      if key.endswith('color'):
        pass # do nothing to val
      elif key.endswith('style'):
        val = 'STYLE_' + val.upper()
      elif type(val) is str:
        val = '"{}"'.format(val)
      #generate the line
      prop = '_'.join(key.split())
      line = "#property indicator_{}{} {}".format(prop, i+1, val)
      lines.append(line)
    property_buffers.append(lines)
  return property_buffers

@require_properties
@require_context
def get_input_c_lines(ctx):
  return ctx['properties']['inputs']

# Actionable code #############################################################

@require_functions
@require_buffers
@require_context
def generate_mlq_expert(ctx):
  """Generates the full MQL4 code for the expert"""
  pass

@require_buffers
@require_properties
@require_context
def generate_mlq_indicator(ctx):
  # expert data
  

  # Contexts
  expert_ctx = ExpertContext(ctx['properties']['name'])

  # Prepare properties lines
  expert_ctx.property_lines = get_common_property_c_lines()
  # Prepear property buffer lines
  expert_ctx.property_buffers = get_buffer_property_c_lines()
  # Prepear property buffer C registers
  # Prepare input lines
  expert_ctx.input_lines = get_input_c_lines()
  # Prepare DLL import lines


  # Write from template
  with open('templates/Indicator.cpp', 'r') as fin:
    template = Template(fin.read())
    code = template.render(ctx=expert_ctx)
  return code

def generate_dll_expert(): pass
def generate_dll_indicator(): pass
def generate_mql_buffers(): pass
def generate_dll_buffers(): pass
def generate_mql_ft(): pass
def generate_dll_ft(): pass

#@require_functions
@require_buffers
@require_properties
@require_context
def generate_all(ctx):
  code = generate_mlq_indicator()
  print(code)
  return
