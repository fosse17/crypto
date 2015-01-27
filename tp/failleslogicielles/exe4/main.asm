.386
.model flat,stdcall
extrn   MessageBoxA:PROC
extrn   CreateWindowExA:PROC
extrn   DefWindowProcA:PROC
extrn   RegisterClassA:PROC
extrn   ShowWindow:PROC
extrn   LoadIconA:PROC
extrn   LoadCursorA:PROC
extrn   SetCursor:PROC
extrn   GetMessageA:PROC
extrn   TranslateMessage:PROC
extrn   DispatchMessageA:PROC
extrn   GetModuleHandleA:PROC
extrn   GetDC:PROC
extrn   ExitProcess:PROC
extrn   LocalAlloc:PROC
extrn   StretchDIBits:PROC
extrn   GetWindowTextA:PROC
extrn   SetWindowTextA:PROC
extrn   MessageBoxA:PROC
extrn   lstrcmp:PROC
extrn   IsDebuggerPresent:PROC


.data

MSGSTRUCT struc
    msHWND          dd    ?
    msMESSAGE       dd    ?
    msWPARAM        dd    ?
    msLPARAM        dd    ?
    msTIME          dd    ?
    msPT            dd    2 dup(?)
MSGSTRUCT ends


LARGEUR                 EQU 300
HAUTEUR                 EQU 100
STARTX                  EQU 10
STARTY                  EQU 10
FREQUENCE               EQU 100

WS_VISIBLE              EQU 010000000h
WS_PUSHBUTTON           EQU 0
WS_TOPMOST              EQU 000000008h
WS_BORDER               EQU 000800000h
WS_POPUP                EQU 080000000h
WS_CAPTION              EQU 000C00000h
WS_CHILD                EQU 040000000h
WS_MAXIMISEBOX          EQU 000010000h
WS_MINIMISEBOX          EQU 000020000h
WS_CLOSEBOX             EQU 000080000h
WS_OVERLAPPED           EQU 000000000h
WS_SYSMENU              EQU 000800000h
WM_CREATE               EQU 000000001h
WM_DESTROY              EQU 000000002h
WM_COMMAND              EQU 000000273
WM_PAINT                EQU 00000000Fh
WM_TIMER                EQU 000000113h
WM_COMMAND              EQU 000000111h
WM_KEYDOWN              EQU 000000100h
WM_LMOUSECLICK          EQU 000000201h
WM_LBUTTONDOWN          EQU 000000201h
WM_LMOUSEDBLCLICK       EQU 000000203h
WM_SHOWWINDOW           EQU 000000018h
WM_SYSCOMMAND           EQU 000000112h
WM_MOUSEMOVE            EQU 000000200h
VK_PRIOR                EQU 000000021h
VK_NEXT                 EQU 000000022h
VK_END                  EQU 000000023h
VK_HOME                 EQU 000000024h
VK_LEFT                 EQU 000000025h
VK_UP                   EQU 000000026h
VK_RIGHT                EQU 000000027h
VK_DOWN                 EQU 000000028h
VK_ESCAPE               EQU 00000001Bh
SW_SHOW                 EQU 000000005h
SW_HIDE                 EQU 000000000h
SC_RESTORE              EQU 00000F120h
SC_MINIMIZE             EQU 00000F020h
SC_MAXIMIZE             EQU 00000F030h
CS_NORMAL               EQU 32512
CS_NS                   EQU 32645

msg_err         db 'erreur',0
nom_fenetre     db 'Exemple 4',0
type_bouton     db 'button',0
type_edit       db 'edit',0
bouton_cap      db 'Check serial',0

message_ok      db 'Bienvenue dans MonLogiciel 4.0 !',0
message_ko      db 'Mauvais mot de passe',0
mdbg            db 'Debugger détecté !',0

Class:
        style           dd ?
        wndproc         dd ?
        classextra      dd ?
        wndextra        dd ?
        instance        dd ?
        icon            dd ?
        cursor          dd ?
        background      dd ?
        menuname        dd ?
        classname       dd ?

msg             MSGSTRUCT ?

hwnd            dd ?
bouton          dd ?
input           dd ?
output          dd ?
buf             db 30 dup (?)
buf2            db 200 dup (?)
.code

start:
        mov wndproc,offset WndProc
        call GetModuleHandleA,0
        mov instance,eax
        mov classname, offset nom_fenetre
        call LoadIconA, 0, 32512
        mov icon,eax
        call LoadCursorA,0, 0
        mov cursor,eax
        call SetCursor, eax
        mov background,1

        call RegisterClassA, offset Class       ;enregistre la classe
        test eax,eax
        jz erreur

