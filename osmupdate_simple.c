// osmupdate 2012-05-14 10:50
#define VERSION "0.2K"
// (c) 2011 Markus Weber, Nuernberg
// hacks (c) 2012 James Michael DuPont
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Affero General Public License
// version 3 as published by the Free Software Foundation.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
// You should have received a copy of this license along
// with this program; if not, see http://www.gnu.org/licenses/.
//
// Other licenses are available on request; please ask the author.

int runwget=1; // debug

#define MAXLOGLEVEL 2
const char* helptext=
  "\nosmupdate " VERSION "\n"
  "\n"
  "This program cares about updating an .osm, .o5m or .pbf file. It\n"
  "will download and apply OSM Change files (.osc) from the servers of\n"
  "\"planet.openstreetmap.org\".\n"
  "It also can assemble a new .osc or .o5c file which can be used to\n"
  "update your OSM data file at a later time.\n"
  "\n"
  "If there is no file timestamp available within your input file, you\n"
  "need to specify the appropriate timestamp manually. In this case, it\n"
  "is recommended to pick a timestamp of one or two days earlier than\n"
  "necessary, just to be on the save side.\n"
  "\n"
  "Prequesites\n"
  "\n"
  "To run this program, please download and install two other programs\n"
  "first: \"osmconvert\" and \"wget\".\n"
  "\n"
  "Usage\n"
  "\n"
  "Two command line arguments are mandatory: the name of the old and the\n"
  "name of the new OSM data file. If the old data file does not have a\n"
  "file timestamp, you may want to specify this timestamp manually on\n"
  "the command line. If you do not, the program will try to determine\n"
  "the timestamp by examining the whole old data file.\n"
  "Instead of the second parameter, you alternatively may specify the\n"
  "name of a change file (.osc or .o5c). In this case, you also may\n"
  "replace the name of the old OSM data file by a timestamp.\n"
  "Command line arguments which are not recognized by osmupdate will be\n"
  "passed to osmconvert. Use this opportunity to supply a bounding box\n"
  "or a bounding polygon if you are going to update a regional change\n"
  "file. You also may exclude unneeded meta data from your file by\n"
  "specifying this osmconvert option: --drop-author\n"
  "\n"
  "Usage Examples\n"
  "\n"
  "  ./osmupdate old_file.o5m new_file.o5m\n"
  "  ./osmupdate old_file.pbf new_file.pbf\n"
  "  ./osmupdate old_file.osm new_file.osm\n"
  "        The old OSM data will be updated and written as new_file.o5m\n"
  "        or new_file.o5m. For safety reasons osmupdate will not delete\n"
  "        the old file. If you do not need it as backup file, please\n"
  "        delete it by yourself.\n"
  "\n"
  "  ./osmupdate old_file.osm 2011-07-15T23:30:00Z new_file.osm\n"
  "  ./osmupdate old_file.osm NOW-86400 new_file.osm\n"
  "        If your old OSM data file does not contain a file timestamp,\n"
  "        or you do not want to rely on this timestamp, it can be\n"
  "        specified manually. Relative times are in seconds to NOW.\n"
  "\n"
  "  ./osmupdate old_file.o5m change_file.o5c\n"
  "  ./osmupdate old_file.osm change_file.osc\n"
  "  ./osmupdate 2011-07-15T23:30:00Z change_file.o5c\n"
  "  ./osmupdate 2011-07-15T23:30:00Z change_file.osc.gz\n"
  "  ./osmupdate NOW-3600 change_file.osc.gz\n"
  "        Here, the old OSM data file is not updated directly. An OSM\n"
  "        changefile is written instead. This changefile can be used to\n"
  "        update the OSM data file afterwards.\n"
  "        You will have recognized the extension .gz in the last\n"
  "        example. In this case, the OSM Change file will be written\n"
  "        with gzip compression. To accomplish this, you need to have\n"
  "        the program gzip installed on your system.\n"
  "\n"
  "  ./osmupdate london_old.o5m london_new.o5m -B=london.poly\n"
  "        The OSM data file london_old.o5m will be updated. Hence the\n"
  "        downloaded OSM changefiles contain not only London, but the\n"
  "        whole planet, a lot of unneeded data will be added to this\n"
  "        regional file. The -B= argument will clip these superfluous\n"
  "        data.\n"
  "\n"
  "The program osmupdate recognizes a few command line options:\n"
  "\n"
  "--max-days=UPDATE_RANGE\n"
  "        By default, the maximum time range for to assemble a\n"
  "        cumulated changefile is 250 days. You can change this by\n"
  "        giving a different maximum number of days, for example 300.\n"
  "        If you do, please ensure that there are daily change files\n"
  "        available for such a wide range of time.\n"
  "\n"
  "--minutely\n"
  "--hourly\n"
  "--daily\n"
  "        By default, osmupdate uses a combination of minutely, hourly\n"
  "        and daily changefiles. If you want to limit these changefile\n"
  "        categories, use one or two of these options and choose that\n"
  "        category/ies you want to be used.\n"
  "\n"
  "--max-merge=COUNT\n"
  "        The subprogram osmconvert is able to merge more than two\n"
  "        changefiles in one run. This ability increases merging speed.\n"
  "        Unfortunately, every changefile consumes about 200 MB of main\n"
  "        memory while being processed. For this reason, the number of\n"
  "        parallely processable changefiles is limited.\n"
  "        Use this commandline argument to determine the maximum number\n"
  "        of parallely processed changefiles. The default value is 7.\n"
  "\n"
  "-t=TEMPPATH\n"
  "--tempfiles=TEMPPATH\n"
  "        On order to cache changefiles, osmupdate needs a separate\n"
  "        directory. This parameter defines the name of this directory,\n"
  "        including the prefix of the tempfiles' names.\n"
  "        The default value is \"osmupdate_temp/temp\".\n"
  "\n"
  "--keep-tempfiles\n"
  "        Use this option if you want to keep local copies of every\n"
  "        downloaded file. This is strongly recommended if you are\n"
  "        going to assemble different changefiles which overlap in\n"
  "        time ranges. Your data traffic will be minimized.\n"
  "\n"
  "--compression-level=LEVEL\n"
  "        Define level for gzip compression. Values between 1 (low\n"
  "        compression, but fast) and 9 (high compression, but slow).\n"
  "\n"
  "--planet-url=PLANET_URL\n"
  "        To accelerate downloads you may specify an alternative\n"
  "        download location. Please enter its URL, or simply the word\n"
  "        \"mirror\" if you want to use gwdg's planet server.\n"
  "\n"
  "-v\n"
  "--verbose\n"
  "        With activated \'verbose\' mode, some statistical data and\n"
  "        diagnosis data will be displayed.\n"
  "        If -v resp. --verbose is the first parameter in the line,\n"
  "        osmupdate will display all input parameters.\n"
  "\n"
  "This program is for experimental use. Expect malfunctions and data\n"
  "loss. Do not use the program in productive or commercial systems.\n"
  "\n"
  "There is NO WARRANTY, to the extent permitted by law.\n"
  "Please send any bug reports to markus.weber@gmx.com\n\n";

