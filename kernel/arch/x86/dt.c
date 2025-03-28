#include <kernel/arch/x86/dt.h>
#include <kernel/arch/x86/apm.h>
#include <kernel/arch/x86/pio.h>

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();
extern void isr37();
extern void isr38();
extern void isr39();
extern void isr40();
extern void isr41();
extern void isr42();
extern void isr43();
extern void isr44();
extern void isr45();
extern void isr46();
extern void isr47();
extern void isr128();

static TASK_STATE_SEGMENT TSS __aligned(16);

static SEGMENT_DESCRIPTOR GDT[9] __aligned(16);

static INTERRUPT_DESCRIPTOR IDT[256] __aligned(16);

static void __naked ReloadKernelSegments()
{
  asm volatile ("mov $0x10, %ax");
  asm volatile ("mov %ax, %ds");
  asm volatile ("mov %ax, %es");
  asm volatile ("mov %ax, %fs");
  asm volatile ("mov %ax, %gs");
  asm volatile ("mov %ax, %ss");
  asm volatile ("pop %eax");
  asm volatile ("pushl $0x08");
  asm volatile ("push %eax");
  asm volatile ("lret");
}

void InitGDT(UINT32 APMTable, UINT32 SystemStack)
{
  DESCRIPTOR_TABLE_POINTER GDTR =
  {
    sizeof(GDT) - 1,
    (UINTPTR)&GDT[0]
  };
  /* Fill required Global Descriptor Table entries */
  GDT[0] = GDT_ENTRY(0, 0, 0, 0);
  GDT[1] = GDT_ENTRY(0, 0xFFFFF, 0x9A, 0xC);
  GDT[2] = GDT_ENTRY(0, 0xFFFFF, 0x92, 0xC);
  GDT[3] = GDT_ENTRY(0, 0xFFFFF, 0xFA, 0xC);
  GDT[4] = GDT_ENTRY(0, 0xFFFFF, 0xF2, 0xC);
  /* Fill the Task State Segment */
  GDT[5] = GDT_ENTRY((UINTPTR)(void*)&TSS, sizeof(TSS) - 1, 0x89, 0);
  TSS = (TASK_STATE_SEGMENT) { 0 };
  TSS.SS0 = 0x10;
  TSS.ESP0 = SystemStack;
  if (APMTable)
  {
    /* Fill optional (APM) Global Descriptor Table entries */
    APM_TABLE *pAPMTable = (void*)(UINTPTR)APMTable;
    GDT[6] = GDT_ENTRY(pAPMTable->CodeSegment, pAPMTable->CodeSegmentLength - 1, 0x9A, 0x8);
    GDT[7] = GDT_ENTRY(pAPMTable->CodeSegment16, pAPMTable->CodeSegment16Length - 1, 0x9A, 0);
    GDT[8] = GDT_ENTRY(pAPMTable->DataSegment, pAPMTable->DataSegmentLength - 1, 0x92, 0);
  }
  else GDTR.Limit -= sizeof(SEGMENT_DESCRIPTOR) * 3;
  /* Load the Global Descriptor Table and Kernel Segments */
  asm volatile ("lgdt %0" : : "m"(GDTR));
  ReloadKernelSegments();
  asm volatile ("ltr %w0" : : "a"(0x2B));
}

void InitIDT()
{
  DESCRIPTOR_TABLE_POINTER IDTR =
  {
    sizeof(IDT) - 1,
    (UINTPTR)&IDT[0]
  };
  /* Remap the Programable Interrupt Controller */
  WritePort8(0x20, 0x11);
  WritePort8(0xA0, 0x11);
  WritePort8(0x21, 0x20);
  WritePort8(0xA1, 0x28);
  WritePort8(0x21, 0x04);
  WritePort8(0xA1, 0x02);
  WritePort8(0x21, 0x01);
  WritePort8(0xA1, 0x01);
  WritePort8(0x21, 0x0);
  WritePort8(0xA1, 0x0);
  /* Fill the Interrupt Descriptor Table */
  int i = 0;
  IDT[i++] = IDT_ENTRY((UINTPTR)isr0, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr1, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr2, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr3, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr4, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr5, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr6, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr7, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr8, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr9, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr10, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr11, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr12, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr13, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr14, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr15, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr16, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr17, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr18, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr19, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr20, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr21, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr22, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr23, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr24, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr25, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr26, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr27, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr28, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr29, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr30, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr31, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr32, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr33, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr34, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr35, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr36, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr37, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr38, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr39, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr40, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr41, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr42, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr43, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr44, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr45, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr46, 8, 0x8E);
  IDT[i++] = IDT_ENTRY((UINTPTR)isr47, 8, 0x8E);
  while (i < 256) IDT[i++] = IDT_ENTRY(0, 0, 0);
  IDT[128] = IDT_ENTRY((UINTPTR)isr128, 8, 0xEE); /* DPL = 3 */
  /* Load the Interrupt Descriptor Table */
  asm volatile ("lidt %0" : : "m"(IDTR));
}
