"""
Attributes for the Cfg files. 

"""

__author__ = "Xin Shi <Xin.Shi@cern.ch>"
__copyright__ = "Copyright (c) Xin Shi"

import os
import sys 
from tls.filetools import CfgFile 
from tls import * 
import atr
import cfg 


def crab_cfg(label):
    com_name = get_name_from_label(label)

    f = CfgFile()
    f.add_section('CRAB')
    f.set('CRAB', 'jobtype', 'cmssw')
    f.set('CRAB', 'scheduler', 'remoteGlidein')
    f.set('CRAB', 'use_server', 0)

    f.add_section('CMSSW')
    f.set('CMSSW', 'datasetpath', atr.ntp.dataset(label))
    f.set('CMSSW', 'pset', atr.ntp.driver(label))

    f.set('CMSSW', 'total_number_of_events', 1000)
    f.set('CMSSW', 'number_of_jobs', 10)
    f.set('CMSSW', 'output_file', 'BToKstarMuMu.root')
    
    f.add_section('USER')
    f.set('USER', 'email', 'Xin.Shi@cern.ch')
    f.set('USER', 'return_data', 0)
    f.set('USER', 'copy_data', 1)
    f.set('USER', 'ui_working_dir', 'crab_%s' %com_name)
    f.set('USER', 'storage_element', 'T2_CH_CERN')
    f.set('USER', 'user_remote_dir', com_name)
    f.set('USER', 'publish_data', 1)
    f.set('USER', 'publish_data_name', com_name)
    f.set('USER', 'dbs_url_for_publication',
          'https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_02_writer/servlet/DBSServlet')
    
    f.add_section('GRID')
    f.set('GRID', 'virtual_organization', 'cms')

    cfg_name = 'crab_%s.cfg' % com_name
    cfg_path = atr.ntp.procdir(label)
    cfg_file = os.path.join(cfg_path, cfg_name)

    function = getattr(cfg, com_name)
    f = function(f, label)
    f.output(cfg_file)
    return cfg_file


def Run2012A_22Jan2013_v1_run2012v0(f, label):
    f.set('CMSSW', 'total_number_of_lumis', -1)
    f.set('CMSSW', 'lumis_per_job', 20)
    f.remove_option('CMSSW', 'total_number_of_events')
    f.remove_option('CMSSW', 'number_of_jobs')
    f.set('CMSSW', 'lumi_mask', '../data/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON_MuonPhys.txt')
    return f

def Run2012B_22Jan2013_v1_run2012v0(f, label):
    return Run2012A_22Jan2013_v1_run2012v0(f, label) 

def Run2012C_22Jan2013_v1_run2012v0(f, label):
    return Run2012A_22Jan2013_v1_run2012v0(f, label) 

def Run2012D_22Jan2013_v1_run2012v0(f, label):
    return Run2012A_22Jan2013_v1_run2012v0(f, label) 