#define _FILE_OFFSET_BITS 64
#include <zlib.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>

#define PRIi32 "i" 
typedef uint8_t byte;
typedef unsigned int uint;
#define isdig(x) isdigit((unsigned char)(x))
static int loglevel= 999;  // logging to stderr;
// 0: no logging; 1: small logging; 2: normal logging;
// 3: extended logging;
#define DP(f) fprintf(stderr,"Debug: " #f "\n");
#define DPv(f,...) fprintf(stderr,"Debug: " #f "\n",__VA_ARGS__);
#define DPM(f,p,m) { byte* pp; int i,mm; static int msgn= 3;		\
    if(--msgn>=0) { fprintf(stderr,"Debug memory: " #f);		\
      pp= (byte*)(p); mm= (m); if(pp==NULL) fprintf(stderr,"\n  (null)"); \
      else for(i= 0; i<mm; i++) {					\
	  if((i%16)==0) fprintf(stderr,"\n ");				\
	  fprintf(stderr," %02x",*pp++); }				\
      fprintf(stderr,"\n"); } }

#define NL "\n"  // use LF as new-line sequence
#define DIRSEP '/'
#define DIRSEPS "/"
#define DELFILE "rm"
#define O_BINARY 0




static inline char *strmcpy(char *dest, const char *src, size_t maxlen) {
  // similar to strcpy(), this procedure copies a character string;
  // here, the length is cared about, i.e. the target string will
  // be limited in case it is too long;
  // src[]: source string which is to be copied;
  // maxlen: maximum length of the destination string
  //         (including terminator null);
  // return:
  // dest[]: destination string of the copy; this is the
  //         function's return value too;
  char* d;

  if(maxlen==0)
    return dest;
  d= dest;
  while(--maxlen>0 && *src!=0)
    *d++= *src++;
  *d= 0;
  return dest;
}  // end   strmcpy()
#define strMcpy(d,s) strmcpy((d),(s),sizeof(d))

static inline char *stecpy(char** destp, char* destend,
			   const char* src) {
  // same as stppcpy(), but you may define a pointer which the
  // destination will not be allowed to cross, whether or not the
  // string will be completed at that moment;
  // in either case, the destination string will be terminated with 0;
  char* dest;

  dest= *destp;
  if(dest>=destend)
    return dest;
  destend--;
  while(*src!=0 && dest<destend)
    *dest++= *src++;
  *dest= 0;
  *destp= dest;
  return dest;
}  // end stecpy()

static inline char *stpesccpy(char *dest, const char *src) {
  // same as C99's stpcpy(), but all quotation marks, apostrophes
  // and backslashes will be escaped (i.e., preceded) by backslashes;
  while(*src!=0) {
    if(*src=='\'' || *src=='\"'

       )
      *dest++= '\\';
    *dest++= *src++;
  }
  *dest= 0;
  return dest;
}  // stpesccpy()

static inline char *steesccpy(char **destp,char *destend,
			      const char *src) {
  // same as stppesccpy(), but you may define a pointer which the
  // destination will not be allowed to cross, whether or not the
  // string will be completed at that moment;
  // in either case, the destination string will be terminated with 0;
  char* dest;

  dest= *destp;
  if(dest>=destend)
    return dest;
  destend-= 2;
  while(*src!=0 && dest<destend) {
    if(*src=='\'' || *src=='\"'

       )
      *dest++= '\\';
    *dest++= *src++;
  }
  *dest= 0;
  *destp= dest;
  return dest;
}  // steesccpy()

static inline char *stpmcpy(char *dest, const char *src, size_t maxlen) {
  // similar to strmcpy(), this procedure copies a character string;
  // however, it returns the address of the destination string's
  // terminating zero character;
  // this makes it easier to concatenate strings;
  char* d;

  if(maxlen==0)
    return dest;
  d= dest;
  while(--maxlen>0 && *src!=0)
    *d++= *src++;
  *d= 0;
  return d;
}  // end stpmcpy()
#define stpMcpy(d,s) stpmcpy(d,s,sizeof(d))

static inline int strzcmp(const char* s1,const char* s2) {
  // similar to strcmp(), this procedure compares two character strings;
  // here, the number of characters which are to be compared is limited
  // to the length of the second string;
  // i.e., this procedure can be used to identify a short string s2
  // within a long string s1;
  // s1[]: first string;
  // s2[]: string to compare with the first string;
  // return:
  // 0: both strings are identical; the first string may be longer than
  //    the second;
  // -1: the first string is alphabetical smaller than the second;
  // 1: the first string is alphabetical greater than the second;
  while(*s1==*s2 && *s1!=0) { s1++; s2++; }
  if(*s2==0)
    return 0;
  return *(unsigned char*)s1 < *(unsigned char*)s2? -1: 1;
}  // end   strzcmp()

