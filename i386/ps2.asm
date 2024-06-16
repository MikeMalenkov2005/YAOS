global _init_ps2

extern _set_irq_handler

section .text

_init_ps2:
  push _irq1_handler
  push 1
  call _set_irq_handler
  add esp, 8
  push _irq12_handler
  push 12
  call _set_irq_handler
  add esp, 8
  ret

_irq1_handler:
  ret

_irq12_handler:
  ret


