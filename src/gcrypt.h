/* gcrypt.h -  GNU digital encryption library interface
 *	Copyright (C) 1998 Free Software Foundation, Inc.
 *
 * This file is part of GnuPG.
 *
 * GnuPG is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * GnuPG is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef _GCRYPT_H
#define _GCRYPT_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef GCRYPT_NO_MPI_MACROS
#define GCRYPT_NO_MPI_MACROS
#endif

#ifndef HAVE_BYTE_TYPEDEF
  #undef byte	    /* maybe there is a macro with this name */
  typedef unsigned char byte;
  #define HAVE_BYTE_TYPEDEF
#endif

struct gcry_mpi;
typedef struct gcry_mpi *GCRY_MPI;

/*******************************************
 *					   *
 *  error handling etc. 		   *
 *					   *
 *******************************************/

/* FIXME: We should use the same values as they were used in GnuPG 1.0.
 *	  gpg --status-fd may print some of these values */
enum {
    GCRYERR_SUCCESS = 0,    /* "no error" */
    GCRYERR_GENERAL = 1,    /* catch all the other errors code */

    GCRYERR_INV_PK_ALGO = 4, /* invalid public key algorithm */
    GCRYERR_INV_MD_ALGO = 5, /* invalid message digest algorithm */
    GCRYERR_BAD_PUBLIC_KEY = 6, /* Bad public key */
    GCRYERR_BAD_SECRET_KEY = 7, /* Bad secret key */
    GCRYERR_BAD_SIGNATURE = 8,	/* Bad signature */

    GCRYERR_INV_CIPHER_ALGO = 12, /* invalid cipher algorithm */
    GCRYERR_BAD_MPI = 30,
    GCRYERR_WRONG_PK_ALGO = 41, /* wrong public key algorithm */
    GCRYERR_WEAK_KEY = 43,  /* weak encryption key */
    GCRYERR_INV_KEYLEN = 44,  /* invalid length of a key*/
    GCRYERR_INV_ARG = 45,    /* invalid argument */
    GCRYERR_SELFTEST = 50,	/* selftest failed */

    /* error codes not used in GnuPG 1.0 */
    GCRYERR_INV_OP = 61,     /* invalid operation code or ctl command */
    GCRYERR_NO_MEM = 62,     /* out of core */
    GCRYERR_INTERNAL = 63,   /* internal error */
    GCRYERR_EOF = 64,	     /* (-1) is remapped to this value */
    GCRYERR_INV_OBJ = 65,    /* an object is not valid */
    GCRYERR_TOO_SHORT = 66,  /* provided buffer too short */
    GCRYERR_TOO_LARGE = 67,  /* object is too large */
    GCRYERR_NO_OBJ = 68,     /* Missign item in an object */
};


int gcry_errno(void);
const char *gcry_strerror( int ec );

enum gcry_ctl_cmds {
    GCRYCTL_SET_KEY  = 1,
    GCRYCTL_SET_IV   = 2,
    GCRYCTL_CFB_SYNC = 3,
    GCRYCTL_RESET    = 4,   /* e.g. for MDs */
    GCRYCTL_FINALIZE = 5,
    GCRYCTL_GET_KEYLEN = 6,
    GCRYCTL_GET_BLKLEN = 7,
    GCRYCTL_TEST_ALGO = 8,
    GCRYCTL_IS_SECURE = 9,
    GCRYCTL_GET_ASNOID = 10,
    GCRYCTL_ENABLE_ALGO = 11,
    GCRYCTL_DISABLE_ALGO = 12,
    GCRYCTL_DUMP_RANDOM_STATS = 13,
    GCRYCTL_DUMP_SECMEM_STATS = 14,
    GCRYCTL_GET_ALGO_NPKEY = 15,
    GCRYCTL_GET_ALGO_NSKEY = 16,
    GCRYCTL_GET_ALGO_NSIGN = 17,
    GCRYCTL_GET_ALGO_NENCR = 18,
};

int gcry_control( enum gcry_ctl_cmds, ... );

enum gcry_random_level {
    GCRY_WEAK_RANDOM = 0,
    GCRY_STRONG_RANDOM = 1,
    GCRY_VERY_STRONG_RANDOM = 2
};


struct gcry_sexp;
typedef struct gcry_sexp *GCRY_SEXP;

enum gcry_sexp_format {
    GCRYSEXP_FMT_DEFAULT   = 0,
    GCRYSEXP_FMT_CANON	   = 1,
    GCRYSEXP_FMT_BASE64    = 2,
    GCRYSEXP_FMT_ADVANCED  = 3,
};


GCRY_SEXP gcry_sexp_new_data( const char *buffer, size_t length );
GCRY_SEXP gcry_sexp_new_mpi( GCRY_MPI mpi );
GCRY_SEXP gcry_sexp_new_name_data( const char *name,
				   const char *buffer, size_t length );
