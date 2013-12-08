
import os
import sys

def parseip():
	bashCommand = "ip route show > outputTorrential.txt"
	returnedlist = os.system(bashCommand) 

	try:
		i = sys.argv[1]
		f = open('outputTorrential.txt','r')

		listofIP = ""
		for line in f:
			if line[0:2].isdigit() and line[0:3] != '169':

				i=0
				while(line[i] != " "):
					listofIP += line[i]
					i = i+1

				listofIP += " "

		f.close()
		removeFileBashCommand = "rm outputTorrential.txt" 
		os.system(removeFileBashCommand)

		runProgram = "./bt_client " + sys.argv[1] + " " + listofIP

		print runProgram

		#os.system(runProgram)

	except IndexError: 
		print ""
                print "Need to add a torrent file."
                print "*"
                print "* Example usage: python parseips.py piratebay.torrent"
                print "*"
                print ""





parseip()
