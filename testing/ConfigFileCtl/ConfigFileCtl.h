#ifndef _LINKC_CONFIG_FILE_CONTROL_
#define _LINKC_CONFIG_FILE_CONTROL_

#include "../../server/include/linkc_utilities.h"   // For ReadLine
#include "../../server/include/def.h"               // For Some ErrorCode
#include <stdio.h>                                  // For FILE , fopen and so on
#define STD_BUFFER_SIZE 128
// QUestion : It's always find the key first and then read the configuration!
// update the requestions please

int ReadConfigFile(FILE *fp, char *Key, char *Value);
/*
 * this function can just read configure file
 * Args:
 *      [1] FILE *fp        an opend File point,must have promise to read!
 *      [2] char *Key       The Key is a point have been malloced the momory
 *      [3] char *Value     The Value of the key now have been read
 *
 *      The Key & Value's Size is Defined As [STD_BUFFER_SIZE]
 *
 *
 * Returns:
 *      [1] LINKC_SUCCESS   Success
 *      [2] EOF             End of File
 */

 int FindKey(FILE *fp,char *Key);
/*
 * this function is used to find key in the configure file
 * After Finding
 *      [1] if there is no such key you want to find,the file point will be reset to the beginning
 *      [2] if find the key,the file point will be reset to this line's beginning
 *          so you can just call ReadConfigFile() to Read the Key & Value you want to find
 * Args:
 *      [1] FILE *fp        an opened File point,must have promise to read!
 *      [2] char *Key       the key which you want to find
 *
 * Returns:
 *      [1] LINKC_SUCCESS   Success
 *      [2] LINKC_FAILURE   Failure
 */

#endif
