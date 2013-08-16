"""
Module for producing NTuple (Before Selection) 

"""

__author__ = "Xin Shi <Xin.Shi@cern.ch>"
__copyright__ = "Copyright (c) Xin Shi"

import sys
import atr 
from tls import *


def main(args):
    datatype = args[0]
    label = args[1]
    grid = option_exists(args, '-grid')

    proc_one_job(datatype, label, grid=grid)
        
def proc_one_job(datatype, label, grid=False):
    if grid:
        crab_cfg = atr.cfg.crab_cfg(label)
        cfg_path, cfg_name = os.path.split(crab_cfg)
        sys.stdout.write('Please create grid jobs with command: \n\n')

        sys.stdout.write('crab -cfg %s -create\n\n' %cfg_name)
        return 
    
    procdir = atr.ntp.procdir(label)
    cmd = 'cmsRun %s' % atr.ntp.driver(label)
    proc_cmd(cmd, test=True, procdir=procdir)
    sys.stdout.write('Please test with the above command.\n')
    
        
