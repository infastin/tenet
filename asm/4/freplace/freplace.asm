.686
.model flat, c

calloc proto c :dword, :dword
strlen proto c :dword
fopen proto c :dword, :dword
fread proto c :dword, :dword, :dword, :dword
fwrite proto c :dword, :dword, :dword, :dword
fputc proto c :byte, :dword
fclose proto c :dword
free proto c :dword

printf proto c :dword, :vararg

.data
_debug_str db "%s", 0dh, 0ah, 0
_debug_int db "%d", 0dh, 0ah, 0
_freplace_read db "r", 0
_freplace_write db "w+", 0

.code
; int *preprocess(const char *needle, int len)
preprocess proc
	push ebp
	mov ebp, esp
	sub esp, 12

	mov eax, dword ptr [ebp+12]
	add eax, 1
	invoke calloc, eax, 4
	mov dword ptr [ebp-4], eax
	mov dword ptr [eax], -1

	mov dword ptr [ebp-8], 1
	mov dword ptr [ebp-12], 0

_preprocess_L1:
	mov esi, dword ptr [ebp-8]
	cmp esi, dword ptr [ebp+12]
	je _preprocess_L2

	mov edx, dword ptr [ebp+8]
	mov edi, dword ptr [ebp-12]
	mov al, byte ptr [edx+esi]
	cmp al, byte ptr [edx+edi]
	jne _preprocess_L3

	mov edx, dword ptr [ebp-4]
	mov eax, dword ptr [edx+4*edi]
	mov dword ptr [edx+4*esi], eax
	jmp _preprocess_L6

_preprocess_L3:
	mov edx, dword ptr [ebp-4]
	mov dword ptr [edx+4*esi], edi
	
_preprocess_L4:
	cmp edi, -1
	je _preprocess_L5

	mov edx, dword ptr [ebp+8]
	mov al, byte ptr [edx+esi]
	cmp al, byte ptr [edx+edi]
	je _preprocess_L5

	mov edx, dword ptr [ebp-4]
	mov edi, dword ptr [edx+4*edi]
	jmp _preprocess_L4

_preprocess_L5:
	mov dword ptr [ebp-12], edi

_preprocess_L6:
	inc dword ptr [ebp-8]
	inc dword ptr [ebp-12]
	jmp _preprocess_L1

_preprocess_L2:
	mov edx, dword ptr [ebp-4]
	mov edi, dword ptr [ebp-12]
	mov dword ptr [edx+4*esi], edi

	mov eax, dword ptr [ebp-4]

	mov esp, ebp
	pop ebp
	ret
preprocess endp

; void freplace(const char *dst, const char *src, const char *needle, const char *replace)
freplace proc
	push ebp
	mov ebp, esp
	sub esp, 548

	invoke strlen, dword ptr [ebp+16]
	mov dword ptr [ebp-4], eax

	invoke strlen, dword ptr [ebp+20]
	mov dword ptr [ebp-8], eax

	push dword ptr [ebp-4]
	push dword ptr [ebp+16]
	call preprocess
	add esp, 8
	mov dword ptr [ebp-12], eax

	invoke fopen, dword ptr [ebp+8], offset _freplace_write
	mov dword ptr [ebp-16], eax

	invoke fopen, dword ptr [ebp+12], offset _freplace_read
	mov dword ptr [ebp-20], eax

	mov dword ptr [ebp-24], 512
	mov dword ptr [ebp-28], 512
	and dword ptr [ebp-32], 0

_freplace_L1:
	mov esi, dword ptr [ebp-28]
	cmp esi, dword ptr [ebp-24]
	jne _freplace_L3
	
	lea eax, [ebp-544]
	invoke fread, eax, 1, 512, dword ptr [ebp-20]
	mov dword ptr [ebp-24], eax
	and dword ptr [ebp-28], 0
	xor esi, esi

_freplace_L3:
	mov edi, dword ptr [ebp-32]
	mov edx, dword ptr [ebp+16]
	cmp byte ptr [edx+edi], 0
	jne _freplace_L4

	invoke fwrite, dword ptr [ebp+20], 1, dword ptr [ebp-8], dword ptr [ebp-16]
	
	cmp dword ptr [ebp-24], 0
	je _freplace_L2

	lea edx, [ebp-544]
	mov esi, dword ptr [ebp-28]
	cmp byte ptr [edx+esi], 0
	je _freplace_L2

	and dword ptr [ebp-32], 0
	jmp _freplace_L1

_freplace_L4:
	cmp dword ptr [ebp-24], 0
	je _freplace_L5
	jmp _freplace_L6

	lea edx, [ebp-544]
	mov esi, dword ptr [ebp-28]
	cmp byte ptr [edx+esi], 0
	jne _freplace_L6

_freplace_L5:
	invoke fwrite, dword ptr [ebp+16], 1, dword ptr [ebp-32], dword ptr [ebp-16]
	jmp _freplace_L2

_freplace_L6:
	lea edx, [ebp-544]
	mov esi, dword ptr [ebp-28]
	mov al, byte ptr [edx+esi]
	mov edx, dword ptr [ebp+16]
	mov edi, dword ptr [ebp-32]
	cmp al, byte ptr [edx+edi]
	jne _freplace_L7

	inc dword ptr [ebp-28]
	inc dword ptr [ebp-32]
	jmp _freplace_L1

_freplace_L7:
	mov ebx, dword ptr [ebp-12]
	mov edi, dword ptr [ebp-32]
	mov ecx, dword ptr [ebx+4*edi]
	cmp ecx, -1
	jne _freplace_L8

	invoke fwrite, dword ptr [ebp+16], 1, dword ptr [ebp-32], dword ptr [ebp-16]

	lea edx, [ebp-544]
	mov esi, dword ptr [ebp-28]
	invoke fputc, byte ptr [edx+esi], dword ptr [ebp-16]

	inc dword ptr [ebp-28]
	and dword ptr [ebp-32], 0
	jmp _freplace_L1

_freplace_L8:
	mov dword ptr [ebp-548], ecx
	mov ecx, dword ptr [ebp-32]
	sub ecx, dword ptr [ebp-548]
	invoke fwrite, dword ptr [ebp+16], 1, ecx, dword ptr [ebp-16]
	mov ecx, dword ptr [ebp-548]
	mov dword ptr [ebp-32], ecx
	jmp _freplace_L1

_freplace_L2:
	invoke fclose, dword ptr [ebp-20]
	invoke fclose, dword ptr [ebp-16]
	invoke free, dword ptr [ebp-12]

	mov esp, ebp
	pop ebp
	ret
freplace endp
end
; vim:ft=masm