static inline int strycmp(const char* s1,const char* s2) {
  // similar to strcmp(), this procedure compares two character strings;
  // here, both strings are end-aligned;
  // not more characters will be compared than are existing in string s2;
  // i.e., this procedure can be used to identify a file name extension;
  const char* s1e;
  int l;

  l= strchr(s2,0)-s2;
  s1e= strchr(s1,0);
  if(s1e-s1<l)
    return 1;
  s1= s1e-l;
  while(*s1==*s2 && *s1!=0) { s1++; s2++; }
  if(*s2==0)
    return 0;
  return *(unsigned char*)s1 < *(unsigned char*)s2? -1: 1;
}  // end   strycmp()

static inline uint32_t strtouint32(const char* s) {
  // read a number and convert it to an unsigned 32-bit integer;
  // return: number;
  int32_t i;
  byte b;

  i= 0;
  for(;;) {
    b= (byte)(*s++ -'0');
    if(b>=10)
      break;
    i= i*10+b;
  }
  return i;
}  // end   strtouint32()

static inline int64_t strtosint64(const char* s) {
  // read a number and convert it to a signed 64-bit integer;
  // return: number;
  int sign;
  int64_t i;
  byte b;

  if(*s=='-') { s++; sign= -1; } else sign= 1;
  i= 0;
  for(;;) {
    b= (byte)(*s++ -'0');
    if(b>=10)
      break;
    i= i*10+b;
  }
  return i*sign;
}  // end   strtosint64()

static inline int64_t strtimetosint64(const char* s) {
  // read a timestamp in OSM format, e.g.: "2010-09-30T19:23:30Z",
  // and convert it to a signed 64-bit integer;
  // also allowed: relative time to NOW, e.g.: "NOW-86400",
  // which means '24 hours ago';
  // return: time as a number (seconds since 1970);
  //         ==0: syntax error;
  if(s[0]=='N') {  // presumably a relative time to 'now'
    if(s[1]!='O' || s[2]!='W' || (s[3]!='+' && s[3]!='-') ||
       !isdig(s[4]))  // wrong syntax
      return 0;
    s+= 3;  // jump over "NOW"
    if(*s=='+') s++;  // jump over '+', if any
    return time(NULL)+strtosint64(s);
  }  // presumably a relative time to 'now'
  if((s[0]!='1' && s[0]!='2') ||
     !isdig(s[1]) || !isdig(s[2]) || !isdig(s[3]) ||
     s[4]!='-' || !isdig(s[5]) || !isdig(s[6]) ||
     s[7]!='-' || !isdig(s[8]) || !isdig(s[9]) ||
     s[10]!='T' || !isdig(s[11]) || !isdig(s[12]) ||
     s[13]!=':' || !isdig(s[14]) || !isdig(s[15]) ||
     s[16]!=':' || !isdig(s[17]) || !isdig(s[18]) ||
     s[19]!='Z')  // wrong syntax
    return 0;
  /* regular timestamp */ {
    struct tm tm;

    tm.tm_isdst= 0;
    tm.tm_year=
      (s[0]-'0')*1000+(s[1]-'0')*100+(s[2]-'0')*10+(s[3]-'0')-1900;
    tm.tm_mon= (s[5]-'0')*10+s[6]-'0'-1;
    tm.tm_mday= (s[8]-'0')*10+s[9]-'0';
    tm.tm_hour= (s[11]-'0')*10+s[12]-'0';
    tm.tm_min= (s[14]-'0')*10+s[15]-'0';
    tm.tm_sec= (s[17]-'0')*10+s[18]-'0';

    return timegm(&tm);

  }  // regular timestamp
}  // end   strtimetosint64()

static inline void int64tostrtime(uint64_t v,char* sp) {
  // write a timestamp in OSM format, e.g.: "2010-09-30T19:23:30Z",
  // into a string;
  // v: value of the timestamp;
  // sp[21]: destination string;
  time_t vtime;
  struct tm tm;
  int i;

  vtime= v;
  gmtime_r(&vtime,&tm);

  i= tm.tm_year+1900;
  sp+= 3; *sp--= i%10+'0';
  i/=10; *sp--= i%10+'0';
  i/=10; *sp--= i%10+'0';
  i/=10; *sp= i%10+'0';
  sp+= 4; *sp++= '-';
  i= tm.tm_mon+1;
  *sp++= i/10+'0'; *sp++= i%10+'0'; *sp++= '-';
  i= tm.tm_mday;
  *sp++= i/10+'0'; *sp++= i%10+'0'; *sp++= 'T';
  i= tm.tm_hour;
  *sp++= i/10+'0'; *sp++= i%10+'0'; *sp++= ':';
  i= tm.tm_min;
  *sp++= i/10+'0'; *sp++= i%10+'0'; *sp++= ':';
  i= tm.tm_sec%60;
  *sp++= i/10+'0'; *sp++= i%10+'0'; *sp++= 'Z'; *sp= 0;
}  // end   int64tostrtime()

static inline bool file_exists(const char* file_name) {
  // query if a file exists;
  // file_name[]: name of the file in question;
  // return: the file exists;
  return access(file_name,R_OK)==0;
}  // file_exists()

static inline int64_t file_length(const char* file_name) {
  // retrieve length of a file;
  // file_name[]: file name;
  // return: number of bytes of this file;
  //         if the file could not be accessed, the return value is -1;
  struct stat s;
  int r;

  r= stat(file_name,&s);
  if(r==0)
    return s.st_size;
  return -1;
}  // end   file_length()



//------------------------------------------------------------
// Module Global   global variables for this program
//------------------------------------------------------------

// to distinguish global variable from local or module global
// variables, they are preceded by 'global_';



static char global_tempfile_name[450]= "";
// prefix of names for temporary files
static bool global_keep_tempfiles= false;
// temporary files shall not be deleted at program end


static int global_max_merge= 7;
// maximum number of parallely processed changefiles
static const char* global_gzip_parameters= "";
// parameters for gzip compression
static char global_planet_url[400]=
  "http://planet.openstreetmap.org/";

#define PERR(f)					\
  fprintf(stderr,"osmupdate Error: " f "\n");
