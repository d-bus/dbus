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
test_pre_hook (void)
{
}

static const char *progname = "";
static void
test_post_hook (void)
{
  _dbus_test_check_memleaks (progname);
}

int
main (int argc, char **argv)
{
  const char *dir;

  progname = argv[0];

  if (argc > 1 && strcmp (argv[1], "--tap") != 0)
    dir = argv[1];
  else
    dir = _dbus_getenv ("DBUS_TEST_DATA");

  if (dir == NULL)
    _dbus_test_fatal ("Must specify test data directory as argv[1] or in DBUS_TEST_DATA env variable");

  test_pre_hook ();
  _dbus_test_diag ("%s: Running config file parser (trivial) test", argv[0]);
  if (!bus_config_parser_trivial_test (dir))
    _dbus_test_fatal ("OOM creating parser");

  /* All failure modes for this test are currently fatal */
  _dbus_test_ok ("%s", argv[0]);
  test_post_hook ();

  return _dbus_test_done_testing ();
}
