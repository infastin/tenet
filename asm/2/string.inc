; size_t strlen(char *str)
strlen proc
	push ebp
	mov ebp, esp
	
	mov edx, dword ptr [ebp+8]
	xor eax, eax

_strlen_loop:
	cmp byte ptr [edx], 0
	je _strlen_out
	inc eax
	inc edx
	jmp _strlen_loop

_strlen_out:
	mov esp, ebp
	pop ebp
	ret
strlen endp

; char *strchr(char *str, char c)
strchr proc
	push ebp
	mov ebp, esp

	mov ebx, dword ptr [ebp+8]
	mov dl, byte ptr [ebp+12]

_strchr_loop:
	cmp byte ptr [ebx], 0
	jne _strchr_loop_L1
	mov eax, 0
	jmp _strchr_out

_strchr_loop_L1:
	cmp byte ptr [ebx], dl
	jne _strchr_loop_L2
	mov eax, ebx
	jmp _strchr_out

_strchr_loop_L2:
	inc ebx
	jmp _strchr_loop

_strchr_out:
	mov esp, ebp
	pop ebp
	ret
strchr endp

; char *strcat(char *dst, char *src)
strcat proc
	push ebp
	mov ebp, esp

	mov ebx, dword ptr [ebp+8]
	mov eax, ebx

_strcat_find_0:
	cmp byte ptr [ebx], 0
	je _strcat_find_0_out
	inc ebx
	jmp _strcat_find_0

_strcat_find_0_out:
	mov dword ptr [ebp+8], ebx

_strcat_loop:
	mov ebx, dword ptr [ebp+12]
	mov dl, byte ptr [ebx]
	mov ebx, dword ptr [ebp+8]
	mov byte ptr [ebx], dl

	cmp dl, 0
	je _strcat_out

	inc dword ptr [ebp+12]
	inc dword ptr [ebp+8]
	jmp _strcat_loop

_strcat_out:
	mov esp, ebp
	pop ebp
	ret
strcat endp

; int strcmp(char *str1, char *str2)
strcmp proc
	push ebp
	mov ebp, esp

_strcmp_loop:
	mov ebx, dword ptr [ebp+8]
	mov cl, byte ptr [ebx]
	mov ebx, dword ptr [ebp+12]
	cmp cl, byte ptr [ebx]
	jle _strcmp_loop_L1
	
	mov eax, 1
	jmp _strcmp_out
	
_strcmp_loop_L1:
	je _strcmp_loop_L2
	mov eax, -1
	jmp _strcmp_out

_strcmp_loop_L2:
	inc dword ptr [ebp+8]
	inc dword ptr [ebp+12]
	jmp _strcmp_loop

_strcmp_out:
	mov esp, ebp
	pop ebp
	ret
strcmp endp

; char *strcpy(char *dst, char *src)
strcpy proc
	push ebp
	mov ebp, esp

	mov eax, dword ptr [ebp+8]

_strcpy_loop:
	mov ebx, dword ptr [ebp+12]
	mov cl, byte ptr [ebx]
	mov ebx, dword ptr [ebp+8]
	mov byte ptr [ebx], cl

	cmp cl, 0
	je _strcpy_out
	
	inc dword ptr [ebp+12]
	inc dword ptr [ebp+8]
	jmp _strcpy_loop

_strcpy_out:
	mov esp, ebp
	pop ebp
	ret
strcpy endp

; char *strstr(char *haystack, char *needle)
strstr proc
	push ebp
	mov ebp, esp

	mov ebx, dword ptr [ebp+12] ; needle
	cmp byte ptr [ebx], 0
	jne _strstr_loop
	
	mov eax, ebx
	jmp _strstr_out

_strstr_loop:
	mov ebx, dword ptr [ebp+8]
	cmp byte ptr [ebx], 0
	je _strstr_out_null

	xor esi, esi ; haystack index
	xor edi, edi ; needle index

_strstr_loop2:
	mov cl, byte ptr [ebx+esi]
	mov ebx, dword ptr [ebp+12]
	cmp cl, byte ptr [ebx+edi]
	jne _strstr_loop_again

	inc esi
	inc edi

	mov ebx, dword ptr [ebp+12]
	cmp byte ptr [ebx+edi], 0
	je _strstr_out_ptr

	mov ebx, dword ptr [ebp+8]
	cmp byte ptr [ebx+esi], 0
	jne _strstr_loop2
	jmp _strstr_out_null

_strstr_loop_again:
	inc dword ptr [ebp+8]
	mov ebx, dword ptr [ebp+8]
	cmp byte ptr [ebx], 0
	jne _strstr_loop

_strstr_out_null:
	mov eax, 0
	jmp _strstr_out

_strstr_out_ptr:
	mov eax, dword ptr [ebp+8]

_strstr_out:
	mov esp, ebp
	pop ebp
	ret
strstr endp
; vim:ft=masm