// print error message
#define PERRv(f,...)						\
  fprintf(stderr,"osmupdate Error: " f "\n",__VA_ARGS__);
// print error message with value(s)
#define WARN(f) { static int msgn= 3; if(--msgn>=0)			\
					fprintf(stderr,"osmupdate Warning: " f "\n"); }
// print a warning message, do it maximal 3 times
#define WARNv(f,...) { static int msgn= 3; if(--msgn>=0)		\
					     fprintf(stderr,"osmupdate Warning: " f "\n",__VA_ARGS__); }
// print a warning message with value(s), do it maximal 3 times
#define PINFO(f)						\
  fprintf(stderr,"osmupdate: " f "\n"); // print info message
#define PINFOv(f,...)					\
  fprintf(stderr,"osmupdate: " f "\n",__VA_ARGS__);
#define ONAME(i)							\
  (i==0? "node": i==1? "way": i==2? "relation": "unknown object")

//------------------------------------------------------------
// end   Module Global   global variables for this program
//------------------------------------------------------------


static void shell_command(const char* command,char* result) {
  // execute a shell command;
  // command[]: shell command;
  // result[1000]: result of the command;
  FILE* fp;
  char* result_p;
  int maxlen;
  int r;

  if(loglevel>=2) {
    PINFOv("Executing shell command:\n%s",command);
  }
  fp= popen(command,"r");
  if(fp==NULL) {
    PERR("Could not execute shell command.")
      result[0]= 0;
    exit(1);
  }
  result_p= result;
  maxlen= 1000-1;
  while(maxlen>0) {
    r= read(fileno(fp),result_p,maxlen);
    if(r<=0)  // end of data
      break;
    result_p+= r;
    maxlen-= r;
  }
  *result_p= 0;
  pclose(fp);
  if(loglevel>=2) {
    PINFOv("Got shell command result:\n%s",result);
  }
}  // end   shell_command()


static void shell_command_retry(const char* command,char* result) {
  shell_command(command,result);
  int count =30;
  while(count){
    if  (strstr(result,"unable to resolve host address"))
      {
	printf ("going to retry\n");
	shell_command(command,result);
	count --;
      }
    else
      {
	count =0;
      }
  }
}


typedef enum {cft_UNKNOWN,cft_MINUTELY,cft_HOURLY,cft_DAILY} changefile_type_t;
#define CFTNAME(i)    (i==cft_MINUTELY? "minutely": i==cft_HOURLY? "hourly":    i==cft_DAILY? "daily": "unknown")


static int64_t get_newest_changefile_timestamp(changefile_type_t changefile_type,int32_t* file_sequence_number) {
  // get sequence number and timestamp of the newest changefile
  // of a specific changefile type;
  // changefile_type: minutely, hourly or daily changefile;
  // return: timestamp of the file (seconds from Jan 01 1970);
  //         ==0: no file timestamp available;
  // *file_sequence_number: sequence number of the newest changefile;
  static bool firstrun= true;
  char command[1000],*command_p;
  char* command_e= command+sizeof(command);
  char result[1000];
  int64_t changefile_timestamp;


  // first run
  command_p= command;
  stecpy(&command_p,command_e,	 "wget  --tries=10 --retry-connrefused      --waitretry=5 -q ");
  stecpy(&command_p,command_e,global_planet_url);
  switch(changefile_type) {  // changefile type
  case cft_MINUTELY:
    stecpy(&command_p,command_e,"minute-replicate/state.txt");
    break;
  case cft_HOURLY:
    stecpy(&command_p,command_e,"hour-replicate/state.txt");
    break;
  case cft_DAILY:
    stecpy(&command_p,command_e,"day-replicate/state.txt");
    break;
  default:  // invalid change file type
    return 0;
  }  // changefile type
  stecpy(&command_p,command_e," -O - 2>&1");

  shell_command_retry(command,result);

  if(firstrun) {  // first run
    firstrun= false;
    if(result[0]!='#' && (result[0]<'1' || result[0]>'2') && (
							      strstr(result,"not found")!=NULL ||
							      strstr(result,"cannot find")!=NULL)) {  // command not found
      PERR("Please install program wget first.")
	exit(1);
    }  // command not found
  }  // first run
  if(result[0]=='#') {  // full status information
    // get sequence number
    char* sequence_number_p;
    sequence_number_p= strstr(result,"sequenceNumber=");
    if(sequence_number_p!=NULL)  // found sequence number
      *file_sequence_number= strtouint32(sequence_number_p+15);
    // get timestamp
    char* timestamp_p;
    timestamp_p= strstr(result,"timestamp=");
    // search timestamp line
    if(timestamp_p!=NULL && timestamp_p-result<sizeof(result)-30) {
      // found timestamp line
      // cpy timestam to begin of result[]
      timestamp_p+= 10;  // jump over text
      memcpy(result,timestamp_p,13);
      memcpy(result+13,timestamp_p+14,3);
      memcpy(result+16,timestamp_p+18,4);
    }  // found timestamp line
  }  // full status information
  changefile_timestamp= strtimetosint64(result);
  if(loglevel>0) {  // verbose mode
    char ts[30];

    if(changefile_timestamp==0)
      strcpy(ts,"(no timestamp)");
    else
      int64tostrtime(changefile_timestamp,ts);
    PINFOv("newest %s timestamp: %s",CFTNAME(changefile_type),ts)
      }  // verbose mode
  return changefile_timestamp;
}  // get_newest_changefile_timestamp

static void  mkpath (changefile_type_t changefile_type,int32_t file_sequence_number)
{
  char path[500];
  char result[1000];
  sprintf(path,"%s.%c/%03d/%03d/",global_tempfile_name,CFTNAME(changefile_type)[0], file_sequence_number/1000000,file_sequence_number/1000%1000);
  struct stat status;
  int stat_status= stat( path, &status );
  
  if (( stat_status == -1) ) {
    sprintf(path,"mkdir -p %s.%c/%03d/%03d/",global_tempfile_name,CFTNAME(changefile_type)[0], file_sequence_number/1000000,file_sequence_number/1000%1000);
    shell_command(path,result);
  }  else    {
    printf("Path exists %d %d %s\n",stat_status,status.st_mode,path);
  }

}

