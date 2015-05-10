#!/usr/bin/python

import sys
import getopt

#import numpy as np
#import matplotlib.pyplot as plt

import re
import shlex
import logging
import threading
import subprocess
import random
import time
import os

#import numpy as np
#import matplotlib.pyplot as plt
#from matplotlib.pyplot import savefig
from __builtin__ import str


logging.basicConfig(level=logging.DEBUG,
                    format='(%(threadName)-10s) %(message)s',
                    )

def getpath(argv):
    runcmd = ''
    data_elements = ''
    outputpath = ''
    inputpath = ''
    time_out = ''
    max_cpu = ''
    try:
        opts, args = getopt.getopt(argv,"hr:i:o:d:m:t:",["rcmd=","ipath=","opath=","data=", "maxcpu=", "timeout="])
    except getopt.GetoptError:
        print 'Regress.py -r <runcmd> -i <inputpath> -o <outputpath> -d <data_elements> -m <maxcpu> -t <timeout>'
        sys.exit(2)
    if len(opts) == 0:
        print 'ERROR: Regress.py -r <runcmd> -i <inputpath> -o <outputpath> -d <data_elements> -m <maxcpu> -t <timeout>'
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print 'Cache_Coherency.py -r <runcmd> -i <inputpath> -o <outputpath> -d <data_elements> -m <maxcpu> -t <timeout>'
            sys.exit()
        elif opt in ("-r", "--rcmd"):
            runcmd = arg
        elif opt in ("-i", "--ipath"):
            inputpath = arg
        elif opt in ("-o", "--opath"):
            outputpath = arg
        elif opt in ("-d", "--data"):
            data_elements = arg
        elif opt in ("-m", "--maxcpu"):
            max_cpu = arg
        elif opt in ("-t", "--timeout"):
            time_out = arg
    print 'Run command is :', runcmd
    print 'Input path is :', inputpath
    print 'Output path is :', outputpath
    print 'Data Elements : ', data_elements 
    print 'MAXCPU : ', max_cpu
    print 'Time out : ', time_out, '\n\n'
    
    return (runcmd,inputpath,outputpath,data_elements,max_cpu, time_out)

class run(object):
    def __init__(self,tname, rcmd,ipath,output):
        self.tname =tname
        #print self.tname
        self.rcmd =rcmd
        #print self.rcmd
        self.ipath = ipath
        #print self.ipath
        self.log = open(output,'w')
        self.process1= None
        
    def runprocess(self,timeout):
        def target():
            args = shlex.split(self.rcmd)
            logging.debug('****Starting****')
            self.process1 = subprocess.Popen(args,stdout=self.log,cwd=self.ipath)
            self.log.flush()
            self.log.close()
            self.process1.communicate()
            logging.debug('****Exiting****\n')
            
        thread = threading.Thread(name=self.tname,target=target)
        thread.start()  
        thread.join(timeout)
        if thread.is_alive():
            print '****Code: ',self.tname,'is Terminated ****'
            self.process1.terminate()
            thread.join()
            self.log.close()
            return(False)
        else:
            return(True)

if __name__ == "__main__":
    rcmd,ipath, opath, data_elm, max_procs, time_out = getpath(sys.argv[1:])
    
    tempcmd = "./" + rcmd

    timestr = time.strftime("%Y%m%d-%H%M%S")
    
    log_path1= opath + "/Test_array_lock_log_" + timestr
    
    os.mkdir(log_path1);
    
    
    result_list_weak = []
    for j in range(1,((int(max_procs))+1)):
        iterative_logpath1 = log_path1 + "/Data_" + str(int(data_elm)) + "_Proc_" + str(j)
        os.mkdir(iterative_logpath1)
        for k in range(1,101):
            r1cmd = tempcmd + " " + str(int(data_elm)) + " " + str(100) +  " " + str(j)
            temp_tname = "Data_" + str(int(data_elm)) + "_Proc_" + str(j)
            log_file = iterative_logpath1 + "/Iteration_" + str(k) + ".txt"
            runprocess = run(temp_tname,r1cmd,ipath,log_file)
            runwasok = runprocess.runprocess(int(time_out))
            
        if runwasok == True:
            avg_time_list = []
            for k in range(1,101):
                read_log = iterative_logpath1 + "/Iteration_" + str(k) + ".txt"
                fp = open(read_log)
                lines = fp.readlines()
                fp.close()
                for line in lines:
                    line = line.strip()
                    if "time" in line.lower():
                        words = line.split(" ")
                        avg_time_list.append(float(words[-1]))
      #                  print float(words[-1])
       #     print len(avg_time_list)
            result_list_weak.append((sum(avg_time_list)/len(avg_time_list)))
            
    print result_list_weak
        
        
            
            
    
    
