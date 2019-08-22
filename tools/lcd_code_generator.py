#!/usr/bin/python
# -*- coding: UTF-8 -*-
#    MIT License
#
#    Copyright (c) 2019, Alexey Dynda
#
#    Permission is hereby granted, free of charge, to any person obtaining a copy
#    of this software and associated documentation files (the "Software"), to deal
#    in the Software without restriction, including without limitation the rights
#    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#    copies of the Software, and to permit persons to whom the Software is
#    furnished to do so, subject to the following conditions:
#
#    The above copyright notice and this permission notice shall be included in all
#    copies or substantial portions of the Software.
#
#    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#    SOFTWARE.
#
###################################################################################

import re
import sys
import os
import shutil
import json

def print_help_and_exit():
    print "Usage: lcd_code_generator.py [args]"
    print "args:"
    print "      -c name   controller name"
    print "      -b bits   bits per pixel"
    print "      -B        Do not add bits to screen class name"
    print "      -r WxH    resolution"
    print "      -j json   Use json data from file"
    print "      -t template path source templates, templates by default"
    exit(1)

if len(sys.argv) < 2:
    print_help_and_exit()

g_voc={}

gs_json=None
controller=""
templates="templates"
resolution_db = []
g_voc = {
          "options":
          {
              "no_bits_in_name": False,
              "args_in_cmd_mode": False,
              "rowcol_bits": 8,
              "col_cmd": "0x15",
              "row_cmd": "0x75",
              "use_paging": False,
          },
          "bits": {},
          "CONTROLLER": "",
          "controller": "",
          "resolution": "",
          "exbits": "",
          "_bits": "",
          "width": "",
          "height": "",
          "init_data": "" }

# parse args
idx = 1
while idx < len(sys.argv):
    opt = sys.argv[idx]
    if opt == "-c":
        idx += 1
        controller = sys.argv[idx]
    elif opt == "-b":
        idx += 1
        bits = int(sys.argv[idx])
        g_voc["bits"].append({"bits": zip( resolution_db, [{}] * len(resolution_db) )})
        resolution_db = []
    elif opt == "-B":
        g_voc["options"]["no_bits_in_name"] = True
    elif opt == "-t":
        idx += 1
        templates = sys.argv[idx]
    elif opt == "-j":
        idx += 1
        gs_json = sys.argv[idx]
    elif opt == "-r":
        idx += 1
        resolution = sys.argv[idx]
        resolution_db.append( resolution )
        for t in g_voc["bits"]:
            t.append( { resolution: {} } )
    else:
        print "Unknown option: ", opt
        print_help_and_exit()
    idx += 1

def get_val_by_path(path, default):
    nodes = path.split('/')
    data = g_voc;
    for i in range(0, len(nodes) - 1):
        data = data.get(nodes[i],{})
    return data.get( nodes[-1], default )

def get_file_data(fname):
    with open(templates + fname, 'r') as myfile:
        data=myfile.read().replace('~CONTROLLER~', get_val_by_path("CONTROLLER","")).\
                           replace('~controller~', get_val_by_path("controller","")).\
                           replace('~RESOLUTION~', get_val_by_path("resolution","")).\
                           replace('~EXBITS~', get_val_by_path("exbits","")).\
                           replace('~BITS~', get_val_by_path("_bits","")).\
                           replace('~WIDTH~', get_val_by_path("width","")).\
                           replace('~HEIGHT~',get_val_by_path("height","")).\
                           replace('~INIT~', get_val_by_path("init_data","")).\
                           replace('~CONFIG_FUNC~', get_val_by_path("options/config_func","_configureSpiDisplay")).\
                           replace('~SET_BLOCK~', get_val_by_path("_set_block",""))
    return data;

def get_json_controller_list(fname):
    with open(templates + 'lcd/' + fname) as json_file:
        data = json.load(json_file)
    return data.keys()

def load_data_from_json(fname, ctl):
    with open(templates + 'lcd/' + fname) as json_file:
        data = json.load(json_file)
    g_voc["bits"] = data[ctl]["bits"];
    g_voc["options"] = data[ctl]["options"]

def load_init_data_from_json(fname, ctl, bits, resolution):
    with open(templates + 'lcd/' + fname) as json_file:
        data = json.load(json_file)
    g_voc["init_data"] = "\n".join( data[ctl]["bits"][bits][resolution]["init"] )

templates = templates + "/"

