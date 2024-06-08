/*
 * main.cpp
 *
 *  Created on: Aug 8, 2020
 *      Author: OS1
 */
#include"stdio.h"
#include"iostream.h"
#include "System.h"

extern int userMain(int argc, char* argv[]);

int main(int argc,char *argv[]){
	System::init();
	int ret = userMain(argc,argv);
	System::restore();
	return ret;
}

