.686
.model flat, c

fopen proto c :dword, :dword
fclose proto c :dword
fwrite proto c :dword, :dword, :dword, :dword
fread proto c :dword, :dword, :dword, :dword
fflush proto c :dword

.data
_fcopy_write db "w+", 0
_fcopy_read db "r", 0

.code
; void fcopy(const char *dst, const char *src)
fcopy proc
	push ebp
	mov ebp, esp
	sub esp, 528

	invoke fopen, dword ptr [ebp+8], offset _fcopy_write
	mov dword ptr [ebp-4], eax

	invoke fopen, dword ptr [ebp+12], offset _fcopy_read
	mov dword ptr [ebp-8], eax

	and dword ptr [ebp-12], 0
	lea eax, [ebp-528]
	mov dword ptr [ebp-16], eax

_fcopy_L1:
	invoke fread, dword ptr [ebp-16], 1, 1, dword ptr [ebp-8]
	mov dword ptr [ebp-12], eax
	cmp eax, 0
	je _fcopy_L2
	
	invoke fwrite, dword ptr [ebp-16], 1, dword ptr [ebp-12], dword ptr [ebp-4]
	jmp _fcopy_L1

_fcopy_L2:
	invoke fclose, dword ptr [ebp-8]
	invoke fclose, dword ptr [ebp-4]

	mov esp, ebp
	pop ebp
	ret
fcopy endp
end
; vim:ft=masm
