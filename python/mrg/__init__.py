"""
Module for Merging ROOT files 

"""

__author__ = "Xin Shi <Xin.Shi@cern.ch>"
__copyright__ = "Copyright (c) Xin Shi"

import sys 
from tls import * 
import atr


def main(args):
    datatype = args[0]
    label = args[1]
    com_name = get_name_from_label(label)
    srcdir = atr.ntp.grid_path(label)
    eosbase = '/afs/cern.ch/user/x/xshi/eos/cms/store/user/xshi/'
    #dstdir = os.path.join(eosbase, 'dat/ntp/data', com_name)
    dstdir = os.path.join(atr.datpath, 'ntp/data', com_name)
    merge_root_files(srcdir, dstdir)

    


