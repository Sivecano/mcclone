//
// Created by sivecano on 02/05/2021.
//

#ifndef MCCLONE_KERNELS_H
#define MCCLONE_KERNELS_H


void cudainit();
void register_blockbuffer(int buffer);
void unregister_blockbuffer();
void cube_facemask();


#endif //MCCLONE_KERNELS_H
