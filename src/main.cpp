#include "PCB.h"
int userMain(int argc, char **argv);

// stara prekidna rutina
unsigned oldTimerOFF, oldTimerSEG;
// postavlja novu prekidnu rutinu
// deklaracija nove prekidne rutine
void interrupt timer();

void inic()
{
	PCB::initPCB();
    asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax // es = 0

            // pamti staru rutinu
		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

            // postavlja novu rutinu
		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

            // postavlja staru rutinu na int 60h
		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
    }
}

// // vraca staru prekidnu rutinu
void restore()
{
    asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
    }
}



int main(int argc, char **argv)
{
    inic();
    userMain(argc, argv);
    restore();
}


