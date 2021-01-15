file := FileOpen("Number.txt","r")
FileVar := file.Read()
file.close()
send %FileVar%