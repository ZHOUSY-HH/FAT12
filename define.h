#ifndef DEFINE_H
#define DEFINE_H

#define SIZE_BLOCK 512
#define SIZE_ONEDIR 32
#define SIZE_ROOTDIRNUM 224
#define SIZE_DIRNUM 16

#define BEGIN_DIR 19
#define BEGIN_FAT1 1
#define BEGIN_FAT2 10
#define BEGIN_DATA 33

#define BLOCKNUM_DIR 14
#define BLOCKNUM_FAT 9

#define ATTR_SYS 0x20
#define ATTR_USER 0X10
#define ATTR_HIDE 0x27
#define ATTR_END 0X00
#define ATTR_NONE 0X01

#define LEATH_NAME 8

#define ORDER_DIR 1
#define ORDER_ERROR 2
#define ORDERC_CD 3   

#endif