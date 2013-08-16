#!/usr/bin/env python
"""
Main script for A_FB analysis.

"""

__author__ = "Xin Shi <Xin.Shi@cern.ch>"
__copyright__ = "Copyright (c) Xin Shi"


import sys

def main():
    args = sys.argv[1:]
    if len(args) == 0 or '-h' in args:
        sys.stdout.write('No args provided!\n')
        sys.exit()
        
    module_name = args[0]
    module = __import__(module_name)
    module.main(args[1:])


if __name__ == '__main__':
    main()

