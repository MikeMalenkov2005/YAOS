global _init_fdc

global _fdc_get_drive_type
global _fdc_get_cylinder_count
global _fdc_get_default_track_size

global _fdc_stop_drive
global _fdc_select_drive

global _fdc_reset
global _fdc_recalibrate
global _fdc_seek
global _fdc_load
global _fdc_store

global _fdc_get_buffer
global _fdc_get_buffer_size

extern _read_cmos
extern _wait_mils
extern _set_irq_handler

SIZE_ID equ 2
SECTOR_SIZE equ 128 << SIZE_ID

section .text

_init_fdc:
  push _fdc_handler
  push 6
  call _set_irq_handler
  push 0x10
  call _read_cmos
  add esp, 12
  mov [DRIVES], al
  xor eax, eax
  mov [WAITING], al
  ret

_fdc_handler:
  xor eax, eax
  mov [WAITING], al
  ret

_fdc_get_drive_type: ; DRIVE INDEX
  xor eax, eax
  mov cl, [esp + 4]
  mov al, [DRIVES]
  times 4 shr al, cl
  and al, 7
  ret

_fdc_get_cylinder_count: ; DRIVE INDEX
  mov eax, [esp + 4]
  push eax
  call _fdc_get_drive_type
  add esp, 4
  mov ecx, eax
  mov eax, [ecx + CYLS_TABLE]
  ret

_fdc_get_default_track_size: ; DRIVE INDEX
  mov eax, [esp + 4]
  push eax
  call _fdc_get_drive_type
  add esp, 4
  mov ecx, eax
  mov eax, [ecx + CYLS_TABLE]
  ret

_fdc_stop_drive:
  mov edx, 0x3F2 ; DOR
  mov al, 0xFF
  mov [DRIVE], al
  mov al, 0x0C
  out dx, al
  xor eax, eax
  ret

_fdc_select_drive: ; DRIVE INDEX
  xor eax, eax
  mov al, [esp + 4]
  cmp al, [DRIVE]
  je .finish
  push eax
  call _fdc_get_drive_type
  add esp, 4
  test al, al
  jz .no_drive
  mov edx, 0x3F2 ; DOR
  mov al, 0x10
  shl al, cl
  or al, cl
  or al, 0xC
  out dx, al
  push 500
  call _wait_mils
  add esp, 4
.finish:
  xor eax, eax
  ret
.no_drive:
  xor eax, eax
  not eax
  ret

_fdc_init_dma: ; MODE
  mov al, 0x06
  out 0x0A, al
  mov al, 0xFF
  out 0x0C, al
  mov eax, SECTOR_BUFFER
  out 0x04, al
  shr eax, 8
  out 0x04, al
  shr eax, 8
  out 0x81, al
  mov al, 0xFF
  out 0x0C, al
  mov eax, SECTOR_SIZE
  out 0x05, al
  shr eax, 8
  out 0x05, al
  mov al, [esp + 4]
  out 0x0B, al
  mov al, 0x02
  out 0x0A, al
  mov al, 0
  ret

_fdc_write_byte: ; BYTE
  xor eax, eax
  mov ecx, 0x800
  mov edx, 0x3F4 ; MSR
  .l0:
    in al, dx
    and al, 0x80
    jnz .e0
    loop .l0
    not eax
    ret
  .e0:
  mov edx, 0x3F5 ; FIFO
  mov al, [esp + 4]
  out dx, al
  xor eax, eax
  ret

_fdc_read_byte:
  xor eax, eax
  mov ecx, 0x800
  mov edx, 0x3F4 ; MSR
  .l0:
    in al, dx
    and al, 0x80
    jnz .e0
    loop .l0
    not eax
    ret
  .e0:
  mov edx, 0x3F5 ; FIFO
  in al, dx
  ret

_fdc_sence_int: ; RETURNES WORD[ BCN | ST0 ]
  push 8 ; CMD
  call _fdc_write_byte
  not eax
  jz .error
  call _fdc_read_byte
  not eax
  jnz .error
  not eax
  mov [esp], eax
  call _fdc_read_byte
  not eax
  jz .error
  not eax
  mov [esp + 1], al
  pop eax
  ret
.error:
  pop eax
  xor eax, eax
  not eax
  ret

