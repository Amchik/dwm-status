#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../h/smod.h"

const char MONTHS[][4] = {
  [ 1] = "Jan",
  [ 2] = "Feb",
  [ 3] = "Mar",
  [ 4] = "Apr",
  [ 5] = "May",
  [ 6] = "Jun",
  [ 7] = "Jul",
  [ 8] = "Aug",
  [ 9] = "Sep",
  [10] = "Oct",
  [11] = "Nov",
  [12] = "Dec",
};

static int get_status(char *str) {
  time_t _tm = time(0);
  struct tm tm = *localtime(&_tm);

  sprintf(str, "%.2d %s %d",
      tm.tm_mday, MONTHS[tm.tm_mon + 1], tm.tm_year + 1900);

  return 0;
}

static void init(smod_config_t *cfg) {
  cfg->get_status = get_status;
  cfg->status_length = 60;
}

MODULE(dateonly, init);

