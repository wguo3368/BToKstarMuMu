"""
Module for Checking Data files 

"""

__author__ = "Xin Shi <Xin.Shi@cern.ch>"
__copyright__ = "Copyright (c) Xin Shi"

import sys 
from tls import * 
import atr 

def main(args):
    datatype = args[0]
    label = args[1]
    jobs_created = atr.ntp.jobs_created(label)
    filepath = atr.ntp.grid_path(label)

    #cmd = 'cmsLs %s' %filepath
    cmd = 'ls %s' %filepath
    files = get_files_from_ls(cmd)
    goodfiles = get_goodfiles(label)

    jobs_finished = [] 
    for f in goodfiles:
        try: 
            job = f.split('_')[1]
        except IndexError:
            sys.stderr.write('Error in the goodfiles: \n')
            sys.stderr.write(f + '\n') 
            sys.exit()
        jobs_finished.append(int(job))

    jobs_missing = jobs_created.difference(jobs_finished)

    if '-stat' in args: 
        save_stat(filepath, files)
        return 

    sys.stdout.write('Number of missing files : %s \n' % len(jobs_missing))
    remove_duplicates(filepath, files, goodfiles, jobs_missing)
    
    if len(jobs_missing) == 0:
        sys.stdout.write('All jobs are done correctly. \n')

    else: 
        sys.stdout.write('Please check and resubmit these jobs: \n')
        crabcmd = 'crab -c crab_%s -resubmit %s' %(
        get_name_from_label(label), ','.join(str(s) for s in jobs_missing))
        sys.stdout.write(crabcmd + '\n')

def get_goodfiles(label):
    comname = get_name_from_label(label) 
    cmd = 'find_goodfiles.py -c crab_%s -q' %comname

    output = proc_cmd(cmd, procdir=atr.ntp.procdir(label))
    goodfiles = []
    for line in output.split():
        goodfiles.append(line.split('/')[-1])

    return goodfiles


def remove_duplicates(filepath, files, goodfiles, jobs_missing):
    duplicatefiles = set(files).difference(set(goodfiles))
    duplicated_jobs = get_job_number_from_files(duplicatefiles)
    
    sys.stdout.write('Number of files by cmsLs : %s \n' % len(files))
    sys.stdout.write('Number of goodfiles : %s \n' % len(goodfiles))
    sys.stdout.write('Number of duplicates : %s \n' % len(duplicatefiles))

    if len(duplicatefiles) == 0:
        return

    s = raw_input('Do you want to list the duplicates names? (yes or no) ')
    if s == 'yes': 
        for f in duplicatefiles:
            sys.stdout.write(' %s \n' %f )

    s = raw_input('Do you want to remove all of them? (yes or no) ')
    if s != 'yes':
        return

    sys.stdout.write('Removing duplicates: \n')
    for f in duplicatefiles:
        sys.stdout.write(' %s ...' %f )
        sys.stdout.flush()
        cmd = 'cmsRm %s/%s' %(filepath, f)
        proc_cmd(cmd)
        sys.stdout.write(' OK.\n')

    sys.stdout.write('Total of %s duplicated files removed. \n' %len(duplicatefiles))


def get_job_number_from_files(fs):
    jobs = []
    for f in fs:
        job = f.split('_')[1]
        jobs.append(int(job))
    return jobs


def get_root_file_names_from_py(filename):
    f = open(filename, 'r')
    fnames = []
    for line in f:
        if '.root' in line:
            fname = line.split('.root')[0]
            fname = fname.split('/')[-1] + '.root'
            fnames.append(fname)

    return fnames


def get_run_event_list(filename):
    f = open(filename, 'r')
    run_event_list = []
    line_num = 0
    
    for line in f:
        line_num += 1
        if line_num < 4:
            continue
        
        items=line.split('*')
        run = items[2].strip()
        event = items[3].strip()
        run_event = '%s,%s' %(run, event)
        run_event_list.append(run_event)

    return run_event_list

    
def output_list_to_file(lst, filename):
    f = open(filename, 'w')
    for li in lst:
        f.write(li + '\n')
    f.close()


