
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

global _FakeThread_GetRSP


global _FakeThread_StartWithRsp
global _FakeThread_YieldWithMainRsp
global _FakeThread_ResumeWithThreadRsp
global _FakeThread_GetItemPtr

global FakeThread_GetRSP
global FakeThread_GetItemPtr
global FakeThread_StartWithRsp
global FakeThread_YieldWithMainRsp
global FakeThread_ResumeWithThreadRsp

FakeThread_GetRSP:
_FakeThread_GetRSP:
    mov rax, rsp
    ret

FakeThread_GetItemPtr:
_FakeThread_GetItemPtr:
    push rdx
    mov rdx, rsp
    mov rsp, rdi
    sub rsp, 8
    pop rax
    mov rsp, rdx
    pop rdx
    ret

; 保存寄存器
_i_SaveRegister:
    pop rcx ; 先把返回地址取出
    push rbx
    push rbp

    push r12
    push r13
    push r14
    push r15

    push rcx ; 将返回地址压入
    ret

; 恢复寄存器
_i_RestoreRegister:
    pop rcx ; 保存返回地址

    pop r15
    pop r14
    pop r13
    pop r12

    pop rbp
    pop rbx
    push rcx ; 压入返回地址
    ret

FakeThread_StartWithRsp:
_FakeThread_StartWithRsp:
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

FakeThread_YieldWithMainRsp:
_FakeThread_YieldWithMainRsp:
    mov [rsi], rsp
    push rbp

    mov rsp, rdi
    sub rsp, 16
    pop rbp
    pop rax
    ret

; resume 函数
FakeThread_ResumeWithThreadRsp:
_FakeThread_ResumeWithThreadRsp:
    mov [rsi], rsp
    push rdx
    push rbp

    mov rsp, rdi
    sub rsp, 8
    pop rbp
    mov rax, rcx ; 保存返回值
    ret