static int64_t get_changefile_timestamp(
					changefile_type_t changefile_type,int32_t file_sequence_number) {
  // download and inspect the timestamp of a specific changefile which
  // is available in the Internet;
  // a timestamp file will not be downloaded if it
  // already exists locally as temporary file;
  // changefile_type: minutely, hourly or daily changefile;
  // file_sequence_number: sequence number of the file;
  // uses:
  // global_tempfile_name
  // return: timestamp of the changefile (seconds from Jan 01 1970);
  //         ==0: no file timestamp available;
  char command[2000]; char* command_p;
  char* command_e= command+sizeof(command);
  char result[1000];
  char timestamp_cachefile_name[400];
  int fd,r;  // file descriptor; number of bytes which have been read
  char timestamp_contents[1000];  // contents of the timestamp
  int64_t changefile_timestamp;
  char* sp;

  // create the file name for the cached timestamp; example:
  // "osmupdate_temp/temp.m000012345.txt"

  sp= stpmcpy(timestamp_cachefile_name,global_tempfile_name,
	      sizeof(timestamp_cachefile_name[0])-20);
  *sp++= '.';
  *sp++= CFTNAME(changefile_type)[0];
  // 'm', 'h', 'd' for minutely, hourly or daily timestamps
  //sprintf(sp,"/%09"PRIi32".txt",file_sequence_number);
  sprintf(sp,"/%03d/%03d/%03d.txt",	    file_sequence_number/1000000,file_sequence_number/1000%1000,   file_sequence_number%1000    );

  // add sequence number and file name extension

  mkpath(changefile_type,file_sequence_number);


  // download the timestamp into a cache file
  if(file_length(timestamp_cachefile_name)<10) {
    // timestamp has not been downloaded yet
    command_p= command;
    stecpy(&command_p,command_e,"wget  --tries=10  --retry-connrefused      --waitretry=5 -nv -c ");
    stecpy(&command_p,command_e,global_planet_url);
    if(changefile_type==cft_MINUTELY)
      stecpy(&command_p,command_e,"minute-replicate/");
    else if(changefile_type==cft_HOURLY)
      stecpy(&command_p,command_e,"hour-replicate/");
    else if(changefile_type==cft_DAILY)
      stecpy(&command_p,command_e,"day-replicate/");
    else  // invalid change file type
      return 0;
    /* assemble Sequence path */ {
      int l;
      l= sprintf(command_p,"%03i/%03i/%03i",
		 file_sequence_number/1000000,file_sequence_number/1000%1000,
		 file_sequence_number%1000);
      command_p+= l;
    }
    stecpy(&command_p,command_e,".state.txt -O \"");
    steesccpy(&command_p,command_e,timestamp_cachefile_name);
    stecpy(&command_p,command_e,"\" 2>&1");

    shell_command_retry(command,result);
    
  }  // timestamp has not been downloaded yet

  // read the timestamp cache file
  fd= open(timestamp_cachefile_name,O_RDONLY|O_BINARY);
  if(fd<=0)  // could not open the file
    timestamp_contents[0]= 0;  // hence we did not read anything
  else {  // could open the file
    r= read(fd,timestamp_contents,sizeof(timestamp_contents)-1);
    if(r<0) r= 0;
    timestamp_contents[r]= 0;  // set string terminator
    close(fd);
  }  // could open the file

  // parse the timestamp information
  if(timestamp_contents[0]=='#') {  // full status information
    // get timestamp
    char* timestamp_p;
    timestamp_p= strstr(timestamp_contents,"timestamp=");
    // search timestamp line
    if(timestamp_p!=NULL &&
       timestamp_p-timestamp_contents<sizeof(timestamp_contents)-30) {
      // found timestamp line
      // copy timestamp to begin of timestamp_contents[]
      timestamp_p+= 10;  // jump over text
      memcpy(timestamp_contents,timestamp_p,13);
      memcpy(timestamp_contents+13,timestamp_p+14,3);
      memcpy(timestamp_contents+16,timestamp_p+18,4);
    }  // found timestamp line
  }  // full status information
  changefile_timestamp= strtimetosint64(timestamp_contents);

  if(loglevel>0) {  // verbose mode
    char ts[30];

    if(changefile_timestamp==0)
      strcpy(ts,"(no timestamp)");
    else
      int64tostrtime(changefile_timestamp,ts);


    printf("%s", CFTNAME(changefile_type));
    printf(" changefile %i",file_sequence_number);
    printf(": %s\n",ts);

    //    PINFOv("%s changefile %i: %s",	   CFTNAME(changefile_type),file_sequence_number,ts)

      }  // verbose mode
  if(changefile_timestamp==0) {  // no timestamp
    if(file_sequence_number==0)  // first file in repository
      changefile_timestamp= 1;  // set virtual very old timestamp
    else {
      PERRv("no timestamp for %s changefile %i.",
	    CFTNAME(changefile_type),file_sequence_number)
	exit(1);
    }
  }  // no timestamp
  return changefile_timestamp;
}  // get_changefile_timestamp