;============================ CREATE WINDOWS ================================

;Creation de la fenetre

        xor ebx,ebx
        push ebx                        ;lpvParam
        push instance                   ;Hinstance
        push ebx                        ;Hmenu
        push ebx                        ;Hparentwindow
        push HAUTEUR                    ;Height
        push LARGEUR                    ;Width
        push STARTY                     ;y
        push STARTX                     ;x
        push WS_SYSMENU OR WS_OVERLAPPED OR WS_CLOSEBOX OR WS_MINIMISEBOX OR WS_CAPTION OR 40000h
        push offset nom_fenetre         ;WndName
        push offset nom_fenetre         ;ClassName
        push WS_TOPMOST                 ;dwExtStyle
        call CreateWindowExA
        mov hwnd,eax
        call ShowWindow, eax, 1

        call CreateWindowExA,0,offset type_bouton,offset bouton_cap,WS_VISIBLE OR WS_CHILD OR WS_PUSHBUTTON,80,50,150,20,hwnd,1,instance,0
        mov bouton,eax
        call ShowWindow,eax,1

        call CreateWindowExA,0,offset type_edit,0,WS_BORDER OR WS_VISIBLE OR WS_CHILD ,40,20,220,20,hwnd,2,instance,0
        mov input,eax
        call ShowWindow,eax,1

        call IsDebuggerPresent
        cmp eax,0
        jz ook
        call MessageBoxA,0,offset mdbg,offset nom_fenetre,0
        call ExitProcess,0
ook:

;================================ INITIALISATIONS ===========================


;================================ MESSAGE LOOP ==============================

MsgLoop:
        call    GetMessageA, offset msg, dword ptr 0, dword ptr 0, dword ptr 0
        test    eax,eax
        jz      erreur
        call    TranslateMessage, offset msg
        call    DispatchMessageA, offset msg
        jmp     MsgLoop


erreur:
        call MessageBoxA,hwnd,offset msg_err,offset msg_err,0
        call ExitProcess,0


;============================== WNDPROC =====================================

WndProc proc hWnd:dword, uMsg:dword, wParam:dword, lParam:dword

        mov eax, uMsg
        cmp eax,WM_DESTROY
        jz WndProc_exit
        cmp eax,WM_COMMAND
        jz WndProc_command

WndProc_default:
        call DefWindowProcA, hWnd, uMsg, wParam, lParam
WndProc_fin:
        ret
WndProc_exit:
        call ExitProcess,0

WndProc_command:
        mov eax,lParam
        cmp eax,bouton
        jnz WndProc_default
        lea esi,buf2
        call GetWindowTextA,input,esi,199
        
        mov esi,offset debut_crypt
        mov ecx,fin_crypt-debut_crypt
dcrypt1:
        mov al,[esi]
        xor al,54
        mov [esi],al
        inc esi
        dec ecx
        cmp ecx,0
        jnz dcrypt1

        mov edx,5
debut_crypt:
        lea esi,buf2
        lea edi,good_password
        call lstrcmp, esi,edi
        cmp eax,0
        jnz bad_boy
good_boy:
        call MessageBoxA,0,offset message_ok,offset nom_fenetre,0

        jmp fin_check
bad_boy:
        call MessageBoxA,0,offset message_ko,offset nom_fenetre,0
        jmp fin_check
good_password   db 'CestFini',0
		
fin_check:
fin_crypt:
        mov esi,offset debut_crypt
        mov ecx,fin_crypt-debut_crypt
dcrypt2:
        mov al,[esi]
        xor al,54
        mov [esi],al
        inc esi
        dec ecx
        cmp ecx,0
        jnz dcrypt2

        jmp WndProc_fin
WndProc endp


printhex:  ; eax=nombre out:buf rempli
        pusha
        lea edi,buf
        mov edx,8
        mov [edi],byte ptr '0'
        inc edi
txt_ph_1:
        rol eax,4
        movzx ebx,al
        and bl,0fh
        movzx ebx,byte ptr [offset hexatable+ebx]
        mov [edi],bl
        inc edi
        dec edx
        jnz txt_ph_1
        mov [edi],byte ptr 'h'
        mov [edi+1],byte ptr 0
        popa
        ret

hexatable db '0123456789ABCDEF'
end start
