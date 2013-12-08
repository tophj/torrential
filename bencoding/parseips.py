
import os

def parseip():
	bashCommand = "ip route show > outputTorrential.txt"
	returnedlist = os.system(bashCommand) 

	#print returnedlist
	#print ""
	f = open('outputTorrential.txt','r')

	listofIP = ""
	for line in f:
		if line[0:2].isdigit() and line[0:3] != 169:

			i=0
			while(line[i] != " "):
				listofIP += line[i]
				i = i+1

			listofIP += " "


	print listofIP

			


	f.close()
	removeFileBashCommand = "rm outputTorrential.txt" 
	os.system(removeFileBashCommand)
		


parseip()