static void wget_changefile (changefile_type_t changefile_type,
		      int32_t file_sequence_number,
		      const char * this_cachefile_name
		      )
{
  char command[1000],*command_p;
  char* command_e= command+sizeof(command);
  command_p= command;

  stecpy(&command_p,command_e,"wget  --tries=10 --retry-connrefused      --waitretry=5 -nv -c ");
  stecpy(&command_p,command_e,global_planet_url);
  
  switch(changefile_type) {  // changefile type
  case cft_MINUTELY:
    stecpy(&command_p,command_e,"minute-replicate/");
    break;
  case cft_HOURLY:
    stecpy(&command_p,command_e,"hour-replicate/");
    break;
  case cft_DAILY:
    stecpy(&command_p,command_e,"day-replicate/");
    break;
  default:  // invalid change file type
    return;
  }  // changefile type
  
  /* process sequence number */ 
  {
    int l;
    l= sprintf(command_p,"%03i/%03i/%03i.osc.gz",
	       file_sequence_number/1000000,file_sequence_number/1000%1000,
	       file_sequence_number%1000);
    command_p+= l;
  }  // process sequence number
  
  stecpy(&command_p,command_e," -O \"");
  steesccpy(&command_p,command_e,this_cachefile_name);
  stecpy(&command_p,command_e,"\" 2>&1 && echo \"Wget Command Ok\"");
  
  if (runwget){
    char result[1000];

    shell_command_retry(command,result);

    if(strstr(result,"Wget Command Ok")==NULL) {  // download error
      PERRv("Could not download %s changefile %i",
	    CFTNAME(changefile_type),file_sequence_number)
	PINFOv("wget Error message:\n%s",result)
	exit(1);
    }      
    }
  else
    {
      printf("Skipping running %s\n",command);
    }
}



static void process_changefile(changefile_type_t changefile_type,int32_t file_sequence_number, int64_t new_timestamp) 
{
  // download and process a change file;
  // change files will not be processed one by one, but cumulated
  // until some files have been downloaded and then processed in a group;
  // a file will not be downloaded if it already exists locally as
  // temporary file;
  // changefile_type: minutely, hourly or daily changefile;
  // file_sequence_number: sequence number of the file;
  //                       ==0: process the remaining files which
  //                            are waiting in the cache; cleanup;
  // new_timestamp: timestamp of the new file which is to be created;
  //            ==0: the procedure will assume the newest of all
  //                 timestamps which has been passed since the
  //                 program has been started;
  // uses:
  // global_max_merge
  // global_tempfile_name
  static bool firstrun= true;

  static int64_t newest_new_timestamp= 0;
  static char master_cachefile_name[400];
  static char master_cachefile_name_temp[400];
  static char cachefile_name[400];
  
  char command[4000+200*10];
  char* command_e= command+sizeof(command);
  char* command_p;
  char result[1000];

  if(firstrun) {
    firstrun= false;
    // create the file name for the cached master changefile;
    // usually: "osmupdate_temp/temp.8"
    strcpy(stpmcpy(master_cachefile_name,global_tempfile_name,
		   sizeof(master_cachefile_name)-5),".8");
    strcpy(stpmcpy(master_cachefile_name_temp,global_tempfile_name,
		   sizeof(master_cachefile_name_temp)-5),".9");
    unlink(master_cachefile_name);
    unlink(master_cachefile_name_temp);
  }
  if(new_timestamp>newest_new_timestamp)
    newest_new_timestamp= new_timestamp;

  if(file_sequence_number!=0) {  // changefile download requested
    char* this_cachefile_name=      cachefile_name;
    int64_t old_file_length;
    char* sp;
    const char* startsp=sp;

    // create the file name for the cached changefile; example:
    // "osmupdate_temp/temp.m000012345.osc.gz"
    sp= stpmcpy(this_cachefile_name,global_tempfile_name,
		sizeof(cachefile_name[0])-20);
    *sp++= '.';
    *sp++= CFTNAME(changefile_type)[0];
    // 'm', 'h', 'd' for minutely, hourly or daily changefiles

    // make a path 
    mkpath(changefile_type,file_sequence_number);

    sprintf(sp,"/%03d/%03d/%03d.osc.gz",	    file_sequence_number/1000000,file_sequence_number/1000%1000,   file_sequence_number%1000    );
   

    // add sequence number and file name extension

    // assemble the URL and download the changefile
    old_file_length= file_length(this_cachefile_name);

    if(old_file_length <= 0)       {
      
      PINFOv("%s changefile %i: downloading",    CFTNAME(changefile_type),file_sequence_number);
      
      wget_changefile (changefile_type,
		       file_sequence_number,
		       this_cachefile_name
		       );
      
      PINFOv("%s changefile %i: download completed",
	     CFTNAME(changefile_type),file_sequence_number);
    }    else       {
      const char * ftypename=CFTNAME(changefile_type);
      
      printf("%s changefile %i: already in cache size:%lld name :%s\n", ftypename,file_sequence_number, old_file_length, this_cachefile_name);
    }
    

  }  // changefile download requested
  

}  // process_changefile()


