def set_return_type(span): return span.string.strip().replace('\xa0', ' ')


def param_decoder0(spans):
    """codeexample.keywords.codeexample.param.codeexample.comments
    Example:
        <span class="f_CodeExample">   </span>
        <span class="f_Keywords">string</span>
        <span class="f_CodeExample">  </span>
        <span class="f_Param">symbol</span>
        <span class="f_CodeExample">,     </span>
        <span class="f_Comments">// symbol</span>
    """
    return_type = set_return_type(spans[1])
    name = spans[3].string.strip().replace('&amp;', '&')
    default = None
    if len(name.split('=')) == 2:
        default = name.split('=')[1]
        name = name.split('=')[0]
    comment = spans[5].string.strip()[3:].replace('\xa0', ' ')
    return return_type, name, default, comment


def param_decoder1(spans):
    """codeexample
    Example:
        <span class="f_CodeExample">   );</span>
    """
    return None


def param_decoder2(spans):
    """codeexample.keywords.codeexample.param.codeexample.param.codeexample.comments
    Example:
        <span class="f_CodeExample">   </span>
        <span class="f_Keywords">int</span>
        <span class="f_CodeExample">            </span>
        <span class="f_Param">sub_window</span>
        <span class="f_CodeExample">,</span>
        <span class="f_Param"> </span>
        <span class="f_CodeExample">           </span>
        <span class="f_Comments">// number of the subwindow</span>
    """
    return_type = set_return_type(spans[1])
    name = spans[3].string.strip().replace('&amp;', '&')
    default = None
    if len(name.split('=')) == 2:
        default = name.split('=')[1]
        name = name.split('=')[0]
    comment = spans[-1].string.strip()[3:].replace('\xa0', ' ')
    return return_type, name, default, comment


def param_decoder3(spans):
    '''codeexample.param.codeexample.comments
    Example:
        <span class="f_CodeExample">   </span>
        <span class="f_Param">argument</span>
        <span class="f_CodeExample">,     </span>
        <span class="f_Comments">// first value</span>
    '''
    return_type = None
    name = spans[3].string.strip().replace('&amp;', '&')
    default = None
    if len(name.split('=')) == 2:
        default = name.split('=')[1]
        name = name.split('=')[0]
    comment = spans[-1].string.strip()[3:].replace('\xa0', ' ')
    return return_type, name, default, comment


def param_decoder4(spans):
    '''param.codeexample.comments
    Example:
        <span class="f_Param">   ...</span>
        <span class="f_CodeExample">           </span>
        <span class="f_Comments">// other values</span>
    '''
    return_type = None
    name = '...'
    default = None
    comment = spans[-1].string.strip()[3:].replace('\xa0', ' ')
    return return_type, name, default, comment


def param_decoder5(spans):
    '''codeexample.keywords.param.param.codeexample.comments
    Example:
        <span class="f_CodeExample">   </span>
        <span class="f_Keywords">object</span>
        <span class="f_Param" style="color: #000000;">* </span>
        <span class="f_Param">anyobject</span>
        <span class="f_CodeExample">      </span>
        <span class="f_Comments">// object pointer</span>
    '''
    return_type = set_return_type(spans[1])
    name = spans[3].string.strip().replace('&amp;', '&')
    default = None
    if len(name.split('=')) == 2:
        default = name.split('=')[1]
        name = name.split('=')[0]
    comment = spans[-1].string.strip()[3:].replace('\xa0', ' ')
    return return_type, name, default, comment


def param_decoder6(spans):
    '''codeexample.keywords.param.codeexample.comments
    Example:
        <span class="f_CodeExample">   </span>
        <span class="f_Keywords">ENUM_CRYPT_METHOD   </span>
        <span class="f_Param">method</span>
        <span class="f_CodeExample">,        </span>
        <span class="f_Comments">// method</span>
    '''
    return_type = set_return_type(spans[1])
    name = spans[2].string.strip().replace('&amp;', '&')
    default = None
    if len(name.split('=')) == 2:
        default = name.split('=')[1]
        name = name.split('=')[0]
    comment = spans[-1].string.strip()[3:].replace('\xa0', ' ')
    return return_type, name, default, comment


def param_decoder7(spans):
    '''codeexample.keywords.param.codeexample.param.codeexample.comments
    Example:
        <span class="f_CodeExample">   </span>
        <span class="f_Keywords">uint&amp;             </span>
        <span class="f_Param">width</span>
        <span class="f_CodeExample">,</span>
        <span class="f_Param">            </span>
        <span class="f_CodeExample">   </span>
        <span class="f_Comments">// for receiving the image width in the resource</span>
    '''
    return_type = set_return_type(spans[1])
    name = spans[2].string.strip().replace('&amp;', '&')
    default = None
    if len(name.split('=')) == 2:
        default = name.split('=')[1]
        name = name.split('=')[0]
    comment = spans[-1].string.strip()[3:].replace('\xa0', ' ')
    return return_type, name, default, comment


def param_decoder8(spans):
    '''comments
    Example:
        <span class="f_Comments">   ...</span>
    '''
    return None, '...', None, None


