import os
import urllib.request

import click
from bs4 import BeautifulSoup
from bs4.element import Tag

@click.group(invoke_without_command=True)
@click.option('--verbos', is_flag=True, help='Turn on verbos output (show all the details).')
@click.option('--debug', is_flag=True, help='Turn on debugging output.')
def cli(verbos, debug):
  """Steps in generating the API:

  \n\t1. Download the MQL4 API
  \n\t2. Decode the Websites into function representation objects (FROs)
  \n\t3. Load/decode buffer objects from file
  \n\t4. Generate selected output

  Optionals:
  \n\t- Generate code for a new Indicator
  \n\t- Generate code for a new Expert Advisor
  \n\t- Generate code for a DLL
  \n\t- Generate all the code
  \n\t- Export generated code
  """
  pass

@cli.command('download-api')
@click.pass_context
def download_api(ctx):
  """Handles downloading the MQL4 API. If the API has not been downloaded or is
  partially downloaded then this downloader will continue the download process
  where it left off."""

  print("Downloading")
  return

  def get_html(url):
    """Download the page given the url."""
    class AppURLopener(urllib.request.FancyURLopener):
      version = "Mozilla/5.0"

    if ctx.obj['DEBUG']:
      click.echo("Downloading from: " + url)
    opener = AppURLopener()
    response = opener.open(url)
    if response.getcode() != 200:
      if ctx.obj['DEBUG']: click.echo(response.getcode(), ":", url)
    content = response.file.read().decode()
    opener.close()
    if ctx.obj['DEBUG']: click.echo("Done.")
    return content

  # Donwload and decode the function index page
  url = 'https://docs.mql4.com/function_indices'
  html = get_html(url)
  soup = BeautifulSoup(html, "html.parser")
  table = soup.find('table', {'class': 'EnumTable'})
  link_elements = table.findAll('a', {'class': 'topiclink'})
  for a in link_elements:
    if len(a['href'].split('/')) == 3:
      href = a['href'].split('#')[0]
      name = '.'.join(href.split('/')[-2:])
      if ctx.obj['DEBUG']: click.echo(name)
      if (name + '.html' in os.listdir('api/')): continue
      page = get_html('/'.join(url.split('/')[:-1]) + href)
      with open('api/' + name + '.html', 'w') as file:
        file.write(page)
        if ctx.obj['DEBUG']: click.echo("Written to: " + 'api/' + name + '.html')
  pass

def decode_api():
  pass

@cli.command('--gen-buffer')
@click.pass_context
def decode_buffers():
  pass

@cli.command('--gen-code')
@click.pass_context
def generate_output():
  pass

def generate_indicator():
  pass

def generate_expert():
  pass




if __name__ == '__main__':
  cli(obj={})