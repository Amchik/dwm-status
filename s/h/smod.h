/* vim: ft=c
 */

#ifndef __smod_header
#define __smod_header

#include <sys/types.h>

enum smod_config_flags {
  /**
   * Ignore errors (do not show on error).
   */
  SM_ERROR_IS_WARNING = (1 << 0),
  /**
   * Runs function in new thread
   * (useful if it can panic)
   */
  SM_THREAD_ISOLATE = (1 << 1),
};

typedef struct {
  /**
   * Refresh rate.
   * get_status() will be called every
   * (25 * refresh_rate) ms.
   * Range from 0 to 10 (safe) or 10! (unsafe).
   * If range == 0 or range > 10!, module will be disabled.
   * (note for dummies: 10! is factorial of 10 or 2*3*4*5*...*10)
   */
  u_int32_t refresh_rate;

  /**
   * Length of string that will be status.
   * Range from 1 (empty string) to memory you have.
   * It will be warned (panic on -DWARN_IS_PANIC) if
   * it more than 64.
   */
  size_t status_length;

  /**
   * (not works yet)
   * Module flags. See more in (enum smod_config_flags)
   * SM_* defines. Defaults is zero.
   */
  int flags;

  /**
   * Handler to get status. Must return 0
   * on success and change *status.
   */
  int (*get_status)(char *status);
} smod_config_t;

struct smod {
  const char *modname;
  void (*mod_init)(smod_config_t *cfg);
};

extern struct smod __start_dwmstatusmods;
extern struct smod __stop_dwmstatusmods;

#define MODULE(name, init) \
  struct smod name \
    __attribute__((section("dwmstatusmods"))) \
      = { .modname = #name, .mod_init = init }

#endif /* __smod_header */
