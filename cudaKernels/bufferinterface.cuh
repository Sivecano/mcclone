//
// Created by sivecano on 02/05/2021.
//

#ifndef MCCLONE_KERNELS_H
#define MCCLONE_KERNELS_H


void cudainit();
void register_buffer(unsigned int buffer);
void unregister_buffer(unsigned int buffer);
void cube_facemask(unsigned int blockbuffer);


#endif //MCCLONE_KERNELS_H
