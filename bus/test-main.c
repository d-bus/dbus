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

static DBusInitialFDs *initial_fds = NULL;

static void
test_pre_hook (void)
{
  
  if (_dbus_getenv ("DBUS_TEST_SELINUX")
      && (!bus_selinux_pre_init ()
	  || !bus_selinux_full_init ()))
    _dbus_test_fatal ("Could not init selinux support");

  initial_fds = _dbus_check_fdleaks_enter ();
}

static const char *progname = "";

static void
test_post_hook (void)
{
  if (_dbus_getenv ("DBUS_TEST_SELINUX"))
    bus_selinux_shutdown ();
  check_memleaks (progname);

  _dbus_check_fdleaks_leave (initial_fds);
  initial_fds = NULL;
}

int
main (int argc, char **argv)
{
  const char *dir;
  const char *only;
  DBusString test_data_dir;

  progname = argv[0];

  if (argc > 1 && strcmp (argv[1], "--tap") != 0)
    dir = argv[1];
  else
    dir = _dbus_getenv ("DBUS_TEST_DATA");

  if (argc > 2)
    only = argv[2];
  else
    only = NULL;

  if (dir == NULL)
    _dbus_test_fatal ("Must specify test data directory as argv[1] or in DBUS_TEST_DATA env variable");

  _dbus_string_init_const (&test_data_dir, dir);

#ifdef DBUS_UNIX
  /* close any inherited fds so dbus-spawn's check for close-on-exec works */
  _dbus_close_all ();
#endif

  if (!_dbus_threads_init_debug ())
    _dbus_test_fatal ("OOM initializing debug threads");

  if (only == NULL || strcmp (only, "expire-list") == 0)
    {
      test_pre_hook ();
      _dbus_test_diag ("%s: Running expire list test", argv[0]);
      if (!bus_expire_list_test (&test_data_dir))
        _dbus_test_fatal ("expire list test failed");
      test_post_hook ();
    }

  if (only == NULL || strcmp (only, "config-parser") == 0)
    {
      test_pre_hook ();
      _dbus_test_diag ("%s: Running config file parser test", argv[0]);
      if (!bus_config_parser_test (&test_data_dir))
        _dbus_test_fatal ("parser test failed");
      test_post_hook ();
    }

  if (only == NULL || strcmp (only, "signals") == 0)
    {
      test_pre_hook ();
      _dbus_test_diag ("%s: Running signals test", argv[0]);
      if (!bus_signals_test (&test_data_dir))
        _dbus_test_fatal ("signals test failed");
      test_post_hook ();
    }

  if (only == NULL || strcmp (only, "dispatch-sha1") == 0)
    {
      test_pre_hook ();
      _dbus_test_diag ("%s: Running SHA1 connection test", argv[0]);
      if (!bus_dispatch_sha1_test (&test_data_dir))
        _dbus_test_fatal ("sha1 test failed");
      test_post_hook ();
    }

  if (only == NULL || strcmp (only, "dispatch") == 0)
    {
      test_pre_hook ();
      _dbus_test_diag ("%s: Running message dispatch test", argv[0]);
      if (!bus_dispatch_test (&test_data_dir)) 
        _dbus_test_fatal ("dispatch test failed");
      test_post_hook ();
    }

  if (only == NULL || strcmp (only, "activation-service-reload") == 0)
    {
      test_pre_hook ();
      _dbus_test_diag ("%s: Running service files reloading test", argv[0]);
      if (!bus_activation_service_reload_test (&test_data_dir))
        _dbus_test_fatal ("service reload test failed");
      test_post_hook ();
    }

#ifdef HAVE_UNIX_FD_PASSING
  if (only == NULL || strcmp (only, "unix-fds-passing") == 0)
    {
      test_pre_hook ();
      _dbus_test_diag ("%s: Running unix fd passing test", argv[0]);
      if (!bus_unix_fds_passing_test (&test_data_dir))
        _dbus_test_fatal ("unix fd passing test failed");
      test_post_hook ();
    }
#endif

  _dbus_test_diag ("%s: Success", argv[0]);

  return 0;
}
