#include <stdio.h>
#include <string.h>

#include "../h/smod.h"

static unsigned long PVWORK, PVTOTAL;

static void fetch_stats(unsigned long *work, unsigned long *total) {
  FILE* stat_fp;
  unsigned long v[10];

  stat_fp = fopen("/proc/stat", "r");

  fscanf(stat_fp, "cpu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu\n",
    v, v + 1, v + 2, v + 3, v + 4, v + 5, v + 6, v + 7, v + 8, v + 9);

  *work  = v[0] + v[1] + v[2];
  *total = v[0] + v[1] + v[2] + v[3] + v[4] + v[5] + v[6] + v[7] + v[8] + v[9];

  fclose(stat_fp);
}

static int get_status(char *str) {
  unsigned long work, total;
  double cpu_usage;

  fetch_stats(&work, &total);
  cpu_usage = (double)(work - PVWORK) / (total - PVTOTAL) * 100.0;
  
  PVWORK  = work;
  PVTOTAL = total;

  sprintf(str, "%2.2f%% cpu", cpu_usage);

  return 0;
}

static void init(smod_config_t *cfg) {
  fetch_stats(&PVWORK, &PVTOTAL);

  cfg->get_status = get_status;
  cfg->status_length = 16;
  cfg->refresh_rate = 1000 / 25;
}

MODULE(cpuusage, init);
