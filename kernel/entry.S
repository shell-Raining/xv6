	# qemu -kernel loads the kernel at 0x80000000
        # and causes each CPU to jump there.
        # kernel.ld causes the following code to
        # be placed at 0x80000000.


.section .text
.global _entry
_entry:
	# set up a stack for C.
        # stack0 is declared in start.c,
        # with a 4096-byte stack per CPU.
        # sp = stack0 + (hartid * 4096)
        la sp, stack0 # 0x8001e140 sp
        li a0, 1024*4
	csrr a1, mhartid # mhartid 是当前线程数，而我们要为每个CPU都开辟空间，所以需要mhartid
        addi a1, a1, 1
        mul a0, a0, a1
# 移动栈指针到栈顶
        add sp, sp, a0
	# jump to start() in start.c
        call start
spin:
        j spin
