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
#include <synch.h>

void _exit(int code){

	struct thread *t;
	struct proc *p;
	t = curthread;
	p = t->t_proc;
	t->exit_code = code;
	proc_remthread(t);
	KASSERT(t->t_proc == NULL);

	p->exitcode = code;
	V(p->proc_sem);
	thread_exit();
}
