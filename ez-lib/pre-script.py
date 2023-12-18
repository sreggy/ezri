#!/opt/homebrew/bin/python3
import os, sys
import shutil
import fileinput
import pysftp 
import argparse
from difflib import SequenceMatcher
from datetime import datetime

def extract(template, text):
    seq = SequenceMatcher(None, template, text, True)
    return [text[c:d] for tag, a, b, c, d in seq.get_opcodes() if tag == 'replace']



template = '% __EZRI_VERSION__ "%"'

if __name__ == "__main__":

    pathname = os.path.dirname(sys.argv[0])        
    os.chdir(pathname)

    parser = argparse.ArgumentParser(description='TheParser')

    # Add arguments to the parser
    parser.add_argument('-v', '--version', dest='ver', type=int, help='Increment version', default=0, nargs='?', action='store', const=1)
    parser.add_argument('-r', '--release', dest='rel', type=int, help='Increment release', default=0, nargs='?', action='store', const=1)
    parser.add_argument('-m', '--modification', dest='mod', type=int, help='Increment modification', default=0, nargs='?', action='store', const=1)
    parser.add_argument('-i', '--initialize', dest='init', help='Reset counter', action='store_true')
    parser.add_argument('-f', '--file', dest='file', type=str, help='File name', default="./include/ezri-version.h")


    # Run method to parse the arguments
    args = parser.parse_args()

    # with open(args.file) as f:
    #     lines = f.readlines()

    for li in fileinput.input(args.file, inplace=1):
        if ("__EZRI_VERSION__" in li):
            dummy, version = extract(template, li)
#            print (vInc)
            if (args.init):
                mod = args.mod
                rel = args.rel
                ver = args.ver
            else:
                mod = int(version[4:])+args.mod
                rel = int(version[2:4])+args.rel
                ver = int(version[:2])+args.ver

            li = "#define __EZRI_VERSION__ \"{:02}{:02}{:03}\"\n".format(ver, rel, mod)

        if ("__EZRI_STAMP__" in li):
#            dummy, stamp = extract(template, li)
#            print (vInc)
            date_time = datetime.now()
            str_date_time = date_time.strftime("%Y%m%d-%H%M%S")

            li = "#define __EZRI_STAMP__ \"" + str_date_time + "\"\n"


        
        sys.stdout.write (li)

