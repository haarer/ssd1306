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

def print_help_and_exit():
    print "Usage: lcd_code_generator.py [args]"
    print "args:"
    print "      -c name   controller name"
    print "      -b bits   bits per pixel"
    print "      -r WxH    resolution"
    print "      -t template path source templates, templates by default"
    exit(1)

if len(sys.argv) < 2:
    print_help_and_exit()

g_controller=""
g_lcontroller=""
g_resolution=""
g_bits=""
g_exbits=""
g_width=""
g_height=""

types=[]
controller=""
templates="templates"
resolution_db = []

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
        types.append({"bits": bits, "resolution": resolution_db})
        resolution_db = []
    elif opt == "-t":
        idx += 1
        templates = sys.argv[idx]
    elif opt == "-r":
        idx += 1
        resolution = sys.argv[idx]
        resolution_db.append( resolution )
        for t in types:
            t["resolution"].append( resolution )
    else:
        print "Unknown option: ", opt
        print_help_and_exit()
    idx += 1

def get_file_data(fname):
    with open(templates + fname, 'r') as myfile:
        data=myfile.read().replace('~CONTROLLER~', g_controller).\
                           replace('~controller~', g_lcontroller).\
                           replace('~RESOLUTION~', g_resolution).\
                           replace('~EXBITS~', g_exbits).\
                           replace('~BITS~', g_bits).\
                           replace('~WIDTH~', g_width).\
                           replace('~HEIGHT~',g_height)
    return data;

templates = templates + "/"
g_controller = controller.upper()
g_lcontroller = controller.lower()

shutil.rmtree(g_lcontroller,True)
os.mkdir(g_lcontroller)
header = open(g_lcontroller + "/lcd_" + g_lcontroller + ".h", "w" )
inl = open(g_lcontroller + "/lcd_" + g_lcontroller + ".inl", "w" )
cpp = open(g_lcontroller + "/lcd_" + g_lcontroller + ".cpp", "w" )

header.write( get_file_data('copyright.txt') )
inl.write( get_file_data('copyright.txt') )
cpp.write( get_file_data('copyright.txt') )

header.write( get_file_data('header.h') )
inl.write( get_file_data('header.inl') )
cpp.write( get_file_data('header.cpp') )

header.write( get_file_data('interface_spi.h') )
inl.write( get_file_data('interface_spi.inl') )

for t in types:
    g_bits = str(t["bits"])
    g_exbits = "x" + g_bits
    header.write( get_file_data( 'display.h' ) )
    inl.write( get_file_data( 'display.inl' ) )
    for res in t["resolution"]:
        g_resolution = res + g_exbits;
        [ g_width, g_height ] = res.split('x')
        header.write( get_file_data('resolution.h') )
        inl.write( get_file_data('resolution.inl') )
        header.write( get_file_data('display_spi.h') )
        cpp.write( get_file_data('display_spi.cpp') )

header.write( get_file_data('footer.h') )

header.close()
inl.close()
cpp.close()
