#include "Common.h"

/* fonction pour encoyer sur le uart */
//void uart_send(u08 *buf, u08 size);


/* fonction pour encoyer sur le uart les streams de debug voulu */
void debug_send(u08 *buf, u08 size);


void uart_init(void);

/* contient les 3 donn�es importantes d�claration global*/
extern volatile int commande[3];
