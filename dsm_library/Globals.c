#include <stdio.h>
#include "Globals.h"

void blockSignals() {
    sigset_t mask;
    sigemptyset (&mask);
    sigaddset (&mask, SIGIO);
    if (sigprocmask(SIG_BLOCK, &mask, &orig_mask) < 0) {
        perror ("sigprocmask");
        return;
    }
}

void unblockSignals() {
    if (sigprocmask(SIG_SETMASK, &orig_mask, NULL) < 0) {
        perror ("sigprocmask");
        return;
    }
}