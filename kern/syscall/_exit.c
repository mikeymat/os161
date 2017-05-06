#include <types.h>
#include <kern/errno.h>
#include <kern/fcntl.h>
#include <lib.h>
#include <proc.h>
#include <current.h>
#include <addrspace.h>
#include <vm.h>
#include <vfs.h>
#include <syscall.h>
#include <test.h>
#include <thread.h>

void _exit(int code){

	struct thread *t;
	t = curthread;
	t->exit_code = code;
	thread_exit();
	kprintf("Exit with code %d",code);
}
