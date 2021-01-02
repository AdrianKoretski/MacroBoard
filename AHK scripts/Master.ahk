#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

F13 & a::
Run VS_bookmark.ahk
return

F14 & a::
Run VS_prev_bookmark.ahk
return

F15 & a::
Run VS_next_bookmark.ahk
return

F16 & a::
Run VS_LogError.ahk
return

F17 & a::
Run Google.ahk
return

F18 & a::
Run VS_close_all_tabs.ahk
return

F19 & a::
Run VS_comment_selected.ahk
return

F20 & a::
Run VS_uncomment_selected.ahk
return

F13 & b::
Run VS_collapse_uncollapse.ahk
return

