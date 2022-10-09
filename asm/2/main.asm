.686
.model flat, c

includelib legacy_stdio_definitions.lib
includelib libcmt.lib

; kernel32.lib msvcrt.lib libcmt.lib
ExitProcess proto stdcall :dword
printf proto c :dword, :vararg

.data
buf db 512 dup(0)
format_str db "%s", 0dh, 0ah, 0

.code
include string.inc

main proc
	push ebp
	mov ebp, esp
	sub esp, 4

	mov edx, dword ptr [ebp+12]
	mov dword ptr [ebp-4], edx

	push dword ptr [edx+4]
	push offset buf
	call strcpy
	add esp, 8

	mov edx, dword ptr [ebp-4]
	
	push dword ptr [edx+8]
	push offset buf
	call strstr
	add esp, 8

	invoke printf, offset format_str, eax

	mov esp, ebp
	pop ebp
	ret
main endp

end
; vim:ft=masm
