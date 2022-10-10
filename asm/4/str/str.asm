.686
.model flat, c

malloc proto c :dword
rand proto c
strcmp proto c :dword, :dword

.code
; char *rand_str()
rand_str proc
	push ebp
	mov ebp, esp
	sub esp, 8

	invoke malloc, 32
	mov dword ptr [ebp-4], eax
	mov dword ptr [ebp-8], 0

_rand_str_L1:
	cmp dword ptr [ebp-8], 32
	je _rand_str_L2

	invoke rand
	
	cdq
	mov ecx, 95
	div ecx
	mov eax, edx
	add eax, 32
	mov edx, dword ptr [ebp-4]
	mov esi, dword ptr [ebp-8]
	mov dword ptr [edx+esi], eax

	inc dword ptr [ebp-8]
	jmp _rand_str_L1

_rand_str_L2:
	mov eax, dword ptr [ebp-4]

	mov esp, ebp
	pop ebp
	ret
rand_str endp

; void inssort(char **array, int len)
inssort proc
	push ebp
	mov ebp, esp
	sub esp, 12

	mov dword ptr [ebp-4], 1

_inssort_L1:
	mov esi, dword ptr [ebp-4]
	cmp esi, dword ptr [ebp+12]
	je _inssort_L2

	mov dword ptr [ebp-12], esi
	mov edi, esi
	sub edi, 1
	mov dword ptr [ebp-8], edi

_inssort_L3:
	mov edi, dword ptr [ebp-8]
	mov esi, dword ptr [ebp-12]
	mov edx, dword ptr [ebp+8]
	invoke strcmp, dword ptr [edx+4*edi], dword ptr [edx+4*esi]
	cmp eax, 0
	jle _inssort_L4

	mov esi, dword ptr [ebp-4]
	mov edi, dword ptr [ebp-8]
	mov edx, dword ptr [ebp+8]
	mov eax, dword ptr [edx+4*esi]
	xchg eax, dword ptr [edx+4*edi]
	mov dword ptr [edx+4*esi], eax

	mov dword ptr [ebp-12], edi
	cmp edi, 0
	je _inssort_L4

	dec dword ptr [ebp-8]
	jmp _inssort_L3

_inssort_L4:
	inc dword ptr [ebp-4]
	jmp _inssort_L1

_inssort_L2:
	mov esp, ebp
	pop ebp
	ret
inssort endp
end
; vim:ft=masm
