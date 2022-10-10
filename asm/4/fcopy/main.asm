.686
.model flat, c

ExitProcess proto stdcall :dword
printf proto c :dword, :vararg
fcopy proto c :dword, :dword

.data

.code
main proc
	push ebp
	mov ebp, esp

	mov ebx, dword ptr [ebp+12]
	invoke fcopy, dword ptr [ebx+4], dword ptr [ebx+8]
	
	xor eax, eax
	mov esp, ebp
	pop ebp
	ret
main endp
end
; vim:ft=masm