GCRY_SEXP gcry_sexp_new_name_mpi( const char *name, GCRY_MPI mpi );
void	  gcry_sexp_release( GCRY_SEXP sexp );
void	  gcry_sexp_dump( GCRY_SEXP a );
GCRY_SEXP gcry_sexp_cons( GCRY_SEXP a, GCRY_SEXP b );
GCRY_SEXP gcry_sexp_vlist( GCRY_SEXP a, ... );
GCRY_SEXP gcry_sexp_append( GCRY_SEXP a, GCRY_SEXP n );
GCRY_SEXP gcry_sexp_prepend( GCRY_SEXP a, GCRY_SEXP n );
int	  gcry_sexp_sscan( GCRY_SEXP *retsexp, const char *buffer,
			       size_t length, size_t *erroff );
size_t	    gcry_sexp_sprint( GCRY_SEXP sexp, int mode, char *buffer,
						size_t maxlength );
GCRY_SEXP   gcry_sexp_find_token( GCRY_SEXP list,
				  const char *tok, size_t toklen );
GCRY_SEXP   gcry_sexp_car( GCRY_SEXP list );
GCRY_SEXP   gcry_sexp_cdr( GCRY_SEXP list );
const char *gcry_sexp_car_data( GCRY_SEXP list, size_t *datalen );
const char *gcry_sexp_cdr_data( GCRY_SEXP list, size_t *datalen );
GCRY_MPI    gcry_sexp_car_mpi( GCRY_SEXP list, int mpifmt );
GCRY_MPI    gcry_sexp_cdr_mpi( GCRY_SEXP list, int mpifmt );



#ifndef GCRYPT_NO_SEXP_MACROS
#define SEXP		GCRY_SEXP
#define SEXP_NEW(a,b)	gcry_sexp_new_data( (a), (b) )
#define SEXP_RELEASE(a) do { gcry_sexp_release( (a) ); (a)=NULL; } while(0)
#define SEXP_CONS(a,b)	gcry_sexp_cons((a),(b))
#endif /*GCRYPT_NO_SEXP_MACROS*/

/*******************************************
 *					   *
 *  multi precision integer functions	   *
 *					   *
 *******************************************/

enum gcry_mpi_format {
    GCRYMPI_FMT_NONE= 0,
    GCRYMPI_FMT_STD = 1,    /* twos complement stored without length */
    GCRYMPI_FMT_PGP = 2,    /* As used by OpenPGP */
    GCRYMPI_FMT_SSH = 3,    /* As used by SSH (same as 0 but with length)*/
    GCRYMPI_FMT_HEX = 4,    /* hex format */
    GCRYMPI_FMT_USG = 5,    /* like STD but this is an unsigned one */
};



GCRY_MPI gcry_mpi_new( unsigned int nbits );
GCRY_MPI gcry_mpi_snew( unsigned int nbits );
void	 gcry_mpi_release( GCRY_MPI a );
GCRY_MPI gcry_mpi_copy( const GCRY_MPI a );
GCRY_MPI gcry_mpi_set( GCRY_MPI w, const GCRY_MPI u );
GCRY_MPI gcry_mpi_set_ui( GCRY_MPI w, unsigned long u );
int	 gcry_mpi_cmp( const GCRY_MPI u, const GCRY_MPI v );
int	 gcry_mpi_cmp_ui( const GCRY_MPI u, unsigned long v );
void	 gcry_mpi_randomize( GCRY_MPI w,
			     unsigned int nbits, enum gcry_random_level level);
int	 gcry_mpi_scan( GCRY_MPI *ret_mpi, enum gcry_mpi_format format,
				       const char *buffer, size_t *nbytes );
int	 gcry_mpi_print( enum gcry_mpi_format format,
			 char *buffer, size_t *nbytes, const GCRY_MPI a );

void gcry_mpi_powm( GCRY_MPI w,
		    const GCRY_MPI b, const GCRY_MPI e, const GCRY_MPI m );


#ifndef GCRYPT_NO_MPI_MACROS
#define mpi_new(n)	    gcry_mpi_new( (n) )
#define mpi_secure_new( n ) gcry_mpi_snew( (n) )
#define mpi_release( a )    do { gcry_mpi_release( (a) ); \
				 (a) = NULL; } while(0)
#define mpi_copy( a )	    gcry_mpi_copy( (a) )
#define mpi_set( w, u)	    gcry_mpi_set( (w), (u) )
#define mpi_set_ui( w, u)   gcry_mpi_set_ui( (w), (u) )
#define mpi_cmp( u, v )     gcry_mpi_cmp( (u), (v) )
#define mpi_cmp_ui( u, v )  gcry_mpi_cmp_ui( (u), (v) )

