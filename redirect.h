#ifndef DEFINE_REDIRECT_H
#define DEFINE_REDIRECT_H

void remove_blanks(char **args);

int redirect(int *bk_pointer, char **args);

int endredirect(int *backup_fd);

#endif
