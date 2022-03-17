#ifndef FLAGS_H
#define FLAGS_H

// max options that can be defined
#ifndef COMMANDER_MAX_OPTIONS
#define COMMANDER_MAX_OPTIONS 32
#endif

// max argument that can be passed
#ifndef COMMANDER_MAX_ARGS
#define COMMANDER_MAX_ARGS 32
#endif

typedef void (* command_callback)(struct command *self);

struct command_option {
  int optional_arg;
  int required_arg;

  char *argname;
  char *large;

  const char *small;
  const char *large_with_arg;
  const char *description;

  command_callback cb;
}

struct command {
  const char *arg;
  const char *name;
  const char *usage;
  const char *version;

  int argc;

  struct command_option options[COMMANDER_MAX_OPTIONS];
  int option_count;

  char *argv[COMMANDER_MAX_ARGS];
  char **nargv;
};


struct command;



// prototypes

void command_init(struct command *self, const char *name, const char *version);

void
command_free(command_t *self);

void
command_help(command_t *self);

void
command_option(command_t *self, const char *small, const char *large, const char *desc, command_callback_t cb);

void
command_parse(command_t *self, int argc, char **argv);

#endif