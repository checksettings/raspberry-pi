#ifndef SHELL_H
#define SHELL_H

#define MAXNUMBEROFFUNCTIONS 32

typedef void (*fcn_ptr)(void); 

extern void shell(void);
extern void addNewCommand(fcn_ptr function_pointer, char command_name[24],char* help_text);

extern void newcommandstoadd(void);

#endif /* SHELL_H */
