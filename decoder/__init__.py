from .MQLFunction import MQLFunction
from typing import List
from tqdm import tqdm
from bs4 import BeautifulSoup
from bs4.element import Tag
import os
import click
from .page_decoder import page_decode_normal


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
  for filename in tqdm(file_list):
    # only want to process html files
    if not filename.endswith('.html'): continue
    with open('api/' + filename, 'r') as file:
      function = get_mql_function(file.read(), filename)

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
    if f is not None: f._filename = filename
    return f

def decode_buffers():
  pass
