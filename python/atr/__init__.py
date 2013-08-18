"""
Attributes for the A_FB analysis.

"""

__author__ = "Xin Shi <Xin.Shi@cern.ch>"
__copyright__ = "Copyright (c) Xin Shi"

import os
import sys

import ntp
import sel 
import cfg

afbpath = os.environ['afb']
datpath = os.path.join(afbpath, 'dat')
figpath = os.path.join(afbpath, 'doc/fig')
logpath = os.path.join(afbpath, 'log')

rafbpath = os.environ['rafb']
rfigpath = os.path.join(rafbpath, 'doc', 'fig')

def get_afb_from_label(label):
    if 'run2012v0' in label:
        afb = 1
    else: 
        raise NameError(label)
    return afb 

    
