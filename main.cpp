#include "Driver.h"
#include "testsDato.h"
#include "testsTabla.h"
#include "testsDB.h"
#include "testsDriver.h"

#include <iostream>

int main(){
	testsDato(1, NULL);
	testsTabla(1, NULL);
	testsDB(1, NULL);
	driverTest(1, NULL);
	return 1;
}
