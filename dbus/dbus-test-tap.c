/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */
/* dbus-test-tap — TAP helpers for "embedded tests"
 *
 * Copyright © 2017 Collabora Ltd.
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
#include "dbus/dbus-test-tap.h"

/*
 * TAP, the Test Anything Protocol, is a text-based syntax for test-cases
 * to report results to test harnesses.
 *
 * See <http://testanything.org/> for details of the syntax, which
 * will not be explained here.
 */

#ifdef DBUS_ENABLE_EMBEDDED_TESTS

#include <stdio.h>
#include <stdlib.h>

/*
 * Output TAP indicating a fatal error, and exit unsuccessfully.
 */
void
_dbus_test_fatal (const char *format,
    ...)
{
  va_list ap;

  printf ("Bail out! ");
  va_start (ap, format);
  vprintf (format, ap);
  va_end (ap);
  printf ("\n");
  fflush (stdout);
  exit (1);
}

/*
 * Output TAP indicating a diagnostic (informational message).
 */
void
_dbus_test_diag (const char *format,
    ...)
{
  va_list ap;

  printf ("# ");
  va_start (ap, format);
  vprintf (format, ap);
  va_end (ap);
  printf ("\n");
}

#endif
