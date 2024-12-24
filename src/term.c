// TAKEN FROM https://github.com/timsousa1/snake
#include "term.h"
#include <termios.h>
#include <unistd.h>

/*
struct termios {
  tcflag_t c_iflag;    // input specific flags (bitmask)
  tcflag_t c_oflag;    // output specific flags (bitmask)
  tcflag_t c_cflag;    // control flags (bitmask)
  tcflag_t c_lflag;    // local flags (bitmask)
  cc_t     c_cc[NCCS]; // special characters
};
*/

// TCSAFLUSH (when to apply change) waits for all output and discards unread input
int enableRAW(struct termios *original, int updateTime){
    struct termios raw;

    if (tcgetattr(STDIN_FILENO, original) == -1) return -1;

    raw = *original;
    raw.c_lflag &= ~(ECHO | ICANON);         // toggling both COOKED and ECHO mode

    raw.c_cc[VTIME] = updateTime;            // time between bytes in tenths of a second
    raw.c_cc[VMIN] = 0;                      // min number of bytes

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) return -1;
    return 0;
}

int disableRAW(struct termios *original){
    return tcsetattr(STDIN_FILENO, TCSAFLUSH, original);
}