int main(int argc,const char** argv) {
  // main procedure;
  // for the meaning of the calling line parameters please look at the
  // contents of helptext[];

  // variables for command line arguments
  int main_return_value;
  const char* a;  // command line argument


  const char* old_file;  // name of the old OSM file
  int64_t old_timestamp;  // timestamp of the old OSM file
  const char* new_file;  // name of the new OSM file or OSM Change file
  bool new_file_is_o5;  // the new file is type .o5m or .o5c
  bool new_file_is_pbf;  // the new file is type .pbf
  bool new_file_is_changefile;  // the new file is a changefile
  bool new_file_is_gz;  // the new file is a gzip compressed
  int64_t max_update_range;  // maximum range for cumulating changefiles
  // in order to update an OSM file; unit: seconds;
  char tempfile_directory[400];  // directory for temporary files
  bool process_minutely,process_hourly,process_daily;
  // if one of these variables is true, then only the chosen categories
  // shall be processed, minutely, hourly or daily;
  bool no_minutely,no_hourly,no_daily;
  // the category shall not be processed;

  // regular variables
  int64_t minutely_timestamp,hourly_timestamp,daily_timestamp;
  // timestamps for changefiles which are available in the Internet;
  // unit: seconds after Jan 01 1970;
  int32_t minutely_sequence_number,hourly_sequence_number,
    daily_sequence_number;
  int64_t timestamp;
  int64_t next_timestamp;

  // care about clean-up procedures
  //atexit(restore_timezone); ,,,

  // initializations
  main_return_value= 0;  // (default)

  old_file= NULL;
  old_timestamp= 0;
  new_file= NULL;
  new_file_is_o5= false;
  new_file_is_pbf= false;
  new_file_is_changefile= false;
  new_file_is_gz= false;
  max_update_range= 250*86400;
  process_minutely= process_hourly= process_daily= false;
  no_minutely= no_hourly= no_daily= false;


  // read command line parameters
  if(argc<=1) {  // no command line parameters given
    fprintf(stderr,"osmupdate " VERSION "\n"
	    "Updates .osm and .o5m files, downloads .osc and o5c files.\n"
	    "To get detailed help, please enter: ./osmupdate -h\n");
    return 0;  // end the program, because without having parameters
    // we do not know what to do;
  }


  while(--argc>0) {  // for every parameter in command line
    argv++;  // switch to next parameter; as the first one is just
    // the program name, we must do this prior reading the
    // first 'real' parameter;
    a= argv[0];
    if(loglevel>0)  // verbose mode
      fprintf(stderr,"osmupdate Parameter: %.2000s\n",a);
    if(strcmp(a,"-h")==0 || strcmp(a,"-help")==0 ||
       strcmp(a,"--help")==0) {  // user wants help text
      fprintf(stderr,"%s",helptext);  // print help text
      // (took "%s", to prevent oversensitive compiler reactions)
      return 0;
    }
    if((strcmp(a,"-v")==0 || strcmp(a,"--verbose")==0 ||
        strzcmp(a,"-v=")==0 || strzcmp(a,"--verbose=")==0) &&
       loglevel==0) {  // test mode - if not given already
      const char* sp;

      sp= strchr(a,'=');
      if(sp!=NULL) loglevel= sp[1]-'0'; else loglevel= 1;
      if(loglevel<1) loglevel= 1;
      if(loglevel>MAXLOGLEVEL) loglevel= MAXLOGLEVEL;
      if(a[1]=='-') {  // must be "--verbose" and not "-v"
        if(loglevel==1)
          fprintf(stderr,"osmupdate: Verbose mode.\n");
        else
          fprintf(stderr,"osmupdate: Verbose mode %i.\n",loglevel);
      }
      continue;  // take next parameter
    }
    if(strzcmp(a,"--max-days=")==0) {  // maximum update range
      max_update_range= (int64_t)strtouint32(a+11)*86400;
      continue;  // take next parameter
    }
    if((strzcmp(a,"-t=")==0 || strzcmp(a,"--tempfiles=")==0) &&
       global_tempfile_name[0]==0) {
      // user-defined prefix for names of temorary files
      strmcpy(global_tempfile_name,strchr(a,'=')+1,
	      sizeof(global_tempfile_name)-50);
      continue;  // take next parameter
    }
    if(strzcmp(a,"--keep-tempfiles")==0) {
      // temporary files shall not be deleted at program end
      global_keep_tempfiles= true;
      continue;  // take next parameter
    }
    if(strzcmp(a,"--compression-level=")==0) {
      // gzip compression level
      static char gzip_par[3]= "";

      if(a[20]<'1' || a[20]>'9' || a[21]!=0) {
        PINFO("Range error. Changed to --compression-level=3")
	  gzip_par[0]= '-'; gzip_par[1]= '3'; gzip_par[2]= 0;
      }
      else {
        gzip_par[0]= '-'; gzip_par[1]= a[20]; gzip_par[2]= 0;
        global_gzip_parameters= gzip_par;
      }
      continue;  // take next parameter
    }
    if(strzcmp(a,"--minutely")==0) {  // process minutely data
      process_minutely= true;
      continue;  // take next parameter
    }
    if(strzcmp(a,"--hourly")==0) {  // process hourly data
      process_hourly= true;
      continue;  // take next parameter
    }
    if(strzcmp(a,"--daily")==0) {  // process daily data
      process_daily= true;
      continue;  // take next parameter
    }
    if(strzcmp(a,"--planet-url=")==0 && a[13]!=0) {
      // change planet url
      const char* ap;
      char* sp;

      ap= a+13;
      if(strcmp(ap,"mirror")==0)
        strcpy(global_planet_url,"ftp://ftp5.gwdg.de/pub/misc/"
	       "openstreetmap/planet.openstreetmap.org/");
      else if(strstr(ap,"://")!=NULL)
        strmcpy(global_planet_url,ap,sizeof(global_planet_url)-1);
      else {
        strcpy(global_planet_url,"http://");
        strmcpy(global_planet_url+7,ap,sizeof(global_planet_url)-8);
      }
      sp= strchr(global_planet_url,0);
      if(sp>global_planet_url && sp[-1]!='/') {
        *sp++= '/'; sp= 0; }
      continue;  // take next parameter
    }
    if(a[0]=='-') {
      // command line argument not recognized by osmupdate

      int len;

      len= strlen(a)+3;

    }
    if(strcmp(a,"--complete-ways")==0 ||
       strcmp(a,"--complex-ways")==0 ||
       strcmp(a,"--drop-brokenrefs")==0 ||
       strcmp(a,"--drop-broken-refs")==0) {
      WARNv("option %.80s does not work with updates.",a)
	continue;  // take next parameter
    }
    if(strzcmp(a,"-b=")!=0 && strzcmp(a,"-B=")!=0) {
      // not a bounding box and not a bounding polygon

    }

    continue;  // take next parameter
  }
  if(old_timestamp==0) {
    old_timestamp= strtimetosint64(a);
  }
  // here: parameter must be a file name
  if(old_file==NULL && old_timestamp==0) {  // name of the old file
    old_file= a;
  
  }
  if(new_file==NULL) {  // name of the new file
    new_file= a;
    new_file_is_o5=
      strycmp(new_file,".o5m")==0 || strycmp(new_file,".o5c")==0 ||
      strycmp(new_file,".o5m.gz")==0 ||
      strycmp(new_file,".o5c.gz")==0;
    new_file_is_pbf=
      strycmp(new_file,".pbf")==0;
    new_file_is_changefile=
      strycmp(new_file,".osc")==0 || strycmp(new_file,".o5c")==0 ||
      strycmp(new_file,".osc.gz")==0 ||
      strycmp(new_file,".o5c.gz")==0;
    new_file_is_gz= strycmp(new_file,".gz")==0;
  }

  /* create tempfile directory for cached timestamps and changefiles */ 
  {
    char *sp;

    if(strlen(global_tempfile_name)<2)  // not set yet
      strcpy(global_tempfile_name,"osmupdate_temp"DIRSEPS"temp");
    // take default
    sp= strchr(global_tempfile_name,0);
    if(sp[-1]==DIRSEP)  // it's a bare directory
      strcpy(sp,"temp");  // add a file name prefix
    strMcpy(tempfile_directory,global_tempfile_name);
    sp= strrchr(tempfile_directory,DIRSEP);
    // get last directory separator
    if(sp!=NULL) *sp= 0;  // if found any, cut the string here
    mkdir(tempfile_directory,0700);

  }

  // get file timestamp of OSM input file
  if(old_timestamp==0) {  // no timestamp given by the user
    if(old_file==NULL) {  // no file name given for the old OSM file
      PERR("Specify at least the old OSM file's name or its timestamp.")
	return 1;
    }
    if(!file_exists(old_file)) {  // old OSM file does not exist
      PERRv("Old OSM file does not exist: %.80s",old_file);
      return 1;
    }


  }  // end   no timestamp given by the user

  // parameter consistency check
  if(new_file==NULL) {
    PERR("No output file was specified.");
    return 1;
  }

  if(old_file==NULL && !new_file_is_changefile) {
    PERR("If no old OSM file is specified, osmupdate can only "
	 "generate a changefile.");
    return 1;
  }

  // take care of user defined processing categories
  if(process_minutely || process_hourly || process_daily) {
    if(!process_minutely) no_minutely= true;
    if(!process_hourly) no_hourly= true;
    if(!process_daily) no_daily= true;
  }

  // get last timestamp for each, minutely, hourly and daily diff files
  minutely_sequence_number= hourly_sequence_number=
    daily_sequence_number= 0;
  minutely_timestamp= hourly_timestamp= daily_timestamp= 0;
  if(!no_minutely) {
    minutely_timestamp= get_newest_changefile_timestamp(cft_MINUTELY,&minutely_sequence_number);
    if(minutely_timestamp==0) {
      PERR("Could not get the newest minutely timestamp from the Internet.")
	return 1;
    }
  }
  if(!no_hourly) {
    hourly_timestamp= get_newest_changefile_timestamp(
						      cft_HOURLY,&hourly_sequence_number);
    if(hourly_timestamp==0) {
      PERR("Could not get the newest hourly timestamp from the Internet.")
	return 1;
    }
  }
  if(!no_daily) {
    daily_timestamp= get_newest_changefile_timestamp(
						     cft_DAILY,&daily_sequence_number);
    if(daily_timestamp==0) {
      PERR("Could not get the newest daily timestamp from the Internet.")
	return 1;
    }
  }

  // check maximum update range
  if(minutely_timestamp-old_timestamp>max_update_range) {
    // update range too large
    int days;
    days= (int)((minutely_timestamp-old_timestamp+86399)/86400);
    PERRv("Update range too large: %i days.",days)
      PINFOv("To allow such a wide range, add: --max-days=%i",days)
      return 1;
  }  // update range too large

  // clear last hourly timestamp if
  // OSM old file's timestamp > latest hourly timestamp - 30 minutes
  if(old_timestamp>hourly_timestamp-30*60 && !no_minutely)
    hourly_timestamp= 0;  // (let's take minutely updates)

  // clear last daily timestamp if
  // OSM file timestamp > latest daily timestamp - 16 hours
  if(old_timestamp>daily_timestamp-16*3600 &&
     !(no_hourly && no_minutely))
    daily_timestamp= 0;  // (let's take hourly and minutely updates)

  // initialize start timestamp
  timestamp= 0;
  if(timestamp<minutely_timestamp) timestamp= minutely_timestamp;
  if(timestamp<hourly_timestamp) timestamp= hourly_timestamp;
  if(timestamp<daily_timestamp) timestamp= daily_timestamp;

  // get and process minutely diff files from last minutely timestamp
  // backward; stop just before latest hourly timestamp or OSM
  // file timestamp has been reached;
  if(minutely_timestamp!=0) {
    next_timestamp= timestamp;
    while(next_timestamp>hourly_timestamp &&
	  next_timestamp>old_timestamp) {
      timestamp= next_timestamp;
      process_changefile(cft_MINUTELY,minutely_sequence_number,timestamp);
      minutely_sequence_number--;
      next_timestamp= get_changefile_timestamp(
					       cft_MINUTELY,minutely_sequence_number);
    }
  }

  // get and process hourly diff files from last hourly timestamp
  // backward; stop just before last daily timestamp or OSM
  // file timestamp has been reached;
  if(hourly_timestamp!=0) {
    next_timestamp= timestamp;
    while(next_timestamp>daily_timestamp &&
	  next_timestamp>old_timestamp) {
      timestamp= next_timestamp;
      process_changefile(cft_HOURLY,hourly_sequence_number,timestamp);
      hourly_sequence_number--;
      next_timestamp= get_changefile_timestamp(
					       cft_HOURLY,hourly_sequence_number);
    }
  }

  // get and process daily diff files from last daily timestamp
  // backward; stop just before OSM file timestamp has been reached;
  if(daily_timestamp!=0) {
    next_timestamp= timestamp;
    while(next_timestamp>old_timestamp) {
      timestamp= next_timestamp;
      process_changefile(cft_DAILY,daily_sequence_number,timestamp);
      daily_sequence_number--;
      next_timestamp= get_changefile_timestamp(
					       cft_DAILY,daily_sequence_number);
    }
  }

  // process remaining files which may still wait in the cache;
  process_changefile((changefile_type_t)0,0,0);
  
  if(main_return_value==0 && loglevel>0)
    PINFO("Completed successfully.")
      
      return main_return_value;
}  // end   main()


