.686
.model flat, c

ExitProcess proto stdcall :dword
printf proto c :dword, :vararg
rand_str proto c
inssort proto c :dword, :dword

.data
format_str db "%s", 0dh, 0ah, 0
format_nl db 0dh, 0ah, 0

.code
main proc
	push ebp
	mov ebp, esp
	sub esp, 44

	and dword ptr [ebp-4], 0

_main_L1:
	mov esi, dword ptr [ebp-4]
	cmp esi, 10
	je _main_L2

	invoke rand_str

	mov esi, dword ptr [ebp-4]
	lea edx, dword ptr [ebp-44]
	mov dword ptr [edx+4*esi], eax

	invoke printf, offset format_str, dword ptr [edx+4*esi]
	inc dword ptr [ebp-4]
	jmp _main_L1

_main_L2:
	invoke printf, offset format_nl

	lea eax, dword ptr [ebp-44]
	invoke inssort, eax, 10

	and dword ptr [ebp-4], 0

_main_L3:
	mov esi, dword ptr [ebp-4]
	cmp esi, 10
	je _main_L4

	lea edx, dword ptr [ebp-44]
	invoke printf, offset format_str, dword ptr [edx+4*esi]
	inc dword ptr [ebp-4]
	jmp _main_L3

_main_L4:
	xor eax, eax
	mov esp, ebp
	pop ebp
	ret
main endp

end
; vim:ft=masm
