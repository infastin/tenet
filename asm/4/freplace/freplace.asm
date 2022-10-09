.686
.model flat, c

calloc proto c :dword, :dword
strlen proto c :dword
fopen proto c :dword, :dword
fread proto c :dword, :dword, :dword, :dword
fwrite proto c :dword, :dword, :dword, :dword
fclose proto c :dword
free proto c :dword

printf proto c :dword, :vararg

.data
_debug_str db "%s", 0dh, 0ah, 0
_debug_int db "%d", 0dh, 0ah, 0

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

	mov esp, ebp
	pop ebp
	ret
freplace endp
end
; vim:ft=masm
