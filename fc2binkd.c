/******************************************************************************
 * FIDOCONFIG --- library for fidonet configs
 ******************************************************************************
 * fidoconfig to binkd config converter:
 *     generate passwords file or
 *     simple binkd config file (may include it into real config)
 *
 * Copyright (C) 2002
 * Stas Degteff
 * Fido:     2:5080/102
 * Internet: g@grumbler.org
 * Copyrigth (c) Husky Developers Team
 * http://husky.sourceforge.net
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
 * or download from site http://www.gnu.org
 *****************************************************************************/

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#if defined(__TURBOC__) || (defined (_MSC_VER) && (_MSC_VER >= 1200))
#  include <io.h>
#else
#  include <unistd.h>
#endif

#include "fidoconf.h"
#include "common.h"
#include "cvsdate.h"

/* ANSI C knows nothing about this constant */
#ifndef F_OK
#define F_OK 0
#endif


char *version="$Revision$";
char *cvs_tag="$Name$";
char *program_name=NULL;
#define COPYRIGHT "(c) Stas Degteff 2:5080/102 g@grumbler.org\n(c) The Husky Team http://husky.sourceforge.net/team.html"

#define BINKDLOG "binkd.log"

int force_flag = 0; /* Force rewriting flag */

void printversion(){
   if( strcmp(cvs_tag,"$Name$") )
      printf("%s %s\n",program_name,  cvs_tag);
   else
      printf("%s %s\n",program_name,  cvs_date);
}

void usage(int retcode){
      printversion();
      printf("fidoconfig to binkd config converter\n" COPYRIGHT "\n");
      printf("\nUsage:\n");
      printf("%s [-v] [-h] [-c path/to/fidoconfig] [-f] [-p] [output_file_name]\n", program_name);
      printf("where\n");
      printf("\t-v\tprint version and exit\n");
      printf("\t-h\tprint usage information\n");
      printf("\t-c\tspecify alternate fidoconfig\n");
      printf("\t-f\tforce owerwrite file\n");
      printf("\t-p\tgenerate passwords file\n");
      exit(retcode);
}

FILE *createOutputFile(const char *ofname){
  FILE *ofd;

  if( ofname ){
    if( !(access(ofname,F_OK) || force_flag) ){
      fprintf( stderr, "File '%s' exist!", ofname );
      return NULL;
    }
    if( !(ofd=fopen(ofname, "w+t")) ){
      fprintf( stderr, "Can't open file '%s': %s!", ofname, strerror(errno) );
      return NULL;
    }
  }else ofd=stdout;
  return ofd;
}

 /* doublicate backslash in output */
int printPath(char *path, FILE *ofd){
  int rc=1;

  for( ; rc && *path; path++ ){
      if( *path=='\\' ) rc = (EOF != fprintf( ofd, "\\\\"));
      else rc = fputc(*path, ofd);
  }
  return !rc;
}

/* Write passwords file:
 * ;comment
 * <address> <password>
 * <address>
 */
int writePasswords( s_fidoconfig *config, char * ofname ){
  FILE *ofd=NULL;
  unsigned ll = config->linkCount;

  ofd = createOutputFile(ofname);

  if( !ofd ) /* can't create */
    return 2;

  fprintf( ofd, "; binkd / t-mail passwords file\n; generated by %s\n", program_name);

  for( ; ll-- ; ){
    fprintf( ofd, "%s %s\n",
            aka2str(config->links[ll].hisAka), 
            config->links[ll].sessionPwd ? config->links[ll].sessionPwd
                                         : (config->links[ll].defaultPwd ?
                                            config->links[ll].defaultPwd : "" )
           );
  }

  return 0;
}

/* Write max. possible binkd config
 *
 */
