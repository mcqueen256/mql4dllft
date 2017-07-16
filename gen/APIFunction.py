from decoder import decode_functions

import json


def as_mqlfunction(dic):
    if 'mqlfunctions' in dic:
        pass


def generate_new_json_file():
    functions = decode_functions()
    func_dict_by_namespace = {}
    for function in functions:
        if function is not None:
            if function.getNamespace() not in func_dict_by_namespace.keys():
                func_dict_by_namespace[function.getNamespace()] = []
            func_dict_by_namespace[function.getNamespace()].append(function.as_dict())

    with open('functions.json', 'w') as fout:
        fout.write(json.dumps({
            'mqlfunctions': func_dict_by_namespace,
        }, indent=4))
