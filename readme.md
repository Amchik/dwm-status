# `dwm-status`

Not suckless way, but suckless way sucks.

## Building & Usage

See `mk.conf` for build options.

Build: `$ ./make`

    Usage: ./dwm-status -h|-v
           ./dwm-status -g
           ./dwm-status

      -g   Debug mode. Do not run xsetroot and print result to stdout.
      -d   Run as daemon.
      -h   Shows help message (this message) and exit.
      -v   Shows dwm-status version and exit.

Run as daemon (detach from terminal): `./dwm-status -d`

## Patches

Ok, may be it use some suckless things, like patches.

Create PR with label `patch`, to apply it use `.diff` links.
(like `https://github.com/torvalds/linux/pull/684.diff`)

## Writing module

Create new file in `s/m/0000-mymodule.c`. First 4
digits is a priority.

```c
#include <stdio.h>
#include <string.h>
#include <time.h>

/* see more docs here: */
#include "../h/smod.h"

static int get_status(char *str) {
  time_t _tm = time(0);
  struct tm tm = *localtime(&_tm);

  sprintf(str, "%.2d:%.2d",
      tm.tm_hour, tm.tm_min);

  return 0; /* get_status() must return 0 */
  /* also, if it return non-zero code,
   * it will be displayed with -g argument */
}

static void init(smod_config_t *cfg) {
  cfg->get_status = get_status; /* get_status() handler */
  cfg->status_length = 60; /* status length (wow) */
  cfg->refresh_rate = 1; /* refreshes every 1 * 25ms */
}

MODULE(mymodule /* CHANGE IT! */, init);
/*
 * More (uncommented) examples in s/m/*.c
 */
```

## To-Do

* [ ] Man Page
* [x] `s/h/config.h` with some consts like SEPARATOR


