#include <stdio.h>
#include <string.h>
#include <sys/sysinfo.h>

#include "../h/smod.h"

static int get_status(char *str) {
	FILE *fp;
	char cap[8], pos;

	fp = fopen("/sys/class/power_supply/BAT0/capacity", "r");
	if (!fgets(cap, sizeof(cap), fp))
		return 1;
	fclose(fp);

	for (pos = 0; cap[pos] != 0 && cap[pos] != '\n'; ++pos);
	cap[pos] = 0;

	sprintf(str, "%s%% bat", cap);

  return 0;
}

static void init(smod_config_t *cfg) {
  cfg->get_status = get_status;
  cfg->status_length = 16;
  cfg->refresh_rate = 5000 / 25;
}

MODULE(battery, init);
