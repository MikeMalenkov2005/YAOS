org 0x800
bits 16

__FAT_TIME__ equ ((((__TIME_NUM__ / 10000) % 100) << 11) | (((__TIME_NUM__ / 100) % 100) << 5) | ((__TIME_NUM__ % 100) >> 1))
__FAT_DATE__ equ (((((__DATE_NUM__ / 10000) - 1980) % 128) << 9) | (((__DATE_NUM__ / 100) % 100) << 5) | (__DATE_NUM__ % 100))

SECTOR_SIZE equ 512
VOLUME_SIZE equ 2880
FAT_COUNT   equ 2
FAT_SIZE    equ 9

FAT_SECTOR  equ EOF + 0
ROOT_SECTOR equ EOF + 4
DATA_SECTOR equ EOF + 8
TEST_BYTE   equ EOF + 12

DATA_BUFFER equ EOF + SECTOR_SIZE

BPB:
  .jump_to_code:
    jmp short init
    nop
  .oem_id:        db "YAOS 0.1"
  .sector_size:   dw SECTOR_SIZE
  .cluster_size:  db 1
  .reserved_size: dw (EOF - $$) / SECTOR_SIZE
  .fat_count:     db FAT_COUNT
  .root_entries:  dw 224
  .volume_size:
    %if VOLUME_SIZE < 0x10000
      dw VOLUME_SIZE
    %else
      dw 0
    %endif
  .media_type:    db 0xF0
  .fat_size:      dw FAT_SIZE
  .track_size:    dw 18
  .head_count:    dw 2
  .hidden_size:   dd 0
  .big_volume_size:
    %if VOLUME_SIZE < 0x10000
      dd 0
    %else
      dd VOLUME_SIZE
    %endif

EBPB:
  .drive:       db 0
  .nt_flags:    db 0
  .signature:   db 0x29
  .volume_id:   dd __POSIX_TIME__
  .volume_name: db "YAOS SYSTEM"
  .system_id:   db "FAT12   "

init:
  cli
  cld
  xor ax, ax
  mov sp, ax
  mov ds, ax
  mov es, ax
  mov ah, 0x80
  mov ss, ax
  mov di, $$
  mov si, 0x7C00
  mov cx, 512
  rep movsb
  jmp 0:boot

boot:
;;;---            RESET THE BOOT DRIVE             ---;;;
  mov [EBPB.drive], dl
  mov ah, 0
  int 0x13
  test ah, ah
  jnz panic
;;;---    CALCULATE OFFSETS AND LAOD EXTENTION     ---;;;
  mov ax, [BPB.hidden_size]
  mov dx, [BPB.hidden_size + 2]
  inc ax
  adc dx, 0
  mov cx, [BPB.reserved_size]
  dec cx
  push ax
  push dx
  mov bx, EXTENDED_BOOT
  call read_sectors
  test cx, cx
  mov cx, ax
  pop dx
  pop ax
  jnz panic
  add ax, cx
  adc dx, 0
  mov [FAT_SECTOR], ax
  mov [FAT_SECTOR + 2], dx
  mov ch, 0
  mov cl, [BPB.fat_count]
  .l0:
    add ax, [BPB.fat_size]
    adc dx, 0
    loop .l0
  mov [ROOT_SECTOR], ax
  mov [ROOT_SECTOR + 2], dx
  mov ax, [BPB.root_entries]
  mov cx, 32 ; FAT DIR ENTRY SIZE
  mul cx
  div word [BPB.sector_size]
  mov cx, ax
  xor dx, dx
  add ax, [ROOT_SECTOR]
  adc dx, [ROOT_SECTOR + 2]
  mov [DATA_SECTOR], ax
  mov [DATA_SECTOR + 2], dx
;;;---          LOAD KERNEL.SYS FROM ROOT          ---;;;
  mov bx, DATA_BUFFER
  mov ax, [ROOT_SECTOR]
  mov dx, [ROOT_SECTOR + 2]
  call read_sectors
  test cx, cx
  jnz panic
  mov si, bx
  mov cx, [BPB.root_entries]
  .l1:
    push si
    push cx
    mov di, KERNEL_NAME
    mov cx, 11
    repe cmpsb
    pop cx
    pop si
    je .e1
    add si, 32
    loop .l1
    jmp panic
  .e1:
  mov ax, [si + 28]
  mov dx, [si + 30]
  div word [BPB.sector_size]
  test dx, dx
  jz .fi0
    xor dx, dx
    inc ax
  .fi0:
  mov cx, ax
  mov ax, [si + 26]
  sub ax, 2
  jb panic
  push cx
  mov cl, [BPB.cluster_size]
  mov ch, 0
  mul cx
  pop cx
  add ax, [DATA_SECTOR]
  adc dx, [DATA_SECTOR + 2]
  call read_sectors
  test cx, cx
  jnz panic
