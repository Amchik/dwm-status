#include <stdio.h>
#include <string.h>
#include <sys/sysinfo.h>

#include "../h/smod.h"

static int get_status(char *str) {
  struct sysinfo memInfo;
  double mem_free;

  sysinfo(&memInfo);
  mem_free = (double)memInfo.freeram * memInfo.mem_unit / 1024 / 1024 / 1024;

  sprintf(str, "%2.2fgb free", mem_free);

  return 0;
}

static void init(smod_config_t *cfg) {
  cfg->get_status = get_status;
  cfg->status_length = 16;
  cfg->refresh_rate = 1000 / 25;
}

MODULE(memusage, init);
