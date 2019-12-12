#ifndef DEFINE_REDIRECT_H
#define DEFINE_REDIRECT_H

int redirect(int fd1,int fd2);

int redirect_filename(int fd_std,char *filename,int flags);

char *getfilename(char **args,int i, char *symbol);

void clear_used_args(char **args,int i,char *symbol);

void remove_blanks(char **args);

int parse_redirects(int *bk_pointer, char **args);

int endredirect(int *backup_fd);

#endif
