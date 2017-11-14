/* -*- mode: C; c-file-style: "gnu" -*- */
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
#include <dbus/dbus-test-tap.h>

#if !defined(DBUS_ENABLE_EMBEDDED_TESTS) || !defined(DBUS_UNIX)
#error This file is only relevant for the embedded tests on Unix
#endif

static void
check_memleaks (const char *name)
{
  dbus_shutdown ();

  _dbus_test_diag ("%s: checking for memleaks", name);
  if (_dbus_get_malloc_blocks_outstanding () != 0)
    {
      _dbus_test_fatal ("%d dbus_malloc blocks were not freed",
                        _dbus_get_malloc_blocks_outstanding ());
    }
}

static void
test_pre_hook (void)
{
}

static const char *progname = "";
static void
test_post_hook (void)
{
  check_memleaks (progname);
}

int
main (int argc, char **argv)
{
  const char *dir;
  DBusString test_data_dir;

  progname = argv[0];

  if (argc > 1 && strcmp (argv[1], "--tap") != 0)
    dir = argv[1];
  else
    dir = _dbus_getenv ("DBUS_TEST_DATA");

  if (dir == NULL)
    _dbus_test_fatal ("Must specify test data directory as argv[1] or in DBUS_TEST_DATA env variable");

  _dbus_string_init_const (&test_data_dir, dir);

  if (!_dbus_threads_init_debug ())
    _dbus_test_fatal ("OOM initializing debug threads");

  test_pre_hook ();
  _dbus_test_diag ("%s: Running config file parser (trivial) test", argv[0]);
  if (!bus_config_parser_trivial_test (&test_data_dir))
    _dbus_test_fatal ("OOM creating parser");
  test_post_hook ();

  _dbus_test_diag ("%s: Success", argv[0]);

  return 0;
}
