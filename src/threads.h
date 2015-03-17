#ifndef THREADS_H
#define	THREADS_H

#include "global.h"
#include "includes.h"

#ifdef	__cplusplus
extern "C" {
#endif
        void connecter (void * arg);
        void communiquer(void *arg);
        void deplacer(void *arg);
        void envoyer(void *arg);
        void print_status(int status);
        void batterie_state(void * args);
        void images(void *args);
#ifdef	__cplusplus
}
#endif

#endif	/* THREADS_H */

