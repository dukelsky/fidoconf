/******************************************************************************
 * FIDOCONFIG --- library for fidonet configs
 ******************************************************************************
 * Copyright (C) 1998-1999
 *
 * Matthias Tichy
 *
 * Fido:     2:2433/1245 2:2433/1247 2:2432/605.14
 * Internet: mtt@tichy.de
 *
 * Grimmestr. 12         Buchholzer Weg 4
 * 33098 Paderborn       40472 Duesseldorf
 * Germany               Germany
 *
 * This file is part of FIDOCONFIG.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; see file COPYING. If not, write to the Free
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *****************************************************************************
 * $Id$
 */

#ifndef _COMMON_H
#define _COMMON_H

#include "typesize.h"
#include "fidoconf.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SIZE_aka2str 24   /* Size of static variable returned by aka2str() */

#define nfree(a) { if (a != NULL) { free(a); a = NULL; } }

/* common functions */

FCONF_EXT int copyString(char *str, char **pmem);
/*DOC
 * Copy string from *str to *pmem, allocating memory as needed
   NOTE! *pmem must be NULL, if not NULL, it will be free().
 */

int copyStringUntilSep(char *str, char *seps, char **dest);
/*DOC
 * Copy chars from str to *dest until one of the chars in seps appears
 * memory is allocated as needed
 * *dest will be freed if non-NULL
 * returns number of chars copied
 */

FCONF_EXT void *memdup(void *p, size_t size);
/*DOC
 * Input:  a chunk of memory, with size bytes
 * Output: a copy of it
 */	

FCONF_EXT long str2attr(const char *str);
/*DOC
  Input:  Msg flag name
  Output: value with corresponding bit set or -1L else
*/

FCONF_EXT int  addrComp(const s_addr a1, const s_addr a2);
/*DOC
  Input:  two addresses
  Output: 0, or !0
  FZ:     0 ist returned if the two addresses are the same, !0 else
*/

FCONF_EXT char *strrstr(const char *HAYSTACK, const char *NEEDLE);

/*DOC
  Input:  two constant null-terminated strings
  Output: NULL or the point to a null-terminated string
  FZ:     finds the LAST occurence of NEEDLE in HAYSTACK
          (same as strstr but last occurence
*/

FCONF_EXT char *fc_stristr(const char *str, const char *find);
/*
 * Find the first occurrence of find in s ignoring case
 */


FCONF_EXT void string2addr(const char *string, s_addr *addr);
/*DOC
  Input:  string is an \0-terminated array of chars. is a pointer to a struct addr.
  Output: ./.
  FZ:     string2addr converts a char[] to an addr. If string is not an addr NULL ist returned.
*/

FCONF_EXT UINT16 getUINT16(FILE *in);
/*DOC
  Input:  in is an file stream opened for reading.
  Output: getUINT16 returns an UINT16
  FZ:     the UINT15 is read from the stream using the method lowByte, highByte.
*/

FCONF_EXT int    fputUINT16(FILE *out, UINT16 word);
/*DOC
  Input:  out is an file opened for writing.
          word is the UINT16 which should be written
  Output: fputUIN16 returns the return of the second fputc call.
  FZ:     fputUINT16 writes word into the stream using the order lowByte, highByte.
*/

FCONF_EXT INT    fgetsUntil0(UCHAR *str, size_t n, FILE *f, char *filter);
/*DOC
  Input:  n-1 chars are read at most.
          str is a buffer with the length n.
          f is a file stream opened for reading.
		  filter is a string with characters that shoudn't be reading
  Output: fgetsUntil0 returns the number of chars read including the last \0
  FZ:     fgetsUntil0 reads chars into the buffer until eof(f) || n-1 are read || a \0 is encountered.
*/

FCONF_EXT char   *stripLeadingChars(char *str, const char *chr);
/*DOC
  Input:  str is a \0-terminated string
          chr contains a list of characters.
  Output: stripLeadingChars returns a pointer to a string
  FZ:     all leading characters which are in chr are deleted.
          str is changed and returned.
*/


/*DOC
  Input:  str is a \0-terminated string
          chr contains a list of characters.
  Output: stripTrailingChars returns a pointer to a string
  FZ:     all trailing characters which are in chr are deleted.
          str is changed and returned (not reallocated, simply shorted).
*/
char *stripTrailingChars(char *str, const char *chr);


