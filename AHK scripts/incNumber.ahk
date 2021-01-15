file := FileOpen("Number.txt","r")
FileVar := file.Read()
file.close()

Number := FileVar , Number += 1  ; convert text to number

file := FileOpen("Number.txt","w")
file.write(Number)
file.close()
