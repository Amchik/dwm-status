#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../h/smod.h"

static int get_status(char *str) {
  time_t _tm = time(0);
  struct tm tm = *localtime(&_tm);

  sprintf(str, "%.2d:%.2d",
      tm.tm_hour, tm.tm_min);

  return 0;
}

static void init(smod_config_t *cfg) {
  cfg->get_status = get_status;
  cfg->status_length = 60;
}

MODULE(timeonly, init);

