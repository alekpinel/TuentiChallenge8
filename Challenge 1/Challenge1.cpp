/*
	25/4/2018
	Made by Alejandro Pinel Martínez
	Tuenti Challenge 8
	Challenge 1 - Waffle love
*/

#include <iostream>
#include <fstream>

using namespace std;

//Function to get the data if the file exits
bool GetData (string testfile, int & ncases, int * & vertical, int * & horizontal){
   const int N_MIN_CASES = 1;
   const int N_MAX_CASES = 100;
   bool error = false;

   //Open the test file
   fstream inputfile;
   inputfile.open (testfile.c_str() , ios::in);

   //Check if the file exists
   if (inputfile.is_open()){
      inputfile >> ncases;
      //Check if the number of cases is in the interval
      if (ncases >= N_MIN_CASES && ncases <= N_MAX_CASES) {
         vertical = new int [ncases];
         horizontal = new int [ncases];

         for (int i = 0; i < ncases; i++)
            inputfile >> vertical[i] >> horizontal[i];
      }
      else
         error = true;

      inputfile.close();
   }
   else
      error = true;

   return error;
}

int main () {
   const int N_MIN_LINES = 2;
   const int N_MAX_LINES = 10000;
	const string FILENAME = "submitInput.txt";
   int ncases;
   int * vertical;
   int * horizontal;

   //Get the data
   bool posible_error = GetData(FILENAME, ncases, vertical, horizontal);

   //If the data is correct
   if (!posible_error){

      int nholes;
      for (int i = 0; i < ncases; i++) {
         //Check if the data fits in the interval
         if (vertical[i] <= N_MAX_LINES && vertical[i] >= N_MIN_LINES && horizontal[i] <= N_MAX_LINES && horizontal[i] >= N_MIN_LINES) {

            //Calculate the holes and show it on screen
            nholes = (vertical[i] - 1) * (horizontal[i] -1);
            cout << "Case #" << i + 1 << ": " << nholes << endl;
         }
         else
            cout << "Case #" << i << ": Can't be done because the data doesn't fit in the interval\n";
      }

   }
   else
      cout << "Test file is missing, the program can't do anything without it :(\n";

   return 0;
}
