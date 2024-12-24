// TAKEN FROM https://github.com/timsousa1/snake
#ifndef T_TERM
#define T_TERM

#include <termios.h>
#include <unistd.h>

#define clear_screen() printf("\033[H\033[J")

int enableRAW(struct termios *original, int updateTime);
int disableRAW(struct termios *original);

#endif