#define stripRoundingChars(str,chrs) (stripTrailingChars(stripLeadingChars((str),(chrs)),(chrs)))


FCONF_EXT char   *strUpper(char *str);
/*DOC
  Input:  str is a \0 terminated string
  Output: a pointer to a \0 terminated string is returned.
  FZ:     strUpper converts the string from lower case to upper case.
  */

FCONF_EXT char   *strLower(char *str);
/*DOC
  Input:  str is a \0 terminated string
  Output: a pointer to a \0 terminated string is returned.
  FZ:     strLower converts the string from upper case to lower case.
*/

/* will be moved to huskylib */
FCONF_EXT  char    *GetFilenameFromPathname(const char* pathname);
/*   Get the object name from the end of a full or partial pathname.
    The GetFilenameFromPathname function gets the file (or directory) name
    from the end of a full or partial pathname. Returns The file (or directory)
    name
*/

/* will be moved to huskylib */
#define basename(f) GetFilenameFromPathname(f)

/* will be moved to huskylib */
/*  Get the object name from the end of a full or partial pathname (OS-independed).
    This function gets the file (or directory) name from the end of a full
    or partial pathname for any path style: UNIX, DOS or mixed (mixed style
    may be used in Windows NT OS family).
    Returns the file (or directory) name: pointer to part of all original pathname.
*/
char *OS_independed_basename(const char *pathname);

/* will be moved to huskylib */
/* Return directory part of pathname (without filename, '/' or '\\' present at end)
 * Return value is pointer to malloc'ed string;
 * if pathname is filenfme without directory return current directory (./ or .\)
 */
char    *GetDirnameFromPathname(const char* pathname);

#define dirname(f) GetDirnameFromPathname(f)

/* will be moved to huskylib */
FCONF_EXT  char *makeMsgbFileName(ps_fidoconfig config, char *s);
/*
    makes correct file neme fot echo or fecho area
*/


FCONF_EXT char   *shell_expand(char *str);
/*DOC
   Input: str is a \0 terminated string which must have been malloc'ed
   Ouput: a pointer to a \0 terminated string is returned which must be free'd
   FZ:    shell_expand expands the strings just like ~/.msged to /home/mtt/.msged
          see sh(1) for further explanations
*/

FCONF_EXT char   *vars_expand(char *str);
/*DOC
   Input: str is a \0 terminated string which must have been malloc'ed
   Ouput: a pointer to a \0 terminated string is returned which must be free'd
   FZ:    vars_expand expands the strings just like [home]/etc to /home/fnet/etc
          and (under unix and os2/emx) `uname` to Linux
*/

/* will be moved to huskylib */
FCONF_EXT char *makeUniqueDosFileName(const char *dir, const char *ext, s_fidoconfig *config);
/*DOC
   Input:  dir: \0 terminated string designating the destination directory
           ext: \0 terminated string designating the file extension w/o dot
           config: is used to generate a node specific offset
   Output: a pointer to a malloc'ed \0 terminated string is returned.
   FZ:     Creates a unique DOS compatible file name inside the given directory.
           See the comments in common.c for further explanations
*/
/* will be moved to huskylib */
FCONF_EXT int  NCreateOutboundFileName(ps_fidoconfig config, s_link *link, e_flavour prio, e_pollType typ);
/*DOC
  Input:  link is the link whose OutboundFileName should be created.
          prio is some kind of CRASH, HOLD, NORMAL
          typ is some kind of PKT, REQUEST, FLOFILE
  Output: a pointer to a char is returned.
  FZ:     1 is returned if link is busy
         -1 can't create bsy file
          0 else
          */

/*  fileBoxes support */

/* will be moved to huskylib */
FCONF_EXT int needUseFileBoxForLink (ps_fidoconfig config, s_link *link);
/* will be moved to huskylib */
FCONF_EXT char *makeFileBoxName     (ps_fidoconfig config, s_link *link);
/* will be moved to huskylib */
FCONF_EXT void fillCmdStatement(char *cmd, const char *call, const char *archiv, const char *file, const char *path);

