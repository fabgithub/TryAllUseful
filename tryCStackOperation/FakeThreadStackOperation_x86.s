
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

global FakeThread_GetRSP

global FakeThread_StartWithRsp
global FakeThread_YieldWithMainRsp
global FakeThread_ResumeWithThreadRsp
global FakeThread_GetItemPtr

; ------
global _FakeThread_GetRSP

global _FakeThread_StartWithRsp
global _FakeThread_YieldWithMainRsp
global _FakeThread_ResumeWithThreadRsp
global _FakeThread_GetItemPtr

_FakeThread_GetRSP:
FakeThread_GetRSP:
    mov eax, esp
    ret

; FakeThreadItem * FakeThread_GetItemPtr(long long llMainRsp);

_FakeThread_GetItemPtr:
FakeThread_GetItemPtr:
    push edx
    mov edx, esp
    mov esp, [edx + 8] ; llMainRsp 为目标栈
    sub esp, 4
    pop eax
    mov esp, edx
    pop edx
    ret

; 保存寄存器
_i_SaveRegister:
    pop ecx ; 先把返回地址取出
    push ebx
    push esi
    push edi
    push ebp
    push ecx ; 将返回地址压入
    ret

; 恢复寄存器
_i_RestoreRegister:
    pop ecx ; 保存返回地址
    pop ebp
    pop edi
    pop esi
    pop ebx
    push ecx ; 压入返回地址
    ret

; FakeThreadItem * FakeThread_StartWithRsp(long long llNewRsp, void (*pfunc) (), FakeThreadItem *pItem, long long *pSaveMainRsp )

_FakeThread_StartWithRsp:
FakeThread_StartWithRsp:
    mov edx, esp
    call _i_SaveRegister

    mov eax, [edx + 16]
    mov [eax], esp ; 第四个参数 保存main esp
    push dword [edx + 12]     ; 第三个参数 保存pItem
    push ebp

    mov eax, esp
    mov esp, [edx + 4] ; 第一个参数llNewRsp

    push eax ; 保存原先的栈
    push eax ; 保存原先的栈
    push eax ; 保存原先的栈
    push eax ; 保存原先的栈
    call [edx + 8] ; 第二个参数
    pop eax ; 取出原先的栈
    pop eax ; 取出原先的栈
    pop eax ; 取出原先的栈
    pop eax ; 取出原先的栈

    mov esp, eax

    pop ebp
    pop eax
    call _i_RestoreRegister
    ret

; FakeThreadResumeParam * FakeThread_YieldWithMainRsp(long long llMainRsp, long long *pThreadRsp);
_FakeThread_YieldWithMainRsp:
FakeThread_YieldWithMainRsp:
    mov edx, esp ; 保存当前栈，以便方问参数
    call _i_SaveRegister
    mov eax, [edx + 8]
    mov [eax], esp ; 第二个参数中保存当前栈位置
    push ebp

    mov esp, [edx + 4] ; 第一个参数是新栈
    sub esp, 8
    pop ebp
    pop eax
    call _i_RestoreRegister
    ret

; resume 函数
; void FakeThread_ResumeWithThreadRsp(long long llThreadRsp, long long *pSaveMainRsp, FakeThreadItem *pItem, FakeThreadResumeParam *pResumeParam)
_FakeThread_ResumeWithThreadRsp:
FakeThread_ResumeWithThreadRsp:
    mov edx, esp ; 这里保存下原先的栈位置，以便方问参数
    call _i_SaveRegister

    mov eax, [edx + 8]
    mov [eax], esp ; 第二个参数用于保存当前栈
    push dword [edx + 12] ; 第三个参数: 新的伪线程对象
    push ebp

    mov esp, [edx + 4] ; 第一个参数是新栈
    sub esp, 4
    pop ebp
    call _i_RestoreRegister
    mov eax, [edx + 16] ; 第四个参数是要返回的值
    ret
