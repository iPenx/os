#ifndef _X86_TSS_H
#define _X86_TSS_H

void update_tss_esp(struct tcb* pthread);
void install_tss(void);

#endif