def param_decoder9(spans):
    '''codeexample.keywords.codeexample.param.comments
    Example:
        <span class="f_CodeExample">   </span>
        <span class="f_Keywords">string</span>
        <span class="f_CodeExample">   </span>
        <span class="f_Param">object_name   </span>
        <span class="f_Comments">// object name</span>
    '''
    return_type = set_return_type(spans[1])
    name = spans[3].string.strip().replace('&amp;', '&')
    default = None
    if len(name.split('=')) == 2:
        default = name.split('=')[1]
        name = name.split('=')[0]
    comment = spans[-1].string.strip()[3:].replace('\xa0', ' ')
    return return_type, name, default, comment


def param_decoder10(spans):
    '''codeexample.keywords.param.comments
    Example:
        <span class="f_CodeExample">   </span>
        <span class="f_Keywords">string   </span>
        <span class="f_Param">object_name   </span>
        <span class="f_Comments">// object name</span>
    '''
    return_type = set_return_type(spans[1])
    name = spans[2].string.strip().replace('&amp;', '&')
    default = None
    if len(name.split('=')) == 2:
        default = name.split('=')[1]
        name = name.split('=')[0]
    comment = spans[-1].string.strip()[3:].replace('\xa0', ' ')
    return return_type, name, default, comment


def param_decoder11(spans):
    '''codeexample.keywords.codeexample.param.param.codeexample.comments
    Example:
        <span class="f_CodeExample">   </span>
        <span class="f_Keywords">int </span>
        <span class="f_CodeExample">       </span>
        <span class="f_Param">ticket</span>
        <span class="f_Param" style="color: #000000;">,</span>
        <span class="f_CodeExample">      </span>
        <span class="f_Comments">// ticket</span>
    '''
    return_type = set_return_type(spans[1])
    name = spans[3].string.strip().replace('&amp;', '&')
    default = None
    if len(name.split('=')) == 2:
        default = name.split('=')[1]
        name = name.split('=')[0]
    comment = spans[-1].string.strip()[3:].replace('\xa0', ' ')
    return return_type, name, default, comment


def param_decoder12(spans):
    '''codeexample.keywords.codeexample.param.param.param.codeexample.comments
    Example:
        <span class="f_CodeExample">   </span>
        <span class="f_Keywords">double </span>
        <span class="f_CodeExample">    </span>
        <span class="f_Param">lots</span>
        <span class="f_Param" style="color: #000000;">,</span>
        <span class="f_Param"> </span>
        <span class="f_CodeExample">       </span>
        <span class="f_Comments">// volume</span>
    '''
    return_type = set_return_type(spans[1])
    name = spans[3].string.strip().replace('&amp;', '&')
    default = None
    if len(name.split('=')) == 2:
        default = name.split('=')[1]
        name = name.split('=')[0]
    comment = spans[-1].string.strip()[3:].replace('\xa0', ' ')
    return return_type, name, default, comment


def param_decoder13(spans):
    '''codeexample.keywords.codeexample.param.param.comments
    Example:
        <span class="f_CodeExample">   </span>
        <span class="f_Keywords">string </span>
        <span class="f_CodeExample">  </span>
        <span class="f_Param">comment=NULL</span>
        <span class="f_Param" style="color: #000000;">,        </span>
        <span class="f_Comments">// comment</span>
    '''
    return_type = set_return_type(spans[1])
    name = spans[3].string.strip().replace('&amp;', '&')
    default = None
    if len(name.split('=')) == 2:
        default = name.split('=')[1]
        name = name.split('=')[0]
    comment = spans[-1].string.strip()[3:].replace('\xa0', ' ')
    return return_type, name, default, comment


def param_decoder14(spans):
    '''codeexample.keywords.codeexample.param.param.param.comments
    Example:
        <span class="f_CodeExample">   </span>
        <span class="f_Keywords">datetime</span>
        <span class="f_CodeExample"> </span>
        <span class="f_Param">expiration=0</span>
        <span class="f_Param" style="color: #000000;">,</span>
        <span class="f_Param">        </span>
        <span class="f_Comments">// pending order expiration</span>
    '''
    return_type = set_return_type(spans[1])
    name = spans[3].string.strip().replace('&amp;', '&')
    default = None
    if len(name.split('=')) == 2:
        default = name.split('=')[1]
        name = name.split('=')[0]
    comment = spans[-1].string.strip()[3:].replace('\xa0', ' ')
    return return_type, name, default, comment


param_decoders = {
    param_decoder0.__doc__.split('\n')[0].strip(): param_decoder0,
    param_decoder1.__doc__.split('\n')[0].strip(): param_decoder1,
    param_decoder2.__doc__.split('\n')[0].strip(): param_decoder2,
    param_decoder3.__doc__.split('\n')[0].strip(): param_decoder3,
    param_decoder4.__doc__.split('\n')[0].strip(): param_decoder4,
    param_decoder5.__doc__.split('\n')[0].strip(): param_decoder5,
    param_decoder6.__doc__.split('\n')[0].strip(): param_decoder6,
    param_decoder7.__doc__.split('\n')[0].strip(): param_decoder7,
    param_decoder8.__doc__.split('\n')[0].strip(): param_decoder8,
    param_decoder9.__doc__.split('\n')[0].strip(): param_decoder9,
    param_decoder10.__doc__.split('\n')[0].strip(): param_decoder10,
    param_decoder11.__doc__.split('\n')[0].strip(): param_decoder11,
    param_decoder12.__doc__.split('\n')[0].strip(): param_decoder12,
    param_decoder13.__doc__.split('\n')[0].strip(): param_decoder13,
    param_decoder14.__doc__.split('\n')[0].strip(): param_decoder14,
}