_fdc_reset:
  inc byte [WAITING]
  mov edx, 0x3F2 ; DOR
  mov al, 0x00
  out dx, al
  not al
  mov [DRIVE], al
  mov al, 0x0C
  out dx, al
  .wait_irq:
    mov al, [WAITING]
    test al, al
    jnz .wait_irq
  call _fdc_sence_int
  mov edx, 0x3F7 ; CCR
  mov al, 0
  out dx, al
  push 3
  call _fdc_write_byte
  add esp, 4
  test eax, eax
  jnz .end
  push 0xDF
  call _fdc_write_byte
  add esp, 4
  test eax, eax
  jnz .end
  push 2
  call _fdc_write_byte
  add esp, 4
  test eax, eax
  jnz .end
  xor eax, eax
  .end:
  ret

_fdc_recalibrate: ; DRIVE INDEX
  mov eax, [esp + 4]
  push eax
  call _fdc_select_drive
  add esp, 4
  test eax, eax
  jnz .fail
  mov ecx, 10
  .retry:
    inc byte [WAITING]
    push ecx
    push 7 ; RECALIBRATE
    call _fdc_write_byte
    add esp, 4
    test eax, eax
    jnz .error
    mov eax, [esp + 8]
    push eax
    call _fdc_write_byte
    add esp, 4
    test eax, eax
    jnz .error
    .wait_irq:
      mov al, [WAITING]
      test al, al
      jnz .wait_irq
    call _fdc_sence_int
    not eax
    jz .error
    not eax
    test al, 0xC0
    jnz .error
    test ah, ah
    jnz .error
    pop eax
    xor eax, eax
    ret
  .error:
    pop ecx
    loop .retry
  .fail:
    xor eax, eax
    not eax
    ret

_fdc_seek: ; DRIVE INDEX, CYLINDER, HEAD
  mov eax, [esp + 4]
  push eax
  call _fdc_select_drive
  add esp, 4
  test eax, eax
  jnz .fail
  mov ecx, 10
  .retry:
    inc byte [WAITING]
    push ecx
    push 15 ; SEEK
    call _fdc_write_byte
    add esp, 4
    test eax, eax
    jnz .error
    mov ecx, [esp + 8]
    mov eax, [esp + 16]
    and al, 1
    shl al, 2
    and cl, 3
    or al, cl
    push eax
    call _fdc_write_byte
    add esp, 4
    mov eax, [esp + 12]
    push eax
    call _fdc_write_byte
    pop ecx
    test eax, eax
    jnz .error
    .wait_irq:
      mov al, [WAITING]
      test al, al
      jnz .wait_irq
    push ecx
    call _fdc_sence_int
    pop ecx
    not eax
    jz .error
    not eax
    test al, 0xC0
    jnz .error
    cmp ah, cl
    jne .error
    pop eax
    xor eax, eax
    ret
  .error:
    pop ecx
    loop .retry
  .fail:
    xor eax, eax
    not eax
    ret

_fdc_load: ; DRIVE INDEX, C, H, S, EOT -> NEXT DWORD[ 0 | S | H | C ];
  mov eax, [esp + 12]
  push eax
  mov eax, [esp + 8]
  push eax
  call _fdc_seek
  add esp, 8
  test eax, eax
  jnz .fail
  push ebp
  mov ebp, esp
  push 0x46
  push 6
  mov eax, [ebp + 24]
  push eax
  mov eax, [ebp + 20]
  push eax
  mov eax, [ebp + 16]
  push eax
  mov eax, [ebp + 12]
  push eax
  call _fdc_transfer_sector
  add esp, 24
  pop ebp
  ret
  .fail:
    xor eax, eax
    not eax
    ret

_fdc_store: ; DRIVE INDEX, C, H, S, EOT -> NEXT DWORD[ 0 | S | H | C ];
  mov eax, [esp + 12]
  push eax
  mov eax, [esp + 8]
  push eax
  call _fdc_seek
  add esp, 8
  test eax, eax
  jnz .fail
  push ebp
  mov ebp, esp
  push 0x4A
  push 5
  mov eax, [ebp + 24]
  push eax
  mov eax, [ebp + 20]
  push eax
  mov eax, [ebp + 16]
  push eax
  mov eax, [ebp + 12]
  push eax
  call _fdc_transfer_sector
  add esp, 24
  pop ebp
  ret
  .fail:
    xor eax, eax
    not eax
    ret

