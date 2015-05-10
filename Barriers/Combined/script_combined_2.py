#!/usr/bin/python -f

import subprocess

for i in range(2,9):
	for j in range(2,13):
		subprocess.call(["/opt/openmpi-1.4.3-gcc44/bin/mpirun", "-np", str(i), "test_combined", str(j)])
