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
#include <vfs.h>
#include <uio.h>
#include <vnode.h>
#include <copyinout.h>

struct vnode** fileTable;


int sys_open(char* filename, int openflag, int* retval){

	struct vnode* v;

	if(fileTable == NULL){
		fileTable = kmalloc(sizeof(struct vnode*)*100);
		for(int i=0;i<100;i++){
			fileTable[i] = NULL;
		}
	}

	vfs_open(filename,openflag,0,&v);

	for(int i = 3; i < 100; i++){
		if(fileTable[i] == NULL){
			fileTable[i] = v;
			*retval= i;
			return 0;
		}
	}

	return -1;
}

void sys_close(int fd){
	
	struct vnode* v;

	KASSERT(fd != 0 && fd != 1 && fd != 2);
	
	v = fileTable[fd];
	fileTable[fd] = NULL;

	vfs_close(v);
}


int sys_write(int fd, const char * buf,size_t nbytes,int *retval){

	char *tmp;
	struct vnode* v;
	struct iovec iov;
	struct uio u;


	if(nbytes == 0 && buf == NULL && fd != 0){
		return 0;
	}

	//file dispatcher
	switch(fd){
		case 0:
			kprintf("Error - cannot print on STDIN\n");
			return -1;
			break;
		case 1:
		case 2:

			tmp = kmalloc((nbytes+1)*sizeof(char));
			copyinstr((userptr_t) buf,tmp,nbytes,NULL);
			tmp[nbytes]='\0';

		 	kprintf("%s",tmp);
			*retval = 0;


			kfree(tmp);
			break;
		default:

			v = fileTable[fd];

			//initialize uio data structure
			uio_kinit(&iov, &u,(void *) buf, nbytes, 0, UIO_WRITE);

			//it's necessary to copy the address space for user process
			u.uio_space = curproc->p_addrspace;
			u.uio_segflg = UIO_USERSPACE;

			//try to write the file
			*retval = VOP_WRITE(v,&u);

			//VOP_WRITE will return 0 on success
			if((*retval) != 0){
				return -1;
			}

			//return the number of wrote bytes in the file
			*retval = nbytes - u.uio_resid;
			break;		
	}

	return 0;
}


int sys_read(int fd,char * buf,size_t nbytes,int *retval){

		struct vnode* v;
		struct iovec iov;
		struct uio u;

	switch(fd){
		case 0:
			kgets((char *) buf,nbytes);  //nota modifica il prototipo da const char-> char
			*retval = 0;
			break;
		case 1:
		case 2:
			kprintf("Error - cannot read from STOUT/STERR\n");
			*retval = -1;
			return -1;
			break;
		default:

			v = fileTable[fd];

			//initialize uio data structure
			uio_kinit(&iov, &u,(void *) buf, nbytes, 0, UIO_READ);

			//it's necessary to copy the address space for user process
			u.uio_space = curproc->p_addrspace;
			u.uio_segflg = UIO_USERSPACE;

			//try to write the file
			*retval = VOP_READ(v,&u);

			//VOP_READ will return 0 on success
			if((*retval) != 0){
				return -1;
			}

			//return the number of read bytes in the file
			*retval = nbytes - u.uio_resid;
			break;		

	}
	return 0;

}
