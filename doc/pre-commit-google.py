#! /usr/bin/env python
##
# @file pre-commit-google.py
# @brief file for testing the git commit for accordance to google style of code
# @author David Sichau <mail"at"sichau"dot"eu>
# @version 1.0
# @date 2012-12-21


import os
import subprocess
import filecmp
import re



##
# @brief the path to the root
pathToGitRoot = subprocess.Popen(['git','rev-parse','--show-toplevel'], stdout=subprocess.PIPE).communicate()[0].splitlines()[0]
##
# @brief where the cfg file for the uncrustifies is stored
uncrustifyCfg = pathToGitRoot + '/doc/google.cfg'
##
# @brief path to the uncrustifier executable
uncrustifyExecutable = subprocess.Popen(['which','uncrustify'], stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()[0].rstrip()
if len(uncrustifyExecutable) == 0:
    print "if you get an exception make sure that uncrustify is actually found by the script"
    uncrustifyExecutable = '/opt/local/bin/uncrustify'

##
# @brief checks if the file is a C++ file
#
# @param fileName the filename
#
# @return true if the file is a c++ file
def isCPPFile(fileName):
    filePath, fileExtension = os.path.splitext(fileName)
    cppExtensions = ['.cpp','.cxx','.h','.hpp','.hxx']
    if fileExtension in cppExtensions:
        return True
    else:
        return False

##
# @brief checks the format of the file with uncrustify
#
# @param fileName the filename
#
# @return True if the format is good and False if not
def checkFormat(fileName):
    filePath, fileExtension = os.path.splitext(fileName)
    tempfile = filePath + '.uncrustify' + fileExtension
    uncrustifyCmd = [uncrustifyExecutable,'-c',uncrustifyCfg,'-l','CPP','-f',fileName,'-o',tempfile]
    retCode = subprocess.call(uncrustifyCmd, stderr=subprocess.PIPE)
    if retCode == 0:
        if filecmp.cmp(fileName, tempfile):
            print 'uncrustify check: clean.'
            os.remove(tempfile)
        else:
            print 'uncrustify check: dirty - please uncrustify:'
            print 'to replace the file run:\n mv ', tempfile, fileName,'\n'
    else:
        print "please install uncrustify on your system"

##
# @brief checks if the cpp file passes the tests of cpplint
#
# @param fileName the filename
#
# @return True if the file is OK
def checkLint(fileName):
    filePath, fileExtension = os.path.splitext(fileName)
    cpplintExecutable = pathToGitRoot + '/doc/cpplint.py'
    cpplintCmd = [cpplintExecutable,'--filter=-readability/streams,-whitespace/line_length,-build/class', fileName]
    out, err = subprocess.Popen(cpplintCmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
    numberOfErrors = int(re.findall(r'Total errors found: (\d+)',err)[0])
    if numberOfErrors == 0:
        print 'cpplint.py check: clean.'
        return True
    else:
        print 'cpplint.py check: dirty - please delint:'
        print err
    return False


##
# @brief get the changed files which should be committed to git
#
# @return a list of all changed file
def getChangedFiles():
    out, err = subprocess.Popen(['git', 'diff', '--cached', '--name-only'], stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
    return out.splitlines()



if __name__ == '__main__':
    errorCode = 0
    for f in getChangedFiles():
        f = pathToGitRoot + '/'+ f
        if isCPPFile(f):
            print "checking file", f
            if not checkLint(f):
                errorCode = 1
                checkFormat(f)
        else:
            print "skipped file", f , 'as it seems to be not cpp file'
        print '----------------------------------------------------------'
    exit(errorCode)

