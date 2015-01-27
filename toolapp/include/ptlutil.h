#ifndef _PTLUTIL_H
#define _PTLUTIL_H

#include "ptlconst.h"
#include "jansson.h"


void StrToLower(char *str);
void StrToUpper(char *str);
void Trim(char *str);
void RTrim(char *str);
void LTrim(char *str);
void GetCurrentTime(char *sCurrentTime);
void GetLocalTime(char *sCurrentTime);
int tPflGetString(char *vspFileName,char *key,char *vspValue,int nLen);
int tPflGetInt(char *vspFileName,char *key,int *vspValue);
int tPflGetDouble(char *vspFileName,char *key,double *vspValue);
json_t *tPflGetJsonObj(char *vspFileName,char *key);
long CovertHourToSec(double hr);

#endif /*_PTLUTIL_H*/
