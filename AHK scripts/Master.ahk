#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

F13 & a::
Run VS_bookmark.ahk
return

F13 & b::
Run VS_prev_bookmark.ahk
return

F13 & c::
Run VS_next_bookmark.ahk
return

F13 & d::
Run VS_LogError.ahk
return

F13 & e::
Run Google.ahk
return
