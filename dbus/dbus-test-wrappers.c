/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */
/*
 * Copyright 2002-2009 Red Hat Inc.
 * Copyright 2011-2018 Collabora Ltd.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <config.h>
#include "dbus/dbus-test-wrappers.h"

#ifndef DBUS_ENABLE_EMBEDDED_TESTS
#error This file is only relevant for the embedded tests
#endif

#include "dbus/dbus-message-internal.h"
#include "dbus/dbus-test-tap.h"

#if HAVE_LOCALE_H
#include <locale.h>
#endif

#ifdef DBUS_UNIX
# include <dbus/dbus-sysdeps-unix.h>
#endif

/*
 * _dbus_test_main:
 * @argc: number of command-line arguments
 * @argv: array of @argc arguments
 * @n_tests: length of @tests
 * @tests: array of @n_tests tests
 * @flags: flags affecting all tests
 * @test_pre_hook: if not %NULL, called before each test
 * @test_post_hook: if not %NULL, called after each test
 *
 * Wrapper for dbus tests that do not use GLib. Processing of @tests
 * can be terminated early by an entry with @name = NULL, which is a
 * convenient way to put a trailing comma on every "real" test entry
 * without breaking compilation on pedantic C compilers.
 */
int
_dbus_test_main (int                  argc,
                 char               **argv,
                 size_t               n_tests,
                 const DBusTestCase  *tests,
                 DBusTestFlags        flags,
                 void               (*test_pre_hook) (void),
                 void               (*test_post_hook) (void))
{
  const char *test_data_dir;
  const char *specific_test;
  size_t i;

#ifdef DBUS_UNIX
  /* close any inherited fds so dbus-spawn's check for close-on-exec works */
  _dbus_close_all ();
#endif

#if HAVE_SETLOCALE
  setlocale(LC_ALL, "");
#endif

  if (argc > 1 && strcmp (argv[1], "--tap") != 0)
    test_data_dir = argv[1];
  else
    test_data_dir = _dbus_getenv ("DBUS_TEST_DATA");

  if (test_data_dir != NULL)
    _dbus_test_diag ("Test data in %s", test_data_dir);
  else if (flags & DBUS_TEST_FLAGS_REQUIRE_DATA)
    _dbus_test_fatal ("Must specify test data directory as argv[1] or "
                      "in DBUS_TEST_DATA environment variable");
  else
    _dbus_test_diag ("No test data!");

  if (argc > 2)
    specific_test = argv[2];
  else
    specific_test = _dbus_getenv ("DBUS_TEST_ONLY");

  for (i = 0; i < n_tests; i++)
    {
      long before, after;
      DBusInitialFDs *initial_fds = NULL;

      if (tests[i].name == NULL)
        break;

      if (n_tests > 1 &&
          specific_test != NULL &&
          strcmp (specific_test, tests[i].name) != 0)
        {
          _dbus_test_skip ("%s - Only intending to run %s",
                           tests[i].name, specific_test);
          continue;
        }

      _dbus_test_diag ("Running test: %s", tests[i].name);
      _dbus_get_monotonic_time (&before, NULL);

      if (test_pre_hook)
        test_pre_hook ();

      if (flags & DBUS_TEST_FLAGS_CHECK_FD_LEAKS)
        initial_fds = _dbus_check_fdleaks_enter ();

      if (tests[i].func (test_data_dir))
        _dbus_test_ok ("%s", tests[i].name);
      else
        _dbus_test_not_ok ("%s", tests[i].name);

      _dbus_get_monotonic_time (&after, NULL);

      _dbus_test_diag ("%s test took %ld seconds",
                       tests[i].name, after - before);

      if (test_post_hook)
        test_post_hook ();

      if (flags & DBUS_TEST_FLAGS_CHECK_MEMORY_LEAKS)
        _dbus_test_check_memleaks (tests[i].name);

      if (flags & DBUS_TEST_FLAGS_CHECK_FD_LEAKS)
        _dbus_check_fdleaks_leave (initial_fds);
    }

  return _dbus_test_done_testing ();
}