int writeBinkdConfig( s_fidoconfig *config, const char *ofname ){
  FILE *ofd=NULL;
  int rc=0;
  unsigned ll;

  ofd = createOutputFile(ofname);

  if( !ofd ) /* can't create */
    return 2;

  fprintf( ofd, "# binkd config file\n# generated by %s\n", program_name);

  /* Address, paths & etc. */
    fprintf( ofd, "\n# The name of your system, it's location, and your name\n");
    if( config->name )
       fprintf( ofd, "sysname   %s\n", config->name );
    if( config->location )
      fprintf( ofd, "location  %s\n", config->location );
    if( config->sysop )
      fprintf( ofd, "sysop     %s\n\n", config->sysop );

    fprintf( ofd, "# Your addresses, 4D or 5D:\naddress" );
    for( ll=config->addrCount ; ll-- ; ){
      fprintf( ofd, " %s", aka2str(config->addr[ll]) );
    }

    fprintf( ofd, "\n\n# Your FTN domains:\ndomain fidonet " );
    printPath( config->outbound, ofd );
    fprintf( ofd, " %u\n\n", config->addr->zone);

    fprintf( ofd, "# Path and name for the logfile\nlog " );
    printPath( config->logFileDir, ofd );
    fprintf( ofd, BINKDLOG "\n" );
    if( config->protInbound ){
      fprintf( ofd, "\n# Inbound directory for secure links\ninbound " );
      printPath( config->protInbound, ofd );
    }
    if( config->inbound ){
      fprintf( ofd, "\n\n# Inbound directory for non-secure links\ninbound-nonsecure " );
      printPath( config->inbound, ofd );
    }
    if( config->minDiskFreeSpace )
      fprintf( ofd, "\n\n# Free space limit for receive next file\nminfree %u\n", config->minDiskFreeSpace );
    else fputc( '\n', ofd );

  /* Links:
   Set out-box flavor to "crash"
   */

  fprintf( ofd, "\n# Define a link:\n"
          "# node [[z:]n/]n[.p][@domain] [-nr] [{hosts|-} [{pwd|-} [flavour [{obox|-} [{ibox|-}]]]]]"
         );
  for( ll=config->linkCount ; ll-- ; ){
    fprintf( ofd, "Node %s * %s %c %s\n",
            aka2str(config->links[ll].hisAka), 
            config->links[ll].sessionPwd ? config->links[ll].sessionPwd
                                         : (config->links[ll].defaultPwd ?
                                            config->links[ll].defaultPwd : "" ),
             config->links[ll].fileBox ? 'c' : ' ',
             config->links[ll].fileBox ? config->links[ll].fileBox : ""
           );
  }

  return rc;
}

int main (int argc, char *argv[]) {
   s_fidoconfig *config=NULL;
   char *ofname=NULL;  /* output file name */
   char *fcfname=NULL; /* fidoconfig file name */
   char *p=NULL;
   int i=1, rc=0;
   unsigned passwords_list_only=0;

   /* Constucte program name from calling string */
   p=strrchr(argv[0],PATH_DELIM);
   if( p ) program_name = p+1;
   else program_name = "fconf2binkd";

   for( ; i<argc; i++ ){
     if( argv[i][0] != '-' ){ /* parameter: output filename */
       ofname = sstrdup( argv[i] );
       break;
     }
     switch( argv[i][1] ){
     case 'v':
     case 'V': /* version request */
               printversion();
               return 1;
     case 'h':
     case 'H': /* help request */
               usage(1);
     case 'f': /* Force rewriting file */
               force_flag = 1;
               break;
     case 'c': /* fidoconfig file name */
               if( ++i<argc ) fcfname = sstrdup( argv[i] );
               else{
                 fprintf(stderr, "'%s' option require parameter!\n", argv[i-1]);
                 usage(-1);
               }
               break;
     case 'p': /* generate passwords list */
               passwords_list_only=1;
               break;
     default:  /* unknown option */
               fprintf(stderr,"Illegal parameter: %s\n",argv[i]);
               usage(1);
     }
   }

   config = readConfig(fcfname);
   if ( config ) {
     if( ofname ) /* if not defined output binkd config to stdout */
       printf("Generate binkd %s file %s\n", passwords_list_only ? "passwords" : "config" , ofname);

     if( passwords_list_only )
       rc = writePasswords( config, ofname );
     else
       rc = writeBinkdConfig( config, ofname );

     disposeConfig( config );
     return rc;
   }

   return 1;
}
