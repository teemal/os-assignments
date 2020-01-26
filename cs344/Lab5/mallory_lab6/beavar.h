/*
 * R Jesse Chaney
 *
 */

#pragma once

# include <string.h>
# include <sys/cdefs.h>

/*
 * beavar archive files start with the BEAVAR_ID identifying string. Then follows a
 * 'struct beavar_hdr_s', and as many bytes of member file data as its 'beavar_size'
 * member indicates, for each member file.
 */

# define BEAVAR_ID	"!<beavar>\n"    /* String that begins an beavar archive file. */
# define BEAVAR_ID_LEN	10            /* Size of the beavar id string string. */

# define BEAVAR_HDR_END	"=\n"		/* String at end of each member header. */
# define BEAVAR_HDR_END_LEN  2

# define BEAVAR_MAX_FILE_NAME_LEN       30
# define BEAVAR_FNAME_LEN_SIZE           2
# define BEAVAR_DATE_SIZE	            10
# define BEAVAR_UGID_SIZE	             6
# define BEAVAR_MODE_SIZE	             7
# define BEAVAR_FILE_SIZE	            12
# define BEAVAR_MAX_MEMBER_FILE_SIZE    1000000 /* Just make things a little easier. */

// Some exit() codes.
# define BEAVAR_FILE_COULD_NOT_OPEN     2
# define BEAVAR_FILE_CORRUPT            3
# define BEAVAR_FILE_MEMBER_BAD         4
# define BEAVAR_FILE_AR_WRITE_FAILED    5
# define BEAVAR_FILE_MEM_FILE_OPEN_FAIL 6
# define BEAVAR_FILE_MEM_FILE_READ_FAIL 7
# define BEAVAR_FILE_AR_FILE_WRITE_FAIL 8

typedef struct beavar_hdr_s
{
    char beavar_fname[BEAVAR_MAX_FILE_NAME_LEN];   /* Member file name, may not be NULL terminated. */
    char beavar_fname_len[BEAVAR_FNAME_LEN_SIZE];  /* The length of the member file name */

    char beavar_adate[BEAVAR_DATE_SIZE];	       /* File access date, decimal sec since Epoch. */
    char beavar_mdate[BEAVAR_DATE_SIZE];	       /* File modify date, decimal sec since Epoch. */

    char beavar_uid[BEAVAR_UGID_SIZE];	           /* user id in ASCII decimal */
    char beavar_gid[BEAVAR_UGID_SIZE];             /* group id in ASCII decimal. */

    char beavar_mode[BEAVAR_MODE_SIZE];	           /* File mode, in ASCII octal. */
    char beavar_size[BEAVAR_FILE_SIZE];	           /* File size, in ASCII decimal. */

    char beavar_hdr_end[BEAVAR_HDR_END_LEN];       /* Always contains BEAVAR_HDR_END. */
} beavar_hdr_t;
