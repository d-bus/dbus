/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */
/*
 * Copyright 2002-2011 Red Hat, Inc.
 * Copyright 2006 Julio M. Merino Vidal
 * Copyright 2006 Ralf Habacker
 * Copyright 2011-2018 Collabora Ltd.
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

#include <dbus/dbus.h>
#include "dbus/dbus-internals.h"
#include "dbus/dbus-test.h"
#include "dbus/dbus-test-tap.h"
#include "test/test-utils.h"

#include "misc-internals.h"

/**
 * @ingroup DBusSignatureInternals
 * Unit test for DBusSignature.
 *
 * @returns #TRUE on success.
 */
static dbus_bool_t
_dbus_signature_test (const char *test_data_dir _DBUS_GNUC_UNUSED)
{
  DBusSignatureIter iter;
  DBusSignatureIter subiter;
  DBusSignatureIter subsubiter;
  DBusSignatureIter subsubsubiter;
  const char *sig;
  dbus_bool_t boolres;

  sig = "";
  _dbus_assert (dbus_signature_validate (sig, NULL));
  _dbus_assert (!dbus_signature_validate_single (sig, NULL));
  dbus_signature_iter_init (&iter, sig);
  _dbus_assert (dbus_signature_iter_get_current_type (&iter) == DBUS_TYPE_INVALID);

  sig = DBUS_TYPE_STRING_AS_STRING;
  _dbus_assert (dbus_signature_validate (sig, NULL));
  _dbus_assert (dbus_signature_validate_single (sig, NULL));
  dbus_signature_iter_init (&iter, sig);
  _dbus_assert (dbus_signature_iter_get_current_type (&iter) == DBUS_TYPE_STRING);

  sig = DBUS_TYPE_STRING_AS_STRING DBUS_TYPE_BYTE_AS_STRING;
  _dbus_assert (dbus_signature_validate (sig, NULL));
  dbus_signature_iter_init (&iter, sig);
  _dbus_assert (dbus_signature_iter_get_current_type (&iter) == DBUS_TYPE_STRING);
  boolres = dbus_signature_iter_next (&iter);
  _dbus_assert (boolres);
  _dbus_assert (dbus_signature_iter_get_current_type (&iter) == DBUS_TYPE_BYTE);

  sig = DBUS_TYPE_UINT16_AS_STRING
    DBUS_STRUCT_BEGIN_CHAR_AS_STRING
    DBUS_TYPE_STRING_AS_STRING
    DBUS_TYPE_UINT32_AS_STRING
    DBUS_TYPE_VARIANT_AS_STRING
    DBUS_TYPE_DOUBLE_AS_STRING
    DBUS_STRUCT_END_CHAR_AS_STRING;
  _dbus_assert (dbus_signature_validate (sig, NULL));
  dbus_signature_iter_init (&iter, sig);
  _dbus_assert (dbus_signature_iter_get_current_type (&iter) == DBUS_TYPE_UINT16);
  boolres = dbus_signature_iter_next (&iter);
  _dbus_assert (boolres);
  _dbus_assert (dbus_signature_iter_get_current_type (&iter) == DBUS_TYPE_STRUCT);
  dbus_signature_iter_recurse (&iter, &subiter);
  _dbus_assert (dbus_signature_iter_get_current_type (&subiter) == DBUS_TYPE_STRING);
  boolres = dbus_signature_iter_next (&subiter);
  _dbus_assert (boolres);
  _dbus_assert (dbus_signature_iter_get_current_type (&subiter) == DBUS_TYPE_UINT32);
  boolres = dbus_signature_iter_next (&subiter);
  _dbus_assert (boolres);
  _dbus_assert (dbus_signature_iter_get_current_type (&subiter) == DBUS_TYPE_VARIANT);
  boolres = dbus_signature_iter_next (&subiter);
  _dbus_assert (boolres);
  _dbus_assert (dbus_signature_iter_get_current_type (&subiter) == DBUS_TYPE_DOUBLE);

  sig = DBUS_TYPE_UINT16_AS_STRING
    DBUS_STRUCT_BEGIN_CHAR_AS_STRING
    DBUS_TYPE_UINT32_AS_STRING
    DBUS_TYPE_BYTE_AS_STRING
    DBUS_TYPE_ARRAY_AS_STRING
    DBUS_TYPE_ARRAY_AS_STRING
    DBUS_TYPE_DOUBLE_AS_STRING
    DBUS_STRUCT_BEGIN_CHAR_AS_STRING
    DBUS_TYPE_BYTE_AS_STRING
    DBUS_STRUCT_END_CHAR_AS_STRING
    DBUS_STRUCT_END_CHAR_AS_STRING;
  _dbus_assert (dbus_signature_validate (sig, NULL));
  dbus_signature_iter_init (&iter, sig);
  _dbus_assert (dbus_signature_iter_get_current_type (&iter) == DBUS_TYPE_UINT16);
  boolres = dbus_signature_iter_next (&iter);
  _dbus_assert (boolres);
  _dbus_assert (dbus_signature_iter_get_current_type (&iter) == DBUS_TYPE_STRUCT);
  dbus_signature_iter_recurse (&iter, &subiter);
  _dbus_assert (dbus_signature_iter_get_current_type (&subiter) == DBUS_TYPE_UINT32);
  boolres = dbus_signature_iter_next (&subiter);
  _dbus_assert (boolres);
  _dbus_assert (dbus_signature_iter_get_current_type (&subiter) == DBUS_TYPE_BYTE);
  boolres = dbus_signature_iter_next (&subiter);
  _dbus_assert (boolres);
  _dbus_assert (dbus_signature_iter_get_current_type (&subiter) == DBUS_TYPE_ARRAY);
  _dbus_assert (dbus_signature_iter_get_element_type (&subiter) == DBUS_TYPE_ARRAY);

  dbus_signature_iter_recurse (&subiter, &subsubiter);
  _dbus_assert (dbus_signature_iter_get_current_type (&subsubiter) == DBUS_TYPE_ARRAY);
  _dbus_assert (dbus_signature_iter_get_element_type (&subsubiter) == DBUS_TYPE_DOUBLE);

  dbus_signature_iter_recurse (&subsubiter, &subsubsubiter);
  _dbus_assert (dbus_signature_iter_get_current_type (&subsubsubiter) == DBUS_TYPE_DOUBLE);
  boolres = dbus_signature_iter_next (&subiter);
  _dbus_assert (boolres);
  _dbus_assert (dbus_signature_iter_get_current_type (&subiter) == DBUS_TYPE_STRUCT);
  dbus_signature_iter_recurse (&subiter, &subsubiter);
  _dbus_assert (dbus_signature_iter_get_current_type (&subsubiter) == DBUS_TYPE_BYTE);

  sig = DBUS_TYPE_ARRAY_AS_STRING
    DBUS_DICT_ENTRY_BEGIN_CHAR_AS_STRING
    DBUS_TYPE_INT16_AS_STRING
    DBUS_TYPE_STRING_AS_STRING
    DBUS_DICT_ENTRY_END_CHAR_AS_STRING
    DBUS_TYPE_VARIANT_AS_STRING;
  _dbus_assert (dbus_signature_validate (sig, NULL));
  _dbus_assert (!dbus_signature_validate_single (sig, NULL));
  dbus_signature_iter_init (&iter, sig);
  _dbus_assert (dbus_signature_iter_get_current_type (&iter) == DBUS_TYPE_ARRAY);
  _dbus_assert (dbus_signature_iter_get_element_type (&iter) == DBUS_TYPE_DICT_ENTRY);

  dbus_signature_iter_recurse (&iter, &subiter);
  dbus_signature_iter_recurse (&subiter, &subsubiter);
  _dbus_assert (dbus_signature_iter_get_current_type (&subsubiter) == DBUS_TYPE_INT16);
  boolres = dbus_signature_iter_next (&subsubiter);
  _dbus_assert (boolres);
  _dbus_assert (dbus_signature_iter_get_current_type (&subsubiter) == DBUS_TYPE_STRING);
  boolres = dbus_signature_iter_next (&subsubiter);
  _dbus_assert (!boolres);

  boolres = dbus_signature_iter_next (&iter);
  _dbus_assert (boolres);
  _dbus_assert (dbus_signature_iter_get_current_type (&iter) == DBUS_TYPE_VARIANT);
  boolres = dbus_signature_iter_next (&iter);
  _dbus_assert (!boolres);

  sig = DBUS_TYPE_DICT_ENTRY_AS_STRING;
  _dbus_assert (!dbus_signature_validate (sig, NULL));

  sig = DBUS_TYPE_ARRAY_AS_STRING;
  _dbus_assert (!dbus_signature_validate (sig, NULL));

  sig = DBUS_TYPE_UINT32_AS_STRING
    DBUS_TYPE_ARRAY_AS_STRING;
  _dbus_assert (!dbus_signature_validate (sig, NULL));

  sig = DBUS_TYPE_ARRAY_AS_STRING
    DBUS_TYPE_DICT_ENTRY_AS_STRING;
  _dbus_assert (!dbus_signature_validate (sig, NULL));

  sig = DBUS_DICT_ENTRY_BEGIN_CHAR_AS_STRING;
  _dbus_assert (!dbus_signature_validate (sig, NULL));

  sig = DBUS_DICT_ENTRY_END_CHAR_AS_STRING;
  _dbus_assert (!dbus_signature_validate (sig, NULL));

  sig = DBUS_DICT_ENTRY_BEGIN_CHAR_AS_STRING
    DBUS_TYPE_INT32_AS_STRING;
  _dbus_assert (!dbus_signature_validate (sig, NULL));

  sig = DBUS_DICT_ENTRY_BEGIN_CHAR_AS_STRING
    DBUS_TYPE_INT32_AS_STRING
    DBUS_TYPE_STRING_AS_STRING;
  _dbus_assert (!dbus_signature_validate (sig, NULL));

  sig = DBUS_STRUCT_END_CHAR_AS_STRING
    DBUS_STRUCT_BEGIN_CHAR_AS_STRING;
  _dbus_assert (!dbus_signature_validate (sig, NULL));

  sig = DBUS_STRUCT_BEGIN_CHAR_AS_STRING
    DBUS_TYPE_BOOLEAN_AS_STRING;
  _dbus_assert (!dbus_signature_validate (sig, NULL));
  return TRUE;
#if 0
 oom:
  _dbus_test_fatal ("out of memory");
  return FALSE;
#endif
}

