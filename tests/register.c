/* register.c - Test for registering of additional cipher modules.
 *	Copyright (C) 2001, 2002, 2003 Free Software Foundation, Inc.
 *
 * This file is part of Libgcrypt.
 *
 * Libgcrypt is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * Libgcrypt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../src/gcrypt.h"

static int verbose;

static void
die (const char *format, ...)
{
  va_list arg_ptr ;

  va_start( arg_ptr, format ) ;
  vfprintf (stderr, format, arg_ptr );
  va_end(arg_ptr);
  exit (1);
}

gcry_err_code_t
foo_setkey (void *c, const unsigned char *key, unsigned keylen)
{
  return 0;
}

#define FOO_BLOCKSIZE 16

void
foo_encrypt (void *c, unsigned char *outbuf, const unsigned char *inbuf)
{
  int i;

  for (i = 0; i < FOO_BLOCKSIZE; i++)
    outbuf[i] = inbuf[i] ^ 0x42;
}

void
foo_decrypt (void *c, unsigned char *outbuf, const unsigned char *inbuf)
{
  int i;

  for (i = 0; i < FOO_BLOCKSIZE; i++)
    outbuf[i] = inbuf[i] ^ 0x42;
}

gcry_cipher_spec_t cipher_spec_foo =
  {
    "FOO", NULL, 16, 0, 0,
    foo_setkey, foo_encrypt, foo_decrypt,
    NULL, NULL,
  };

int
check_list (int algorithm)
{
  gcry_error_t err = GPG_ERR_NO_ERROR;
  int *list, list_length;
  int i, ret = 0;

  err = gcry_cipher_list (NULL, &list_length);
  assert (! err);
  list = malloc (sizeof (int) * list_length);
  assert (list);
  err = gcry_cipher_list (list, &list_length);
  
  for (i = 0; i < list_length && (! ret); i++)
    if (list[i] == algorithm)
      ret = 1;

  return ret;
}

void
check_run (void)
{
  int err, algorithm;
  gcry_cipher_hd_t h;
  char plain[16] = "Heil Discordia!";
  char encrypted[16], decrypted[16];
  gcry_module_t module;
  int ret;

  err = gcry_cipher_register (&cipher_spec_foo, &algorithm, &module);
  assert (! err);

  err = gcry_cipher_open (&h, algorithm, GCRY_CIPHER_MODE_CBC, 0);
  assert (! err);

  err = gcry_cipher_encrypt (h,
			     (unsigned char *) encrypted, sizeof (encrypted),
			     (unsigned char *) plain, sizeof (plain));
  assert (! err);
  assert (memcmp ((void *) plain, (void *) encrypted, sizeof (plain)));

  err = gcry_cipher_reset (h);
  assert (! err);

  err = gcry_cipher_decrypt (h,
			     (unsigned char *) decrypted, sizeof (decrypted),
			     (unsigned char *) encrypted, sizeof (encrypted));
  assert (! err);
  assert (! memcmp ((void *) plain, (void *) decrypted, sizeof (plain)));

  ret = check_list (algorithm);
  assert (ret);

  gcry_cipher_close (h);

  gcry_cipher_unregister (module);

  ret = check_list (algorithm);
  assert (! ret);
}

int
main (int argc, char **argv)
{
  int debug = 0;
  int i = 1;

  if (argc > 1 && !strcmp (argv[1], "--verbose"))
    verbose = 1;
  else if (argc > 1 && !strcmp (argv[1], "--debug"))
    verbose = debug = 1;

  gcry_control (GCRYCTL_DISABLE_SECMEM, 0);
  if (!gcry_check_version (GCRYPT_VERSION))
    die ("version mismatch\n");
  gcry_control (GCRYCTL_INITIALIZATION_FINISHED, 0);
  if (debug)
    gcry_control (GCRYCTL_SET_DEBUG_FLAGS, 1u , 0);

  for (; i > 0; i--)
    check_run ();
  
  return 0;
}
