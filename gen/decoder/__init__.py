from .MQLFunction import MQLFunction
from typing import List
from tqdm import tqdm
from bs4 import BeautifulSoup
from bs4.element import Tag
import os
import click
from .page_decoder import page_decode_normal
import logging
import json
import core


def decode_functions() -> List[MQLFunction]:
  '''Returns a list of the code signatures for every function in the API.'''
  # Get the file list
  try:
    file_list = os.listdir('api/')
  except FileNotFoundError as e:
    click.echo('Error: The \'api/\' directory is missing. Try: python gen.py --download-api')
    exit(1) # error

  # process the files
  functions = []
  click.echo("Analysing html files...")
  for filename in tqdm(file_list):
    # only want to process html files
    if not filename.endswith('.html'): continue
    with open('api/' + filename, 'r') as file:
      function = get_mql_function(file.read(), filename)
      # TODO: will need to remove next line some time
      if function is None: continue

      # check if the function needs to be duplicated to match variable parameters
      need_dupping = False
      dups = []
      for t, n, d, c in function.getParameters():
        # looking for (None, '...', None, None)
        if t is None and d is None and c is None and n == '...':
          need_dupping = True
          continue
        if need_dupping:
          param = t, n, d, c
          dups.append(param)
      # if in need of duplication, do it now
      if need_dupping:
        bear_params = function.getParameters()[:-(len(dups) * 2 + 1)]
        for maximum in range(1, core.settings["DUP_LEVEL"] + 1):
          f = function.copy()
          params = bear_params[:]
          for i in range(1, maximum + 1):
            for t, n, d, c in dups:
              n = n[::-1].replace('N', str(i), 1)[::-1]
              param = t, n, d, c
              params.append(param)
          f.setParameters(params)
          functions.append(f)
      else:
        functions.append(function)
  return functions

def get_mql_function(html, filename):
  """Extracts the fucntion signature the html file."""
  soup = BeautifulSoup(html, "html.parser")
  # get the document node
  doc_node = soup.find('td', {'class': 'doctext'}).div

  # seperate the children
  tags = []
  for child in doc_node.children:
    if type(child) != Tag: continue
    tags.append(child)

  # select correct processing function
  tag_names = [tag.name for tag in tags]
  f = None # the function
  if '.'.join(tag_names).startswith('h1.p.div.p'): f = page_decode_normal(tags)
  elif '.'.join(tag_names).startswith('h1.p.p.div.p.div.p'): pass # print('process double code block', filename)
  elif '.'.join(tag_names).startswith('h1.p.p.div.p.p.p.p.p.br'): pass # getlasterror
  #TODO: throw error for none instances
  if f is not None: f.setNamespace(filename.split('.')[0])
  logging.info(filename)
  return f

def decode_buffers():
  with open('buffers.json', 'r') as fin:
    return json.loads(fin.read())['buffers']

def decode_properties():
  with open('properties.json', 'r') as fin:
    return json.loads(fin.read())