def generate_set_block_content():
    lines = ["    lcduint_t rx = w ? (x + w - 1) : (m_base.width() - 1);",
             "    commandStart();",
             "    this->send({0});".format(get_val_by_path("options/col_cmd", "0x22")) ]
    if not get_val_by_path("options/args_in_cmd_mode", False):
        lines.append("    spiDataMode(1);  // According to datasheet all args must be passed in data mode")
    if get_val_by_path("options/rowcol_bits",8) != 8:
        lines.append("    this->send(0);")
    if get_val_by_path("options/column_div", 1) > 1:
        lines.append( "    this->send(x / {0});".format(get_val_by_path("options/column_div", 1)) )
    else:
        lines.append( "    this->send(x);" )
    if get_val_by_path("options/rowcol_bits",8) != 8:
        lines.append("    this->send(0);")
    if get_val_by_path("options/column_div", 1) > 1:
        lines.append( "    this->send( (rx < m_base.width() ? rx : (m_base.width() - 1)) / {0} );".format(
                     get_val_by_path("options/column_div", 1)) )
    else:
        lines.append( "    this->send( rx < m_base.width() ? rx : (m_base.width() - 1) );" )

    if not get_val_by_path("options/args_in_cmd_mode", False):
        lines.append("    spiDataMode(0);")
    lines.append("    this->send({0});".format(get_val_by_path("options/row_cmd", "0x72")))
    if not get_val_by_path("options/args_in_cmd_mode", False):
        lines.append("    spiDataMode(1);  // According to datasheet all args must be passed in data mode")
    if get_val_by_path("options/rowcol_bits",8) != 8:
        lines.append("    this->send(0);")
    lines.append("    this->send(y);")
    if get_val_by_path("options/rowcol_bits",8) != 8:
        lines.append("    this->send(0);")
    lines.append("    this->send(m_base.height() - 1);")
    if not get_val_by_path("options/args_in_cmd_mode", False):
        lines.append("    spiDataMode(0);")
    if get_val_by_path("options/exit_cmd_mode_command", None) is not None:
        lines.append("    this->send({0});".format( get_val_by_path("options/exit_cmd_mode_command", None) ) )
    lines.extend( [ "    if (m_dc >= 0)",
                    "    {",
                    "        spiDataMode(1);",
                    "    }",
                    "    else",
                    "    {",
                    "        this->stop();",
                    "        this->start();",
                    "        this->send(0x40);",
                    "    }" ] )
    return "\n".join(lines)

def generate_controller_data(ctl):
    controller = ctl.lower();
    g_voc["CONTROLLER"] = controller.upper()
    g_voc["controller"] = controller.lower()

    if gs_json is not None:
        load_data_from_json( gs_json, ctl )
    g_voc["_set_block"] = generate_set_block_content()

    shutil.rmtree(controller,True)
    os.mkdir(controller)
    header = open(controller + "/lcd_" + controller + ".h", "w" )
    inl = open(controller + "/lcd_" + controller + ".inl", "w" )
    cpp = open(controller + "/lcd_" + controller + ".cpp", "w" )

    header.write( get_file_data('copyright.txt') )
    inl.write( get_file_data('copyright.txt') )
    cpp.write( get_file_data('copyright.txt') )

    header.write( get_file_data('header.h') )
    inl.write( get_file_data('header.inl') )
    cpp.write( get_file_data('header.cpp') )

    header.write( get_file_data('interface_spi.h') )
    inl.write( get_file_data('interface_spi.inl') )


    for _bits in g_voc["bits"].keys():
        g_voc["_bits"] = _bits;
        if not get_val_by_path("options/no_bits_in_name", False):
            g_voc["exbits"] = "x" + g_voc["_bits"]
        header.write( get_file_data( 'display.h' ) )
        inl.write( get_file_data( 'display.inl' ) )
        for res in g_voc["bits"][_bits].keys():
            g_voc["resolution"] = res + g_voc["exbits"]
            g_voc["init_data"] = get_file_data('init_data.inl')
            if gs_json is not None:
                load_init_data_from_json(gs_json, ctl, _bits, res)
            g_voc["width"] = res.split('x')[0]
            g_voc["height"] = res.split('x')[1]
            header.write( get_file_data('resolution.h') )
            inl.write( get_file_data('resolution.inl') )
            header.write( get_file_data('display_spi.h') )
            cpp.write( get_file_data('display_spi.cpp') )

    header.write( get_file_data('footer.h') )

    header.close()
    inl.close()
    cpp.close()

if gs_json is not None:
    _ctl_list = get_json_controller_list(gs_json)
    for _ctl in _ctl_list:
        generate_controller_data(_ctl)
else:
    generate_controller_data(controller)
