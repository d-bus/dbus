/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */
/*
 * Copyright © 2017 Collabora Ltd.
 *
 * SPDX-License-Identifier: MIT
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

#ifndef DBUS_TEST_WRAPPERS_H
#define DBUS_TEST_WRAPPERS_H

#include <dbus/dbus-types.h>

typedef struct
{
  const char *name;
  dbus_bool_t (*func) (const char *test_data_dir);
} DBusTestCase;

typedef enum
{
  DBUS_TEST_FLAGS_REQUIRE_DATA = (1 << 0),
  DBUS_TEST_FLAGS_CHECK_MEMORY_LEAKS = (1 << 1),
  DBUS_TEST_FLAGS_CHECK_FD_LEAKS = (1 << 2),
  DBUS_TEST_FLAGS_NONE = 0
} DBusTestFlags;

int _dbus_test_main (int                  argc,
                     char               **argv,
                     size_t               n_tests,
                     const DBusTestCase  *tests,
                     DBusTestFlags        flags,
                     void               (*test_pre_hook) (void),
                     void               (*test_post_hook) (void));

#endif
