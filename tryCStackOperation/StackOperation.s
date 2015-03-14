
section .data
    top db 0x12,0x34,0x56,0x78,
        dq 0x2121212121212121
        dq 0x1111111111111111
        dq 0x2222222222222222
        dq 0x3333333333333333
    stack        db "this is the stack for try something, ok"
        db "this is the stack for try something, ok"
        db "this is the stack for try something, ok"
        db "this is the stack for try something, ok"
        db "this is the stack for try something, ok"
        db "this is the stack for try something, ok"
        db "this is the stack for try something, ok"
        db "this is the stack for try something, ok"
        db "this is the stack for try something, ok"

section .text

global _GetCurStack
global _GetSS
global _GetRSP
global _ReturnParam
global _GetAtRsp

global _StartWithRsp
global _YieldWithMainRsp
global _ResumeWithThreadRsp
global _GetItemPtr

global _TryUseMemForStack
global _TryPointerToLongLong

extern _SomeFuncInFakeThread

func:
    mov rax, 12345678
    ret

_GetCurStack:

    mov rdx, ss
    mov rcx, rsp

    add rsp, 48
    sub rsp, ( + 7 * 1024 * 1024)
    mov rbx, 0x99999
    push rbx
    pop rax

    mov ss, rdx
    mov rsp, rcx

    mov rax, rsp
    mov rax, ss
    ret

_GetSS :
    mov rax, ss
    ret

_ReturnParam:
    mov rax, rdi
    ret

_GetAtRsp:
    mov rbx, rsp
    mov rsp, rdi
    mov rax, [rsp]
    mov rsp, rbx
    ret


_GetRSP:
    mov rax, rsp
    ret

_GetItemPtr:
    push rdx
    mov rdx, rsp
    mov rsp, rdi
    sub rsp, 8
    pop rax
    mov rsp, rdx
    pop rdx
    ret

_StartWithRsp:
    mov [rcx], rsp ; 保存main rsp
    push rdx       ; 保存pItem
    push rbp

    mov rax, rsp
    mov rsp, rdi
    push rax
    push rbp

    call rsi

    pop rbp
    pop rax
    mov rsp, rax

    pop rbp
    pop rax
    ret

_YieldWithMainRsp:
    mov [rsi], rsp
    push rbp

    mov rsp, rdi
    sub rsp, 16
    pop rbp
    pop rax
    ret

_ResumeWithThreadRsp:
    mov [rsi], rsp
    push rdx
    push rbp

    mov rsp, rdi
    sub rsp, 8
    pop rbp
    ret

_TryUseMemForStack:
    push rbx
    mov rax, rsp
    mov rsp, rdi
    mov rbx, 0x87654321
    push rbx
    mov rbx, 0x6162636465666768 ; 'abcdefghi'
    push rbx
    push rbx
    push rbx
    push rbx
    mov rsp, rax
    pop rbx
    ret

_TryPointerToLongLong:
    mov rax, rdi
    ret
