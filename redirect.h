#ifndef DEFINE_REDIRECT_H
#define DEFINE_REDIRECT_H

enum redirect_type { R_STDOUT, R_STDIN, R_OUTS };

int redirect(int fd,enum redirect_type mode);

int endredirect(int backup_fd,enum redirect_type mode);

#endif