/* will be moved to huskylib */
/*  Change file sufix (add if not present).
    inc = 1 - increment suffix of file if new file exist;
          rename file; return new file name or NULL; set errno
    inc = 0 - do not increment suffix, do not rename file, return new suffix only
    if 1st or 2nd parameter is NULL return NULL and set errno to EINVAL
*/
FCONF_EXT char* changeFileSuffix(char *fileName, char *newSuffix, int inc);

/* will be moved to huskylib */
FCONF_EXT int move_file(const char *from, const char *to, const int force_rewrite);
/* DOC
   Input:  source and destination filename
   Output: 0 if OK, != 0 and errno set on error
   FZ:     Move a file, works even over file system boundaries,
   replace file if the destination file already exists and force_rewrite !=0
*/

/* will be moved to huskylib */
/*
 * copy file into other location
 * rewrite existing file if third parameter not zero
 * return 0 if success; else return -1 and set errno
 */
FCONF_EXT int copy_file(const char *from, const char *to, const int force_rewrite);

FCONF_EXT ULONG fc_GetDiskFreeSpace(const char *path);

/*  this function returns the string representation of an address. */
/*  it returns a static array!!! */
FCONF_EXT char *aka2str(const s_addr aka);

/* This function returns the string representation of an 5D address.
 * Return malloc()'ed string!
 */
FCONF_EXT char *aka2str5d(s_addr aka);

FCONF_EXT int patimat(char *raw, char *pat); /* case insensitive */

void freeGroups(char **grps, int numGroups);
char **copyGroups(char **grps, int numGroups);
void freeLink (s_link *link);

int e_readCheck(const s_fidoconfig *config, s_area *echo, s_link *link);
/*  '\x0000' access o'k */
/*  '\x0001' no access group */
/*  '\x0002' no access level */
/*  '\x0003' no access export */
/*  '\x0004' not linked */

int e_writeCheck(const s_fidoconfig *config, s_area *echo, s_link *link);
/*  '\x0000' access o'k */
/*  '\x0001' no access group */
/*  '\x0002' no access level */
/*  '\x0003' no access import */
/*  '\x0004' not linked */

/*
   safe malloc, realloc, calloc, strdup
   if no memory allocated -- abort()
*/
FCONF_EXT void *smalloc(size_t size);
FCONF_EXT void *srealloc(void *ptr, size_t size);
FCONF_EXT void *scalloc(size_t nmemb, size_t size);
FCONF_EXT char *sstrdup(const char *src);

/* safe strdup for line part */
#define sstrndup(src,len) strncpy(smalloc(len),src,len)

/* safe strlen: if src==NULL return 0 */
#define sstrlen(src) ( src ? strlen(src) : 0 )

/* safe strcpy: if src==NULL or dst==NULL don't copy and return NULL */
#define sstrcpy(dst,src) ( (src)&&(dst) ? strcpy(dst,src) : NULL )

/* safe strncpy: if src==NULL or dst==NULL don't copy and return NULL */
#define sstrncpy(dst,src,len) ( (src)&&(dst) ? strncpy(dst,src,len) : NULL )

/* safe strstr: if str==NULL or find==NULL return NULL */
#define sstrstr(str,find) ( (str)&&(find) ? strstr(str,find) : NULL )

/* safe stristr: if str==NULL or find==NULL return NULL */
#define sstristr(str,find) ( (str)&&(find) ? fc_stristr(str,find) : NULL )

/* safe strcmp */
FCONF_EXT int sstrcmp(const char *str1, const char *str2);

/* safe strncmp */
FCONF_EXT int sstrncmp(const char *str1, const char *str2, size_t length);

/* safe stricmp (case-insencitive) */
FCONF_EXT int sstricmp(const char *str1, const char *str2);

/* safety strnicmp (case-insencitive) */
FCONF_EXT int sstrnicmp(const char *str1, const char *str2, size_t length);

/* safety string envelope (use this in functions calls) */
#define sstr(ss) ( ss ? ss : "" )

/* strsep.c
*/
FCONF_EXT char *strseparate(register char **stringp, register const char *delim);

/* Converts decimal value to octal [useful for chmod()] */
unsigned int dec2oct(unsigned int decimal);


#ifdef __cplusplus
}
#endif

#endif


