#!/usr/bin/python -f

import subprocess

for i in range(1,11):
	print("------------------------------" + str(i) + "------------------------------")
	subprocess.call(["/opt/openmpi-1.4.3-gcc44/bin/mpirun", "-np", str(i), "test_dissbarrier"])
