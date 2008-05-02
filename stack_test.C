#include <stdio.h>
#include <assert.h>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <fstream>


#include "source_location.h"

using namespace std;



int main(){

  cout << "int:" << endl;
  cout << "Source location = " <<  source_location_int() << endl;
  cout << "Source location = " <<  source_location_int() << endl;
  cout << "Source location = " <<  source_location_int() << endl;
  cout << "Source location = " <<  source_location_int() << endl;
  cout << "Source location = " <<  source_location_int() << endl;

  cout << "ulong:" << endl;
  cout << "Source location = " <<  source_location_ulong() << endl;
  cout << "Source location = " <<  source_location_ulong() << endl;
  cout << "Source location = " <<  source_location_ulong() << endl;
  cout << "Source location = " <<  source_location_ulong() << endl;

}
