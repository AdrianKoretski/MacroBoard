file := FileOpen("0Number.txt","r")
FileVar := file.Read()
file.close()

Number := FileVar , Number += 1  ; convert text to number

file := FileOpen("0Number.txt","w")
file.write(Number)
file.close()
