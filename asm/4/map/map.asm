.686
.model flat, c

.code
; void map(int *buf, int size, int (*fun_ptr)(int))
map proc
	push ebp
	mov ebp, esp
	sub esp, 4

	and dword ptr [ebp-4], 0

_map_L1:
	mov ecx, dword ptr [ebp+12]
	cmp dword ptr [ebp-4], ecx
	je _map_L2

	mov edx, dword ptr [ebp+8]
	mov edi, dword ptr [ebp-4]

	push dword ptr [edx+4*edi]
	call dword ptr [ebp+16]
	add sp, 4

	mov edx, dword ptr [ebp+8]
	mov edi, dword ptr [ebp-4]
	mov dword ptr [edx+4*edi], eax

	inc dword ptr [ebp-4]
	jmp _map_L1

_map_L2:
	mov esp, ebp
	pop ebp
	ret
map endp
end
; vim:ft=masm
