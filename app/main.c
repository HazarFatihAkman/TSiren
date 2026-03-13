#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

void trigger_siren() {
#ifdef _WIN32
  system("powershell.exe -c \"[Console]::Beep(1000, 500)\"");
#elif __APPLE__
  system("afplay /System/Library/Sounds/Ping.aiff &");
#else
  system("paplay /usr/share/sounds/freedesktop/stereo/message.oga || canberra-gtk-play --id=\"message-new-instant\" &");
#endif
}

void set_conio_mode() {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag &= ~(ICANON | ECHO);
    ttystate.c_cc[VMIN] = 0;
    ttystate.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

int main(int argv, char *args[]) {
  if (args[1] == NULL) {
    args[1] = 0;
  }

  if (args[2] == NULL) {
    args[2] = 0;
  }

  int min = atoi(args[1]);
  int sec = atoi(args[2]);
  int counter_sec = min * 60 + atoi(args[2]);
  int d_min = min, d_sec = sec;
  do {
    dprintf(STDOUT_FILENO, "\r\033[2K%02d:%02d", d_min, d_sec);
    clock_t stop = clock() + CLOCKS_PER_SEC;
    while (clock() < stop) {}
    counter_sec--;

    d_sec = counter_sec % 60;
    if (d_sec == 59) d_min--;
  } while (0 <= counter_sec);

  dprintf(STDOUT_FILENO, "\r\nSiren triggered. Press 'q' to turn off : ");
  set_conio_mode();
  char c;
  int counter = 0;
  while(c != 'q') {
    if (counter % 100 == 0) {
        trigger_siren();
    }

    if (read(STDIN_FILENO, &c, 1) > 0 && c == 'q') break;
    usleep(10000);
    counter++;
  }

  struct termios ttystate;
  tcgetattr(STDIN_FILENO, &ttystate);
  ttystate.c_lflag |= (ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

  return 0;
}

