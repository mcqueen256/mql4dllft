import click, os, json, jinja2

def properties():
    """Returns the decoded form of 'properties.json.'"""
    with open('properties.json', 'r') as fin:
        return json.loads(fin.read())

def check_path(path):
    """check_path(path: str)
    check_path(...) follows the given path variable and ensures that the path exists.
    """
    checked = './'
    for d in path.strip('/').split('/'):
        to_check = checked + '/' + d
        if not os.path.isdir(to_check) or not os.path.exists(to_check):
            os.mkdir(to_check)
        checked += '/' + d


def convert(srcdir, destdir, extention='.cpp'):
    """convert(srcdir: str, destdir: str [, extention: str = '.cpp'])
    Converts the templates in the srcdir to source code in the destdir. Overwrites existing files!

    @param srcdir is the source directory the template files are stored in
    @param destdir is the destination directory the completed template files will go
    @param extention is file extention filter, only file with that extention will be templated 
    """
    check_path(destdir)
    for file_name in os.listdir(srcdir):
        if not file_name.endswith(extention): continue
        with open(srcdir + file_name, 'r') as fin:
            with open(destdir + file_name, 'w') as fout:
                # Build templater environment (so that includes are accessable)
                include_dir = 'templates/'
                loader = jinja2.FileSystemLoader([include_dir, srcdir])
                environment = jinja2.Environment(loader=loader)
                template = environment.get_template(file_name)
                fout.write(template.render(**properties()))
                click.echo('generator: {} -> {}'.format(srcdir + file_name, destdir + file_name))

def cpp():
    # gather list of files
    convert('templates/src/', 'src/')
    convert('templates/include/', 'include/', extention='.hpp')
    convert('templates/test/src/', 'test/src/')

def expert():
    convert('templates/expert/', 'build/')

def indicators():
    convert('templates/indicators/', 'build/')

def export():
    convert('templates/export/', './', extention='.bat')

@click.group(invoke_without_command=True)
@click.pass_context
def cli(ctx):
    """"""
    # if there is no command, generate everything
    if ctx.invoked_subcommand is None:
        cpp()
        expert()
        indicators()
        export()

@cli.command('cpp')
def cli_cpp():
    expert()

@cli.command('expert')
def cli_expert():
    expert()

@cli.command('indicators')
def cli_indicators():
    indicators()

@cli.command('export')
def cli_export():
    export()

if __name__ == '__main__':
    cli()