;;;---          SWITCH TO PROTECTED MODE           ---;;;
  cli
  push ax
  call enable_a20
  pop cx
  jz panic
  lgdt [TMP_GDT.PTR]
  mov eax, cr0
  or al, 1
  mov cr0, eax
  jmp 8:relocate

;;;---         PANIC (TODO: HANDLE PANIC)          ---;;;
panic:
  jmp $

;;;---          ENABLES A20 ADDRESS LINE           ---;;;
enable_a20:
  call .check
  jnz .end
  mov ax, 0x2401
  int 0x15
  call .check
  jnz .end
  call .wait
  mov al, 0xD1
  out 0x64, al
  call .wait
  mov al, 0xDF
  out 0x60, al
  call .wait
  call .check
  jnz .end
  in al, 0x92
  or al, 2
  and al, 0xFE
  out 0x92, al
  jmp .check
.wait:
  in al, 0x64
  test al, 2
  jnz .wait
  ret
.check:
  push si
  push di
  push es
  push ds
  xor ax, ax
  mov ds, ax
  mov si, TEST_BYTE
  not ax
  mov es, ax
  mov di, TEST_BYTE + 0x10
  mov [ds:si], al
  not ax
  mov [es:di], al
  cmp al, [ds:si]
  pop ds
  pop es
  pop di
  pop si
.end:
  ret

;;;--- CONVERT LBA IN DX:AX TO CHS IN BIOS FORMAT  ---;;;
to_chs:
  div word [BPB.track_size]
  mov cx, dx
  xor dx, dx
  div word [BPB.head_count]
  mov dh, dl
  mov dl, [EBPB.drive]
  mov ch, al
  inc cl
  ret

;;;--- READS CX SECTORS TO ES:BX STARTING AT DX:AX ---;;;
read_sectors:
  push es
  push bx
  push cx
  test cx, cx
  jz .e0
  .l0:
    push ax
    push cx
    push dx
    call to_chs
    mov al, 4
    .retry:
      push ax
      mov ax, 0x0201
      int 0x13
      test ah, ah
      pop ax
      jz .next
      dec al
      jnz .retry
      pop dx
      pop cx
      pop ax
      jmp .e0
    .next:
    add bx, word [BPB.sector_size]
    jnc .proper_seg
      mov ax, es
      add ax, 0x1000
      mov es, ax
    .proper_seg:
    pop dx
    pop cx
    pop ax
    loop .l0
  .e0:
  pop ax
  sub ax, cx
  pop bx
  pop es
  ret

KERNEL_NAME: db "KERNEL  SYS"

times (SECTOR_SIZE - 2) - ($ - $$) db 0
dw 0xAA55
EXTENDED_BOOT:

bits 32

relocate:
  xor eax, eax
  mov al, 16
  mov ss, ax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov eax, ecx
  mov edx, 0xFFFF
  and eax, edx
  mov ecx, [BPB.sector_size]
  and ecx, edx
  shr ecx, 2
  mul ecx
  mov ecx, eax
  mov esp, 0x80000
  mov esi, DATA_BUFFER
  mov edi, 0x100000
  rep movsd
  jmp 0x100000

align 8

TMP_GDT:
  dq 0
  dq 0x00CF9A000000FFFF
  dq 0x00CF92000000FFFF
  .PTR:
    dw $ - TMP_GDT - 1
    dd TMP_GDT

align SECTOR_SIZE, db 0

EOF:

%rep FAT_COUNT
dd 0xFFFFF0
times (FAT_SIZE * SECTOR_SIZE) - 4 db 0
%endrep

VOLUME_LABEL:
  .short_name:      db "MDOS SYSTEM"
  .attributes:      db 8
  .attributes_ext:  db 0
  .creation_santis: db 0
  .creation_time:   dw __FAT_TIME__
  .creation_date:   dw __FAT_DATE__
  .access_date:     dw __FAT_DATE__
  .cluster_high:    dw 0
  .change_time:     dw __FAT_TIME__
  .change_date:     dw __FAT_DATE__
  .cluster_low:     dw 0
  .size_in_bytes:   dd 0

times (VOLUME_SIZE * SECTOR_SIZE) - ($ - $$) db 0

