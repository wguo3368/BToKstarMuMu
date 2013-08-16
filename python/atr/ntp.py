"""
Attributes for the NTuples.

"""

__author__ = "Xin Shi <Xin.Shi@cern.ch>"
__copyright__ = "Copyright (c) Xin Shi"


import os
import sys
from tls import *
import atr 
#import ntp 

def procdir(label):
    if 'run2012v0' in label: 
        procdir = os.path.join(
            atr.afbpath,
            'rel/CMSSW_5_3_9_patch3/src/BphAna/BToKstarMuMu_run2012v0/python')
    else:
        raise NameError(label)
    
    return procdir
    

def driver(label):
    if 'Run2012A-22Jan2013-v1' in label:
        driver =  'btokstarmumu_Run2012.py' 
    else:
        raise NameError(label)
    
    return driver 


def dataset(label):
    if  'Run2012A-22Jan2013-v1' in label:
        dataset = '/DoubleMuParked/Run2012A-22Jan2013-v1/AOD'
    elif 'Run2012B-22Jan2013-v1' in label:
        dataset = '/DoubleMuParked/Run2012B-22Jan2013-v1/AOD'
    else:
        raise NameError(label)
    return dataset


def rootpath(datatype, label, test=False):
    eosbase = 'root://eoscms//eos/cms/store/user/xshi/'
    if datatype == 'data': 
        ntp_label = get_label_by_version(label, 'x')
        inpath = os.path.join(eosbase, 'dat/ntp/data', ntp_label)
    else:
        inpath = os.path.join(eosbase, 'dat/ntp/mc', label)

    return inpath 


def rootname(datatype, label, batch=False):
    if datatype == 'mc': 
        name = 'BToKstarMuMu_merged_1'
    else:
         name = 'BToKstarMuMu_merged_*'

    if batch :
        name = 'BToKstarMuMu_merged_${LSB_JOBINDEX}'
    return name

   
        
def grid_path(label):
    com_name = get_name_from_label(label)
    eosbase = '/afs/cern.ch/user/x/xshi/eos/cms/store/user/xshi/'
    if label in ['Run2011A-PromptReco-v4_run2011v0_1', 
                 ] : 
        srcdir = os.path.join(
            eosbase, 'MuOnia',
            com_name, '09dd54ed3307c6d768a6853667b85e6a')
    else:
        raise NameError(label)
    return srcdir 
