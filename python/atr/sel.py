"""
Attributes for the Selected Files. 

"""

__author__ = "Xin Shi <Xin.Shi@cern.ch>"
__copyright__ = "Copyright (c) Xin Shi"


import os
import sys
import atr 


def procdir(label):
    if 'run2012v0' in label: 
        procdir = os.path.join(
            atr.afbpath,
            'rel/CMSSW_5_3_9_patch3/src/BphAna/BToKstarMuMu_run2012v0/plugins')
    else:
        raise NameError(label)
    return procdir
    
def bashdir(label):
    if 'run2012v0' in label: 
        procdir = os.path.join(
            atr.afbpath,
            'rel/CMSSW_5_3_9_patch3/src/BphAna/BToKstarMuMu_run2012v0/bash')
    else:
        raise NameError(label)
    return procdir
    


