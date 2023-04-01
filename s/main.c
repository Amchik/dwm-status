#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/wait.h>

#include "h/smod.h"
#include "h/config.h"

#ifdef XORG
#include <X11/Xlib.h>

Display *X_DISPLAY;
Window   X_WINDOW;
static void init_xorg() {
    X_DISPLAY = XOpenDisplay(0);
    if (!X_DISPLAY) {
        fprintf(stderr, "Failed to open display\n");
        exit(1);
    }
    X_WINDOW  = XDefaultRootWindow(X_DISPLAY);
}
#endif

#define IS_DEBUG  (progmode & 1) == 1
#define IS_DAEMON (progmode & 2) == 2

typedef struct {
  char   *ptr;
  size_t len;
} str_t;

#define initsmod_config(cfg) \
  (cfg)->flags         = 0;\
  (cfg)->get_status    = 0;\
  (cfg)->refresh_rate  = 1;\
  (cfg)->status_length = 32;

void handle_sigint() {
  exit(0);
}
static void display_bar(const char *str);

int main(int argc, char **argv) {
  size_t i, cnt, statusstr_len, statusstr_cur, modstr_len;
  smod_config_t mods[&__stop_dwmstatusmods - &__start_dwmstatusmods];
  char *statusstr;
  str_t status_strings[&__stop_dwmstatusmods - &__start_dwmstatusmods];
  int res, progmode;

#ifdef XORG
  init_xorg();
#endif

  progmode = 0;
  for (i = 1; i < argc; i += 1) {
    if (!strcmp(argv[i], "-g")) {
      progmode |= 1;
    } else if (!strcmp(argv[i], "-h")) {
      printf("Usage: %s -h|-v\n"
             "       %s -g\n"
             "       %s\n"
             "\n"
             "  -g   Debug mode. Do not run xsetroot and print result to stdout.\n"
             "  -d   Run as daemon.\n"
             "  -h   Shows help message (this message) and exit.\n"
             "  -v   Shows dwm-status version and exit.\n",
             argv[0], argv[0], argv[0]);
      return 0;
    } else if (!strcmp(argv[i], "-v")) {
      puts("dwm-status by ceheki! version 1.1.0\n"
           "source: 09/11/22 (done), 09/19/22 (updated)");
      return 0;
    } else if (!strcmp(argv[i], "-d")) {
      progmode |= 2;
    } else {
      fprintf(stderr, "Unknown argument '%s'. Try\n $ %s -h\n", argv[i], argv[0]);
      return 1;
    }
  }

  if (IS_DAEMON)
    daemon(0, 0);

  signal(SIGINT, handle_sigint);
  signal(SIGTERM, handle_sigint);

  statusstr_len = 0;

  for (i = 0; i < (&__stop_dwmstatusmods - &__start_dwmstatusmods); i += 1) {
    initsmod_config(mods + i);
    (&__start_dwmstatusmods + i)->mod_init(mods + i);
    statusstr_len += (mods + i)->status_length - 1;
    status_strings[i].ptr = (char*)malloc((mods + i)->status_length);
    status_strings[i].ptr[0] = 0;
    status_strings[i].len = 0;
    if (IS_DEBUG)
      printf("\033[1;32m[+]\033[0m %s\n", (&__start_dwmstatusmods + i)->modname);
  }

  statusstr = malloc(statusstr_len + 1);
  if (IS_DEBUG)
    printf("\n");

  cnt = 0;
  while (1) {
    statusstr_cur = 0;

    for (i = 0; i < (&__stop_dwmstatusmods - &__start_dwmstatusmods); i += 1) {
      /* notice: this checks may not required because mods[i] immutable */
      if (mods[i].status_length == 0 || mods[i].get_status == 0)
        continue; /* skip broken module */

      if (cnt % mods[i].refresh_rate == 0) {
        res = mods[i].get_status(status_strings[i].ptr);
        if (res == 0) {
          status_strings[i].len = strlen(status_strings[i].ptr);
        } else {
          /* error handling... */
          if (IS_DEBUG) {
            printf("\033[1;31m[!]\033[0m get_status() of %s returned %d\n\n",
                (&__start_dwmstatusmods + i)->modname, res);
          }
        }
      }

      if (status_strings[i].len == 0)
        continue; /* skip empty string */

      if (i != 0) {
        memcpy(statusstr + statusstr_cur, SEPARATOR, sizeof(SEPARATOR) - 1);
        statusstr_cur += sizeof(SEPARATOR) - 1;
      }

      memcpy(statusstr + statusstr_cur, status_strings[i].ptr, status_strings[i].len);
      statusstr_cur += status_strings[i].len;
    }
    statusstr[statusstr_cur] = 0;

    if (IS_DEBUG) {
      printf("\033[A\033[G\033[2K");
      puts(statusstr);
    } else {
      display_bar(statusstr);
    }

    cnt += 1;
    if (cnt > (2*3*4*5*6*7*8*9*10)) {
      cnt = 0;
    }
    usleep(25000);
  }

  return 0;
}

#ifdef XORG
static void display_bar(const char *str) {
  XStoreName(X_DISPLAY, X_WINDOW, str);
  XFlush(X_DISPLAY);
}
#else
static void display_bar(const char *str) {
  int res, pid;

  pid = fork();
  if (pid < 0) {
    /* handle error... */
    perror("fork()");
    exit(127);
  } else if (pid == 0) {
    execl("/usr/bin/xsetroot", "xsetroot", "-name", str, 0);
    exit(127);
  }
  do {
    waitpid(pid, &res, 0);
  } while (!WIFEXITED(res));
}
#endif
