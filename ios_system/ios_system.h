//
//  ios_system.h
//  ios_system
//
//  Created by Nicolas Holzschuch on 04/12/2017.
//  Copyright © 2017 Nicolas Holzschuch. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

//! Project version number for ios_system.
FOUNDATION_EXPORT double ios_systemVersionNumber;

//! Project version string for ios_system.
FOUNDATION_EXPORT const unsigned char ios_systemVersionString[];

// Thread-local input and output streams
extern __thread FILE* thread_stdin;
extern __thread FILE* thread_stdout;
extern __thread FILE* thread_stderr;
extern __thread void* thread_context;

// rust doesn't support extern __thread vars yet
// see https://github.com/rust-lang/rust/issues/30795
// so we provide function accessors for them.
extern FILE* ios_stdin(void);
extern FILE* ios_stdout(void);
extern FILE* ios_stderr(void);
extern void* ios_context(void);

// set to true to have more commands available, more debugging information.
extern bool sideLoading;
// set to false to have the main thread run in detached mode (non blocking)
extern bool joinMainThread;

extern int ios_executable(const char* inputCmd); // does this command exist? (executable file or builtin command)
extern int ios_system(const char* inputCmd); // execute this command (executable file or builtin command)
extern FILE *ios_popen(const char *command, const char *type); // Execute this command and pipe the result
extern int ios_kill(void); // kill the current running command
extern int ios_killpid(pid_t pid, int sig); // kill the current running command
extern int chdir(const char* path);

extern int ios_isatty(int fd); // test whether a file descriptor refers to a terminal
extern pthread_t ios_getLastThreadId(void);
extern pthread_t ios_getThreadId(pid_t pid);
extern void ios_storeThreadId(pthread_t thread);
extern void ios_releaseThread(pthread_t thread);
extern void ios_releaseThreadId(pid_t pid);
extern pid_t ios_currentPid(void);
extern int ios_getCommandStatus(void);
extern const char* ios_progname(void);
extern char * ios_getenv(const char *name);
extern int ios_setenv(const char* variableName, const char* value, int overwrite);
int ios_unsetenv(const char* variableName);
extern char** environmentVariables(pid_t pid);
extern NSArray* environmentAsArray(void);
extern void storeEnvironment(char* envp[]);
extern pid_t ios_fork(void);
extern void ios_waitpid(pid_t pid);
extern pid_t ios_full_waitpid(pid_t pid, int *stat_loc, int options);
extern NSString *ios_getLogicalPWD(const void* sessionId);
void ios_setWindowSize(int width, int height, const void* sessionId);

extern NSString* commandsAsString(void);
extern NSArray* commandsAsArray(void);      // set of all commands, in an NSArray
extern NSArray* aliasesAsArray(void);       // set of all aliases defined, in an NSArray
extern NSString* aliasedCommand(NSString* command); // if the command is aliased, return the command it points to
extern NSString* getoptString(NSString* command);
extern NSString* operatesOn(NSString* command);
extern void initializeEnvironment(void);
extern int ios_setMiniRoot(NSString*);  // restricts operations to a certain hierarchy
extern int ios_setMiniRootURL(NSURL*);  // restricts operations to a certain hierarchy
extern int ios_setAllowedPaths(NSArray<NSString *> *paths);  // restricts operations to a certain hierarchy
extern void ios_setBookmarkDictionaryName(NSString*);  // name of the dictionary in user preferences, holding the bookmarks.
extern void ios_switchSession(const void* sessionid);
extern void ios_closeSession(const void* sessionid);
extern void ios_setStreams(FILE* _stdin, FILE* _stdout, FILE* _stderr);
extern void ios_settty(FILE* _tty);
extern int ios_getstdin(void);
extern int ios_gettty(void);
extern int ios_activePager(void);
extern void ios_setContext(const void *context);
extern const void* ios_getContext(void);
extern void ios_setDirectoryURL(NSURL* workingDirectoryURL);
extern void newPreviousDirectory(void);
extern void makeGlobal(void);
extern void makeLocal(void);
extern void replaceCommand(NSString* commandName, NSString* functionName, bool allOccurences);
extern NSError* addCommandList(NSString* fileLocation);
extern NSArray* backgroundCommandList;
extern int numPythonInterpreters;
extern int numPerlInterpreters;
extern int numTeXInterpreters;
extern int cd_main(int argc, char** argv);
extern const char* ios_getBookmarkedVersion(const char* p);
extern void ios_stopInteractive(void);
extern void finishedPreparingWebAssemblyCommand(void);
extern int webAssemblyCommandOrder(void);

// ============================================================================
// TRACE HOOKS (M1-I1: Structured tracing for debugging)
// ============================================================================

typedef enum {
    ASHELL_TRACE_NONE = 0,
    ASHELL_TRACE_ERROR,
    ASHELL_TRACE_WARN,
    ASHELL_TRACE_INFO,
    ASHELL_TRACE_DEBUG
} ashell_trace_level_t;

// Set global trace level
extern void ashell_set_trace_level(ashell_trace_level_t level);
extern ashell_trace_level_t ashell_get_trace_level(void);

// Trace with format string
extern void ashell_trace(ashell_trace_level_t level, const char* fmt, ...);

// Specialized trace hooks
extern void ashell_trace_command_start(const char* cmd, pid_t pid);
extern void ashell_trace_command_end(const char* cmd, pid_t pid, int status);
extern void ashell_trace_session_switch(const void* from_session, const void* to_session);
extern void ashell_trace_io_operation(const char* op, size_t bytes);
extern void ashell_trace_error(const char* context, const char* error);

// Enable/disable trace categories
extern void ashell_trace_enable_category(const char* category);
extern void ashell_trace_disable_category(const char* category);

// Trace categories
#define ASHELL_TRACE_CAT_COMMANDS   "commands"
#define ASHELL_TRACE_CAT_SESSIONS   "sessions"
#define ASHELL_TRACE_CAT_IO         "io"
#define ASHELL_TRACE_CAT_ENV        "environment"
