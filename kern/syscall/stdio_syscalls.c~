#include <kern/unistd.h>
#include <types.h>
#include <lib.h>
#include <syscall.h>
#include <copyinout.h>


int sys_write(int fd, const char * buf,size_t nbytes,int *retval){

	char *tmp;
	tmp = kmalloc((nbytes+1)*sizeof(char));
	copyinstr((userptr_t) buf,tmp,nbytes,NULL);
	tmp[nbytes]='\0';

	switch(fd){
		case 1:
		 	kprintf("%s",tmp);
			*retval = 0;
			break;
		case 2:
			for(int i =0; i < (int) nbytes; i++ ){
			kprintf("%c",buf[i]);						//stderr
		}
			*retval = 0;
			break;
		default:
			kprintf("Error - cannot print on STDIN\n");
			*retval = -1;
			break;
	}
	kfree(tmp);
	return 0;
}

int sys_read(int fd,char * buf,size_t nbytes,int *retval){

	switch(fd){
		case 0:
			kgets((char *) buf,nbytes);  //nota modifica il prototipo da const char-> char
			*retval = 0;
			break;
		default:
			kprintf("Error - cannot read from STOUT/STERR\n");
			*retval = -1;
			break;
	}
	return 0;

}