#define mpi_powm(w,b,e,m)   gcry_mpi_powm( (w), (b), (e), (m) )
#endif /* GCRYPT_NO_MPI_MACROS */

/********************************************
 *******  symmetric cipher functions  *******
 ********************************************/

struct gcry_cipher_handle;
typedef struct gcry_cipher_handle *GCRY_CIPHER_HD;

enum gcry_cipher_algos {
    GCRY_CIPHER_NONE	    = 0,
    GCRY_CIPHER_IDEA	    = 1,
    GCRY_CIPHER_3DES	    = 2,
    GCRY_CIPHER_CAST5	    = 3,
    GCRY_CIPHER_BLOWFISH    = 4,
    GCRY_CIPHER_SAFER_SK128 = 5,
    GCRY_CIPHER_DES_SK	    = 6,
    GCRY_CIPHER_TWOFISH     = 10,
};

enum gcry_cipher_modes {
    GCRY_CIPHER_MODE_NONE   = 0,
    GCRY_CIPHER_MODE_ECB    = 1,
    GCRY_CIPHER_MODE_CFB    = 2,
    GCRY_CIPHER_MODE_CBC    = 3,
};

enum gcry_cipher_flags {
    GCRY_CIPHER_SECURE	    = 1,  /* allocate in secure memory */
    GCRY_CIPHER_ENABLE_SYNC = 2,  /* enable CFB sync mode */
};


GCRY_CIPHER_HD gcry_cipher_open( int algo, int mode, unsigned flags);
void gcry_cipher_close( GCRY_CIPHER_HD h );
int  gcry_cipher_ctl( GCRY_CIPHER_HD h, int cmd, void *buffer, size_t buflen);
int gcry_cipher_info( GCRY_CIPHER_HD h, int what, void *buffer, size_t *nbytes);
int gcry_cipher_algo_info( int algo, int what, void *buffer, size_t *nbytes);
const char *gcry_cipher_algo_name( int algo );
int gcry_cipher_map_name( const char* name );

int gcry_cipher_encrypt( GCRY_CIPHER_HD h, byte *out, size_t outsize,
				      const byte *in, size_t inlen );
int gcry_cipher_decrypt( GCRY_CIPHER_HD h, byte *out, size_t outsize,
				      const byte *in, size_t inlen );


/* some handy macros */
#define gcry_cipher_setkey(h,k,l)  gcry_cipher_ctl( (h), GCRYCTL_SET_KEY, \
								  (k), (l) )
#define gcry_cipher_setiv(h,k,l)  gcry_cipher_ctl( (h), GCRYCTL_SET_IV, \
								  (k), (l) )
#define gcry_cipher_sync(h)  gcry_cipher_ctl( (h), GCRYCTL_CFB_SYNC, \
								   NULL, 0 )

#define gcry_cipher_get_algo_keylen(a) \
	    gcry_cipher_algo_info( (a), GCRYCTL_GET_KEYLEN, NULL, NULL )
#define gcry_cipher_get_algo_blklen(a) \
	    gcry_cipher_algo_info( (a), GCRYCTL_GET_BLKLEN, NULL, NULL )
#define gcry_cipher_test_algo(a) \
	    gcry_cipher_algo_info( (a), GCRYCTL_TEST_ALGO, NULL, NULL )


/*********************************************
 *******  asymmetric cipher functions  *******
 *********************************************/

enum gcry_pk_algos {
    GCRY_PK_RSA = 1,
    GCRY_PK_RSA_E = 2,	    /* use only for OpenPGP */
    GCRY_PK_RSA_S = 3,	    /* use only for OpenPGP */
    GCRY_PK_ELG_E = 16,     /* use only for OpenPGP */
    GCRY_PK_DSA   = 17,
    GCRY_PK_ELG   = 20,
};

/* Flags describing usage capabilites/request of a PK algorithm */
#define GCRY_PK_USAGE_SIGN 1
#define GCRY_PK_USAGE_ENCR 2

int gcry_pk_encrypt( GCRY_SEXP *result, GCRY_SEXP data, GCRY_SEXP pkey );
int gcry_pk_decrypt( GCRY_SEXP *result, GCRY_SEXP data, GCRY_SEXP skey );
int gcry_pk_sign(    GCRY_SEXP *result, GCRY_SEXP data, GCRY_SEXP skey );
int gcry_pk_verify(  GCRY_SEXP sigval, GCRY_SEXP data, GCRY_SEXP pkey );

int gcry_pk_ctl( int cmd, void *buffer, size_t buflen);
int gcry_pk_algo_info( int algo, int what, void *buffer, size_t *nbytes);
const char *gcry_pk_algo_name( int algo );
int gcry_pk_map_name( const char* name );


