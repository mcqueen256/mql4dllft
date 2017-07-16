from MQLFunction import MQLFunction
from .param_decoder import param_decoders
from bs4.element import Tag
import logging


def get_function_comment(tag):
    # set comment
    p = tag
    return p.span.string


def pull_apart_code_example(tag):
    # pull apart code group
    p = tag.table.tr.td.p
    code_groups = [[]]
    for child in p.children:
        if type(child) != Tag: continue
        if child.name == 'br':
            code_groups.append([])
            continue
        else:
            code_groups[-1].append(child)
    return code_groups


def get_function_return_type(group):
    # set return type
    for tag in group:
        if tag.name == 'span' and tag['class'][0] == 'f_Keywords':
            return tag.string.strip()
        else:
            continue
    return None


def get_function_name(group):
    # set function name
    for tag in group:
        if tag.name == 'span' and tag['class'][0] == 'f_Functions':
            return tag.string.strip()
        else:
            continue
    return None


def get_function_parameters(code_groups):
    parameters = []
    for parameter_spans in code_groups:
        signature = '.'.join([span['class'][0][2:].lower() for span in parameter_spans])
        if signature in param_decoders.keys():
            parameters.append(param_decoders[signature](parameter_spans))
        else:
            # this one is okay
            if signature != 'keywords.functions.codeexample':
                logging.warning("signature not processed: " + signature + str(code_groups))
    return parameters[:-1]


def get_function_return_comment(tags):
    last_title = None
    for tag in tags[3:]:
        if tag.name.startswith('h'):
            last_title = None
        # if tag is a title
        if 'class' in tag.attrs.keys():
            if tag['class'][0] == 'p_BoldTitles':
                last_title = tag.span.string
            else:
                if (tag['class'][0] == 'p_FunctionRemark' or tag['class'][
                    0] == 'p_Function') and last_title is not None:
                    if last_title.lower().find('return') != -1:
                        return tag.get_text()
    return None


def get_function_note(tags):
    last_title = None
    for tag in tags[3:]:
        if tag.name.startswith('h'):
            last_title = None
        # if tag is a title
        if 'class' in tag.attrs.keys():
            if tag['class'][0] == 'p_BoldTitles':
                last_title = tag.span.string
            else:
                if tag['class'][0] == 'p_FunctionRemark' and last_title is not None:
                    if last_title.lower().find('note') != -1:
                        logging.debug('get_function_note:' + tag.get_text())
                        return tag.get_text()
    return None


def page_decode_normal(tags):
    """Analyse the tags provided and return a MQLFunction representation."""
    f = MQLFunction()

    f.setComment(get_function_comment(tags[1]))
    logging.debug('comment:' + str(f.getComment()))

    code_groups = pull_apart_code_example(tags[2])

    f.setReturnType(get_function_return_type(code_groups[0]))
    logging.debug('return_type:' + str(f.getReturnType()))

    f.setName(get_function_name(code_groups[0]))
    logging.debug('name:' + str(f.getName()))

    # set parameters
    f.setParameters(get_function_parameters(code_groups))
    for i, param in enumerate(f.getParameters()):
        logging.debug('param:' + str(i + 1) + ':' + str(param))

    # set return comment
    f.setReturnComment(get_function_return_comment(tags))
    logging.debug("return_comment:" + str(f.getReturnComment()))

    # set note
    f.setNote(get_function_note(tags))
    logging.debug("note:" + str(f.getNote()))
    return f
