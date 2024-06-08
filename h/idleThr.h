/*
 * idleThr.h
 *
 *  Created on: Aug 8, 2020
 *      Author: OS1
 */

#ifndef IDLETHR_H_
#define IDLETHR_H_

#include"thread.h"

class idleThr:public Thread {
public:
	idleThr():Thread(1024,1){}
protected:
	void run();
};

#endif /* IDLETHR_H_ */
