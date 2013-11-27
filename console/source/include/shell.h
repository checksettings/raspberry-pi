#ifndef SHELL_H
#define SHELL_H

#define MAXNUMBEROFFUNCTIONS 32

typedef void (*fcn_ptr)(void); 
extern fcn_ptr commands[MAXNUMBEROFFUNCTIONS];

extern char *commands_name[MAXNUMBEROFFUNCTIONS];
extern void shell(void);

#endif /* SHELL_H */
