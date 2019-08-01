#!/usr/bin/python

import os

lines = [line for line in open("hehe.txt")]

for line in lines:
	i = 0
	for c in line:
		if (c != '_' and not (c >= '0' and c <= '9')):
			break
		i+=1
	
	cmd = "mv " +  line[0:i].strip() + line[i+5:].strip() + " lab2_" + line[0:i].strip() + line[i+5:].strip()
	print cmd
	os.system(cmd)
	continue
	
	index = line.find("_lab2_")
	num = line[0 : index + 1]
	value = line[index + 6 : ]
	nn = "lab2_" + num + value
	cmd = "mv 3_" + line.strip() + " " + nn
	#print cmd
	os.system(cmd)
