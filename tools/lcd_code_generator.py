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
    print "      -r WxH    resolution"
    print "      -b bits   bits per pixel"
    exit(1)

if len(sys.argv) < 2:
    print_help_and_exit()

controller=""
types=[]

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
        types.append({"bits": bits, "resolution": []})
    elif opt == "-r":
        idx += 1
        resolution = sys.argv[idx]
        for t in types:
            t["resolution"].append( resolution )
    else:
        print "Unknown option: ", opt
        print_help_and_exit()
    idx += 1

shutil.rmtree(controller,True)
os.mkdir(controller)
header = open(controller + "/lcd_" + controller + ".h", "w" )
inl = open(controller + "/lcd_" + controller + ".inl", "w" )
cpp = open(controller + "/lcd_" + controller + ".cpp", "w" )

with open('templates/interface_spi.h', 'r') as myfile:
    data=myfile.read().replace('~CONTROLLER~', controller)
header.write( data )

with open('templates/interface_spi.inl', 'r') as myfile:
    data=myfile.read().replace('~CONTROLLER~', controller)
inl.write( data )

for t in types:
    for res in t["resolution"]:
        resolution = res;
        if len(types) > 1:
            resolution = resolution + "x" + str(t["bits"])
        with open('templates/resolution.h', 'r') as myfile:
            data=myfile.read().replace('~CONTROLLER~', controller).\
                replace('~BITS~',str(t["bits"])).replace('~RESOLUTION~',resolution)
        header.write( data )
        with open('templates/resolution.inl', 'r') as myfile:
            data=myfile.read().replace('~CONTROLLER~', controller).\
                replace('~BITS~',str(t["bits"])).replace('~RESOLUTION~',resolution)
        inl.write( data )
        with open('templates/display_spi.h', 'r') as myfile:
            data=myfile.read().replace('~CONTROLLER~', controller).\
                replace('~BITS~',str(t["bits"])).replace('~RESOLUTION~',resolution)
        header.write( data )
        with open('templates/display_spi.cpp', 'r') as myfile:
            data=myfile.read().replace('~CONTROLLER~', controller).\
                replace('~BITS~',str(t["bits"])).replace('~RESOLUTION~',resolution)
        cpp.write( data )

header.close()
inl.close()
cpp.close()
