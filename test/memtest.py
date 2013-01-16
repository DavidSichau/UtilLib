#!/usr/bin/python
# run valgrind's memory error checker on all tests.
# filter uninteresting errors and known false positives
#

#
# This file is based on kdevplatform/veritas/tests/runMemcheck.py
# by the KDevelop project, www.kdevelop.org
#

from os import system, name, remove, environ, defpath, path, pathsep, X_OK, access, sep
from sys import exit, stdout, platform
from subprocess import Popen, PIPE
from xml.dom.minidom import parse, parseString

##
# @brief filter for valgridn output
# @param line a line
# @return the line if not garbage
def garbage(line):
    return not line.startswith('<unknown program name>') and \
            not line.startswith('profiling:')

##
# @brief run valgrind-memcheck on test in testdir. return xml output as string
# @param test the test executed
# @return the filtered output of valgrind
def memcheck(test):
    v = find_valgrind()
    if platform == "darwin": #mac stuff
        cmd = v+" --tool=memcheck --dsymutil=yes --child-silent-after-fork=yes --leak-check=full --xml=yes --xml-fd=3 --num-callers=50 " + test + " 3>memcheck.tmp"
    else: #linux stuff
        cmd = v+" --tool=memcheck --child-silent-after-fork=yes --leak-check=full --xml=yes --xml-fd=3 --num-callers=50 " + test + " 3>memcheck.tmp"
    system(cmd)
    out = open("memcheck.tmp").readlines()
    out = filter(garbage, out)
    return ''.join(out)

##
# @brief  extract child data for tag.
# @param dom the dom
# @param tag the tag
# @return None if not found
def xml_child_data(dom,tag):
    elem = dom.getElementsByTagName(tag)
    val = None
    if len(elem) != 0:
        val = elem[0].firstChild.data
    return val

##
# @brief single entry in a memory error backtrace
class Frame:
    ##
    # @brief constructor
    # @param dom_frame the dom frame
    # @return nothing
    def __init__(self, dom_frame):
        '''<frame>
        <ip>0x62ACDBF</ip>
        <obj>/home/nix/KdeDev/kde4/lib/libkdevplatformlanguage.so.1.0.0</obj>
        <fn>KDevelop::ParamIterator::ParamIterator(QString, QString, int)</fn>
        <dir>/home/nix/KdeDev/kdevplatform/language/duchain</dir>
        <file>stringhelpers.cpp</file>
        <line>292</line>
        </frame>'''
        self.obj   = xml_child_data(dom_frame, 'obj')
        self.func  = xml_child_data(dom_frame, 'fn')
        self.sfile = xml_child_data(dom_frame, 'file')
        self.sline = xml_child_data(dom_frame, 'line')

    ##
    # @brief the conversion to string operator
    # @return The converted string
    def __str__(self):
        out = ""
        if self.func:
            out += "\t" + self.func
        if self.sfile and self.sline:
            out += " (" + self.sfile + ":" + self.sline + ")"
        out += "\n"
        return out

##
# @brief valgrind memcheck stack trace
class BackTrace:
    ##
    # @brief the constructor
    # @param errordom the error dom
    # @return nothing
    def __init__(self, errordom):
        self.dom = errordom
        self.kind = self.dom.getElementsByTagName('kind')[0].firstChild.data
        stack = self.dom.getElementsByTagName('frame')
        self.stack = []
        for frame in stack:
            if xml_child_data(frame, 'fn'): # filter anonymous frames out
                self.stack.append(Frame(frame))
        self.what = xml_child_data(self.dom, 'what')

    ##
    # @brief checks if it is definetively lost
    # @return true if definetively lost entry
    def is_definitely_lost(self):
        return self.kind == u'Leak_DefinitelyLost'

    ##
    # @brief the conversion to string operator
    # @return The converted string
    def __str__(self):
        out = ""
        for frame in self.stack:
            out += str(frame)
        return out

##
# @brief extract the interesting memcheck errors from the xml-string input 'out'
# @param out the xml-string input
# @return return these as a list
def parse_errors(out):
xmldoc = parseString(out)
    errors = xmldoc.getElementsByTagName('error')
    errors_ = []
    for error in errors:
        bt = BackTrace(error)
        if bt.is_definitely_lost():
            errors_.append(bt)
    return errors_

##
# @brief from: http://mail.python.org/pipermail/python-list/2002-August/157829.html
# @param filename the filename
# @return true if exists
def which (filename):
    if not environ.has_key('PATH') or environ['PATH'] == '':
        p = defpath
    else:
        p = environ['PATH']

    pathlist = p.split (pathsep)

    for thepath in pathlist:
        f = thepath+sep+filename
        if access(f, X_OK):
            return f
    return None

##
# @brief searches for valgrind
# @return the path to valgrind
def find_valgrind():
    valgrind = which('valgrind')
    if valgrind != None:
        return valgrind
    else:
        print ("valgrind NOT FOUND")
        exit(-1)

##
# @brief runs a test
# @param exe_name the executable path
# @return nothing
def run_single_test(exe_name):
    if access(exe_name, X_OK):
        pass
    else:
        print ("executable "+exe_name+" NOT FOUND")
        exit(-1)

    print (">> running valgrind memcheck on " + exe_name)
    out = memcheck(exe_name)
    errors = parse_errors(out)
    if len(errors) == 0:
        print "PASS"
        exit(0)
    else:
        for trace in errors:
            print (trace),
            print "---------------------------------------------------"
        print("FAIL")
        exit(-1)

################### ENTRY ####################################################

if __name__ == '__main__':
    import sys
    if len(sys.argv) > 1: run_single_test(sys.argv[1])
    else:
        print "usage: ./runMemcheck.py test_executable"
        exit(-1)