#define gcry_pk_test_algo(a) \
	    gcry_pk_algo_info( (a), GCRYCTL_TEST_ALGO, NULL, NULL )

/*********************************************
 *******  cryptograhic hash functions  *******
 *********************************************/

enum gcry_md_algos {
    GCRY_MD_NONE    = 0,
    GCRY_MD_MD5     = 1,
    GCRY_MD_SHA1    = 2,
    GCRY_MD_RMD160  = 3,
    GCRY_MD_TIGER   = 6
};

enum gcry_md_flags {
    GCRY_MD_FLAG_SECURE = 1
};


struct gcry_md_context;
struct gcry_md_handle {
    struct gcry_md_context *ctx;
    int  bufpos;
    int  bufsize;
    byte buf[1];
};
typedef struct gcry_md_handle *GCRY_MD_HD;


GCRY_MD_HD gcry_md_open( int algo, unsigned flags );
void gcry_md_close( GCRY_MD_HD hd );
int gcry_md_enable( GCRY_MD_HD hd, int algo );
GCRY_MD_HD gcry_md_copy( GCRY_MD_HD hd );
void gcry_md_reset( GCRY_MD_HD hd );
int gcry_md_ctl( GCRY_MD_HD hd, int cmd, byte *buffer, size_t buflen);
void gcry_md_write( GCRY_MD_HD hd, const byte *buffer, size_t length);
byte *gcry_md_read( GCRY_MD_HD hd, int algo );
void gcry_md_hash_buffer( int algo, char *digest,
			  const char *buffer, size_t length);
int gcry_md_get_algo( GCRY_MD_HD hd );
unsigned int gcry_md_get_algo_dlen( int algo );
/*??int gcry_md_get( GCRY_MD_HD hd, int algo, byte *buffer, int buflen );*/
int gcry_md_info( GCRY_MD_HD h, int what, void *buffer, size_t *nbytes);
int gcry_md_algo_info( int algo, int what, void *buffer, size_t *nbytes);
const char *gcry_md_algo_name( int algo );
int gcry_md_map_name( const char* name );

#define gcry_md_putc(h,c)  \
	    do {					\
		if( (h)->bufpos == (h)->bufsize )	\
		    gcry_md_write( (h), NULL, 0 );	\
		(h)->buf[(h)->bufpos++] = (c) & 0xff;	\
	    } while(0)

#define gcry_md_final(a) \
	    gcry_md_ctl( (a), GCRYCTL_FINALIZE, NULL, 0 )

#define gcry_md_is_secure(a) \
	    gcry_md_info( (a), GCRYCTL_IS_SECURE, NULL, NULL )

#define gcry_md_test_algo(a) \
	    gcry_md_algo_info( (a), GCRYCTL_TEST_ALGO, NULL, NULL )

/*****************************************
 *******  miscellaneous stuff	**********
 *****************************************/

void gcry_randomize( byte *buffer, size_t length,
		     enum gcry_random_level level );
void *gcry_random_bytes( size_t nbytes, enum gcry_random_level level );
void *gcry_random_bytes_secure( size_t nbytes, enum gcry_random_level level );

/* Provide custom functions for special tasks of libgcrypt.
 */
void gcry_set_allocation_handler( void *(*new_alloc_func)(size_t n),
				  void *(*new_alloc_secure_func)(size_t n),
				  int (*new_is_secure_func)(const void*),
				  void *(*new_realloc_func)(void *p, size_t n),
				  void (*new_free_func)(void*) );
void gcry_set_outofcore_handler( int (*h)( void*, size_t, unsigned int ),
								void *opaque );
void gcry_set_fatalerror_handler( void (*fnc)(void*,int, const char*),
								void *opaque );
void gcry_set_gettext_handler( const char *(*f)(const char*) );


/* Access to the memory function of libgcrypt.
 * Especially the gcry_free() should be used for memory
 * allocated by gcry_ functions.
 */
void *gcry_malloc( size_t n );
void *gcry_calloc( size_t n, size_t m );
void *gcry_malloc_secure( size_t n );
void *gcry_calloc_secure( size_t n, size_t m );
void *gcry_realloc( void *a, size_t n );
void *gcry_xmalloc( size_t n );
void *gcry_xcalloc( size_t n, size_t m );
void *gcry_xmalloc_secure( size_t n );
void *gcry_xcalloc_secure( size_t n, size_t m );
void *gcry_xrealloc( void *a, size_t n );
char *gcry_xstrdup( const char * a);
void  gcry_free( void *p );



#ifndef GCRYPT_NO_MPI_MACROS
  typedef struct gcry_mpi *MPI;
#endif /* GCRYPT_NO_MPI_MACROS */

#ifdef __cplusplus
}
#endif
#endif /* _GCRYPT_H */