_fdc_transfer_sector: ; C, H, S, EOT, CMD, MODE -> NEXT DWORD[ 0 | S | H | C ]; DOES NOT SEEK !!
  push ebx
  push edi
  mov al, [DRIVE]
  push eax
  call _fdc_get_drive_type
  add esp, 4
  test al, al
  jz .fail
  mov ecx, 20
  .retry:
    inc byte [WAITING]
    push ecx
    mov eax, [esp + 24]
    push eax
    call _fdc_init_dma
    add esp, 4
    mov eax, [esp + 20]
    and al, 15
    or al, 0xC0
    push eax
    call _fdc_write_byte
    add esp, 4
    test eax, eax
    jnz .error
    mov eax, [esp + 8]
    mov ah, [DRIVE]
    and al, 1
    shl al, 2
    and ah, 3
    or al, ah
    push eax
    call _fdc_write_byte
    add esp, 4
    test eax, eax
    jnz .error
    mov eax, [esp + 4]
    push eax
    call _fdc_write_byte
    add esp, 4
    test eax, eax
    jnz .error
    mov eax, [esp + 8]
    and al, 1
    push eax
    call _fdc_write_byte
    add esp, 4
    test eax, eax
    jnz .error
    mov eax, [esp + 12]
    push eax
    call _fdc_write_byte
    add esp, 4
    test eax, eax
    jnz .error
    mov eax, SIZE_ID
    push eax
    call _fdc_write_byte
    add esp, 4
    test eax, eax
    jnz .error
    mov eax, [esp + 16]
    push eax
    call _fdc_write_byte
    add esp, 4
    test eax, eax
    jnz .error
    mov al, [DRIVE]
    push eax
    call _fdc_get_drive_type
    add esp, 4
    mov ecx, GPL1_TABLE
    add ecx, eax
    mov al, [ecx]
    push eax
    call _fdc_write_byte
    add esp, 4
    test eax, eax
    jnz .error
    not al
    push eax
    call _fdc_write_byte
    add esp, 4
    test eax, eax
    jnz .error
    .wait_irq:
      mov al, [WAITING]
      test al, al
      jnz .wait_irq
    call _fdc_read_byte
    not eax
    jz .error
    not eax
    shl eax, 8
    mov edi, eax
    call _fdc_read_byte
    shl eax, 16
    or edi, eax
    call _fdc_read_byte
    shl eax, 24
    or edi, eax
    call _fdc_read_byte
    mov ebx, eax
    call _fdc_read_byte
    shl eax, 8
    or ebx, eax
    call _fdc_read_byte
    shl eax, 16
    or ebx, eax
    call _fdc_read_byte
    or eax, edi
    test eax, 0x20000
    jnz .not_writable
    test ah, 0xC8
    jnz .error
    cmp al, SIZE_ID
    jne .error
    shr eax, 16
    test eax, 0x77B5
    jnz .error
    mov eax, ebx
    pop ecx
    pop edi
    pop ebx
    ret
  .error:
    pop ecx
    loop .retry_loop
  .fail:
    pop edi
    pop ebx
    xor eax, eax
    not eax
    ret
  .retry_loop:
    jmp .retry
  .not_writable:
    xor eax, eax
    not eax
    dec eax
    pop ecx
    pop edi
    pop ebx
    ret

_fdc_get_buffer:
  mov eax, SECTOR_BUFFER
  ret

_fdc_get_buffer_size:
  mov eax, SECTOR_SIZE
  ret

section .data

CYLS_TABLE: db 0, 40, 80, 80, 80, 80
EOT_TABLE:  db 0, 9, 15, 9, 18, 36
GPL1_TABLE: db 0, 0x2A, 0x2A, 0x1B, 0x1B, 0x1B
GPL2_TABLE: db 0, 0x50, 0x50, 0x54, 0x54, 0x54

section .bss

align SECTOR_SIZE

SECTOR_BUFFER: resb SECTOR_SIZE

WAITING:  resb 1
DRIVES:   resb 1
DRIVE:    resb 1

