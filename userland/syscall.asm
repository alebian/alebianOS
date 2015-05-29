GLOBAL syscall

syscall:
	push ebp
	mov ebp, esp

	push ebx
	push ecx
	push edx
	push esi
	push edi

	mov eax, [ebp + 8] ; Syscall number
	mov ebx, [ebp + 12]; Arg1
	mov ecx, [ebp + 16]; Arg2
	mov edx, [ebp + 20]; Arg3
	mov esi, [ebp + 24]; Arg4
	mov edi, [ebp + 28]; Arg5

	int 80h

	pop edi
	pop esi
	pop edx
	pop ecx
	pop ebx

	mov esp, ebp
	pop ebp
	ret