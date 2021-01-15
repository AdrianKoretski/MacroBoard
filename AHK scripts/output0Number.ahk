file := FileOpen("0Number.txt","r")
FileVar := file.Read()
file.close()
send 0%FileVar%