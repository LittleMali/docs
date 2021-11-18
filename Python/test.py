#codeing=utf-8

import os
import sys
import configparser

cfgFile = "test.ini"

conf = configparser.ConfigParser()
conf.read(cfgFile)

if not "DeleteFuncs" in conf.sections():
    conf.add_section("DeleteFuncs")

conf.set("DeleteFuncs", "Ngn", "1")

conf.write(open(cfgFile, "w"))


conf1 = configparser.ConfigParser()
conf1.read(cfgFile)

if not "DeleteFuncs" in conf1.sections():
    conf1.add_section("DeleteFuncs")
    
conf1.set("DeleteFuncs", "ToolBox", "1")

conf1.write(open(cfgFile, "w"))
