import os
import urllib.request

import click
from bs4 import BeautifulSoup
from bs4.element import Tag

@click.command()
@click.option('--skip-download', is_flag=True, help='Skips downloading of the MLQ4 API pages.')
@click.option('--verbos', is_flag=True)
@click.option('--debug', is_flag=True)
def download_api(skip_download, verbos, debug):
  """Handles downloading the MQL4 API. If the API has not been downloaded or is
  partially downloaded then this downloader will continue the download process
  where it left off."""

  # chekc for a skip
  if skip_download: return

  def get_html(url):
    """Download the page given the url."""
    class AppURLopener(urllib.request.FancyURLopener):
      version = "Mozilla/5.0"

    if debug:
      click.echo("Downloading from: " + url)
    opener = AppURLopener()
    response = opener.open(url)
    if response.getcode() != 200:
      if debug: click.echo(response.getcode(), ":", url)
    content = response.file.read().decode()
    opener.close()
    if debug: click.echo("Done.")
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
      if debug: click.echo(name)
      if (name + '.html' in os.listdir('api/')): continue
      page = get_html('/'.join(url.split('/')[:-1]) + href)
      with open('api/' + name + '.html', 'w') as file:
        file.write(page)
        if debug: click.echo("Written to: " + 'api/' + name + '.html')
  pass

@click.command()
def decode_api():
  pass

@click.command()
def decode_buffers():
  pass

@click.command()
def generate_output():
  pass


def main():
  """Steps in generating the API:
  1. Download the MQL4 API
  2. Decode the Websites into function representation objects (FROs)
  3. Load/decode buffer objects from file
  4. Generate selected output

  Optionals:
    - Generate code for a new Indicator
    - Generate code for a new Expert Advisor
    - Generate code for a DLL
    - Generate all the code
    - Export generated code
  """
  download_api()
  decode_api()
  decode_buffers()
  generate_output()

if __name__ == '__main__':
  main()