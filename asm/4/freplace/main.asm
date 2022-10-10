.686
.model flat, c

ExitProcess proto stdcall :dword
freplace proto c :dword, :dword, :dword, :dword

.data

.code
main proc
	push ebp
	mov ebp, esp

	mov ebx, dword ptr [ebp+12]
	invoke freplace, dword ptr [ebx+4], dword ptr [ebx+8], dword ptr [ebx+12], dword ptr [ebx+16]

	xor eax, eax
	mov esp, ebp
	pop ebp
	ret
main endp
end
; vim:ft=masm
