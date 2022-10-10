.686
.model flat, c

ExitProcess proto stdcall :dword
printf proto c :dword, :vararg
rand proto c
map proto c :dword, :dword, :dword

.data
format_int db "%d ", 0
formant_nl db 0dh, 0ah, 0
buf dd 100 dup(0)

.code
; int fun_div2(int)
fun_div2 proc
	push ebp
	mov ebp, esp

	mov eax, dword ptr [ebp+8]
	cdq
	mov edi, 2
	div edi

	mov esp, ebp
	pop ebp
	ret
fun_div2 endp

main proc
	push ebp
	mov ebp, esp
	sub esp, 4

	and dword ptr [ebp-4], 0

_main_L1:
	cmp dword ptr [ebp-4], 100
	je _main_L2

	invoke rand
	
	cdq
	mov edi, 100
	idiv edi
	mov eax, edx

	mov edx, offset buf
	mov edi, dword ptr [ebp-4]
	mov dword ptr [edx+4*edi], eax

	invoke printf, offset format_int, eax

	inc dword ptr [ebp-4]
	jmp _main_L1

_main_L2:
	invoke printf, offset formant_nl

	push fun_div2
	push 100
	push offset buf
	call map
	add esp, 12

	and dword ptr [ebp-4], 0

_main_L3:
	cmp dword ptr [ebp-4], 100
	je _main_L4

	mov edx, offset buf
	mov edi, dword ptr [ebp-4]
	mov eax, dword ptr [edx+4*edi]
	invoke printf, offset format_int, eax

	inc dword ptr [ebp-4]
	jmp _main_L3

_main_L4:
	invoke printf, offset formant_nl

	xor eax, eax
	mov esp, ebp
	pop ebp
	ret
main endp

end
; vim:ft=masm
