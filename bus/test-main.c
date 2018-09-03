/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */
/* test-main.c  main() for make check
 *
 * Copyright (C) 2003 Red Hat, Inc.
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <config.h>
#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus-string.h>
#include <dbus/dbus-sysdeps.h>
#include <dbus/dbus-internals.h>
#include <dbus/dbus-message-internal.h>
#include <dbus/dbus-test-tap.h>
#include "selinux.h"

#ifndef DBUS_ENABLE_EMBEDDED_TESTS
#error This file is only relevant for the embedded tests
#endif

#ifdef DBUS_UNIX
# include <dbus/dbus-sysdeps-unix.h>
#endif

static const char *only;
static DBusInitialFDs *initial_fds = NULL;

static void
test_pre_hook (void)
{
  initial_fds = _dbus_check_fdleaks_enter ();
}

static void
test_post_hook (const char *name)
{
  if (_dbus_getenv ("DBUS_TEST_SELINUX"))
    bus_selinux_shutdown ();

  _dbus_test_check_memleaks (name);
  _dbus_check_fdleaks_leave (initial_fds);
  initial_fds = NULL;
}

static void
test_one (const char *name,
          dbus_bool_t (*func) (const char *),
          const char *test_data_dir)
{
  long before, after;

  if (only != NULL && strcmp (only, name) != 0)
    {
      _dbus_test_skip ("%s - Only intending to run %s", name, only);
      return;
    }

  _dbus_test_diag ("Running test: %s", name);
  _dbus_get_monotonic_time (&before, NULL);

  test_pre_hook ();

  if (func (test_data_dir))
    _dbus_test_ok ("%s", name);
  else
    _dbus_test_not_ok ("%s", name);

  _dbus_get_monotonic_time (&after, NULL);

  _dbus_test_diag ("%s test took %ld seconds", name, after - before);

  test_post_hook (name);
}

int
main (int argc, char **argv)
{
  const char *dir;

  if (argc > 1 && strcmp (argv[1], "--tap") != 0)
    dir = argv[1];
  else
    dir = _dbus_getenv ("DBUS_TEST_DATA");

  if (argc > 2)
    only = argv[2];
  else
    only = _dbus_getenv ("DBUS_TEST_ONLY");

  if (dir == NULL)
    _dbus_test_fatal ("Must specify test data directory as argv[1] or in DBUS_TEST_DATA env variable");

#ifdef DBUS_UNIX
  /* close any inherited fds so dbus-spawn's check for close-on-exec works */
  _dbus_close_all ();
#endif

  test_one ("expire-list", bus_expire_list_test, dir);
  test_one ("config-parser", bus_config_parser_test, dir);
  test_one ("signals", bus_signals_test, dir);
  test_one ("dispatch-sha1", bus_dispatch_sha1_test, dir);
  test_one ("dispatch", bus_dispatch_test, dir);
  test_one ("activation-service-reload",
            bus_activation_service_reload_test, dir);

#ifdef HAVE_UNIX_FD_PASSING
  test_one ("unix-fds-passing", bus_unix_fds_passing_test, dir);
#else
  _dbus_test_skip ("fd-passing not supported on this platform");
#endif

  return _dbus_test_done_testing ();
}
