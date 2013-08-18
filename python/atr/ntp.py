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
    if '22Jan2013-v1' in label:
        driver = 'btokstarmumu_Run2012.py' 
    else:
        raise NameError(label)
    return driver 


def dataset(label):
    if  'Run2012A-22Jan2013-v1' in label:
        dataset = '/DoubleMuParked/Run2012A-22Jan2013-v1/AOD'
    elif 'Run2012B-22Jan2013-v1' in label:
        dataset = '/DoubleMuParked/Run2012B-22Jan2013-v1/AOD'
    elif 'Run2012C-22Jan2013-v1' in label:
        dataset = '/DoubleMuParked/Run2012C-22Jan2013-v1/AOD'
    elif 'Run2012D-22Jan2013-v1' in label:
        dataset = '/DoubleMuParked/Run2012D-22Jan2013-v1/AOD'
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
    if '22Jan2013-v1_run2012v0' in label: 
        srcdir = os.path.join(
            eosbase, 'DoubleMuParked',
            com_name, '2216baf4bbf15d7684a97e8092188a42')
    else:
        raise NameError(label)
    return srcdir 

def jobs_created(label): 
    if 'Run2012A-22Jan2013-v1_run2012v0' in label: 
        njobs = 243
    elif 'Run2012B-22Jan2013-v1_run2012v0' in label:
        njobs = 1647
    elif 'Run2012C-22Jan2013-v1_run2012v0' in label:
        njobs = 2429
    elif 'Run2012D-22Jan2013-v1_run2012v0' in label:
        njobs = 2353
    else:
        raise NameError(label)

    jobs_created = set(range(1, njobs+1))
    return jobs_created 


def ntp_labels(datatype, label):
    if datatype == 'data' and 'run2012v0' in label: 
        ntp_labels = [
            'Run2012A_22Jan2013_v1_run2012v0',
            'Run2012B_22Jan2013_v1_run2012v0',
            'Run2012C_22Jan2013_v1_run2012v0',
            'Run2012D_22Jan2013_v1_run2012v0',
        ]
    else:
        raise NameError(label)
    return ntp_labels

num_rootfiles = {
    'Run2012A_22Jan2013_v1_run2012v0': 1, 
    'Run2012B_22Jan2013_v1_run2012v0': 4, 
    'Run2012C_22Jan2013_v1_run2012v0': 5, 
    'Run2012D_22Jan2013_v1_run2012v0': 5, 
}
