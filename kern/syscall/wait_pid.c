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

int wait_pid(pid_t pid){

	struct proc *p;

	p = proc_from_table(pid);

	KASSERT(p != NULL);
	return wait_proc(p);
}