static DBusTestCase tests[] =
{
  { "string", _dbus_string_test },
  { "sysdeps", _dbus_sysdeps_test },
  { "data-slot", _dbus_data_slot_test },
  { "misc", _dbus_misc_test },
  { "address", _dbus_address_test },
  { "server", _dbus_server_test },
  { "object-tree", _dbus_object_tree_test },
  { "signature", _dbus_signature_test },
  { "marshalling", _dbus_marshal_test },
  { "byteswap", _dbus_marshal_byteswap_test },
  { "memory", _dbus_memory_test },
  { "mem-pool", _dbus_mem_pool_test },
  { "list", _dbus_list_test },
  { "marshal-validate", _dbus_marshal_validate_test },
  { "credentials", _dbus_credentials_test },
  { "keyring", _dbus_keyring_test },
  { "sha", _dbus_sha_test },
  { "auth", _dbus_auth_test },

#if defined(DBUS_UNIX)
  { "userdb", _dbus_userdb_test },
  { "transport-unix", _dbus_transport_unix_test },
#endif

  { NULL }
};

int
main (int    argc,
      char **argv)
{
  return _dbus_test_main (argc, argv, _DBUS_N_ELEMENTS (tests), tests,
                          DBUS_TEST_FLAGS_CHECK_MEMORY_LEAKS,
                          NULL, NULL);
}
