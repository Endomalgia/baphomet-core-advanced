/*

  Baphomet Configuration

*/

/* Extra Program */
#include <X11/XF86keysym.h>

/* System Macros */

#define BAPHOMET_ALLOW_DEBUG  TRUE

/* ///////////////////////////// PROJECT CONFIG ///////////////////////////// */

const char* program_name                         = "Bael - 01";
const char* program_identifier                   = "com.example.bael";


/* ////////////////////////////////// SDL3 ////////////////////////////////// */

/* General */


/* Window */
static const uint default_window_width           = 640;
static const uint default_window_height          = 480;

/* ////////////////////////////// COMMAND LINE ////////////////////////////// */

const char* argp_program_bug_address             = "<autumn.hazel91@gmail.com>";
const char* argp_program_version                 = "baphomet v0.0.0a 'Bael'"; // Linear order from the lesser key of solomon
static char argp_program_desc[]                  = "a.out -- Test test";
static char argp_program_args_desc[]             = "[ARGS]";
static struct argp_option argp_program_options[] = {
  {"verbose",   'v',  0,  0,  "Produce debug related output"},
  {"debug",     'd',  0,  OPTION_ALIAS },
  {"quiet",     'q',  0,  0,  "Produce no output"},
  { 0 }};
