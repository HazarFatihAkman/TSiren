#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <stdint.h>

void trigger_siren() {
#ifdef _WIN32
  system("powershell.exe -c \"[Console]::Beep(1000, 500)\"");
#elif __APPLE__
  system("afplay /System/Library/Sounds/Ping.aiff &");
#else
  system("paplay /usr/share/sounds/freedesktop/stereo/message.oga || canberra-gtk-play --id=\"message-new-instant\" &");
#endif
}

void set_conio_mode(uint8_t enable) {
  static struct termios t;
  struct termios new_t;
  if (enable) {
    tcgetattr(STDIN_FILENO, &t);
    new_t.c_lflag &= ~(ICANON | ECHO);
    new_t.c_cc[VMIN] = 0;
    new_t.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_t);
  }
  else {
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
  }
}

void d_print(int d_min, int d_sec) {
  char buf[32];
  int len = snprintf(buf, sizeof(buf), "\r\033\[2K%02d:%02d", d_min, d_sec);
  write(STDOUT_FILENO, buf, len);
}

int main(int argv, char *args[]) {
  if (argv < 3) {
    write(STDOUT_FILENO, "Usage: [min] [sec]\r\n", 20);
    return 1;
  }

  int min = atoi(args[1]);
  int sec = atoi(args[2]);
  int counter_sec = min * 60 + atoi(args[2]);
  int d_min = min, d_sec = sec;
  do {
    d_print(d_min, d_sec);
    if (counter_sec > 0) {
      sleep(1);
    }
    counter_sec--;

    d_sec = counter_sec % 60;
    if (d_sec == 59) d_min--;
  } while (0 <= counter_sec);

  write(STDOUT_FILENO, "\r\nSiren triggered. Press 'q' to turn off : ", 30);
  set_conio_mode(1);
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

  set_conio_mode(0);
  return 0;
}

