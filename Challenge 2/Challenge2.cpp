/*
	26/4/2018
	Made by Alejandro Pinel Martínez
	Tuenti Challenge 8
	Challenge 2 - Hidden numbers
*/

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

//Function to get the data if the file exits
bool GetData (string testfile, int & ncases, int * & length) {
   int MIN_LENGTH = 2;
   int MAX_LENGTH = 26;
   bool error = false;

   //Open the test file
   fstream inputfile;
   inputfile.open (testfile.c_str() , ios::in);

   //Check if the file exists
   if (inputfile.is_open()){

      inputfile >> ncases;


         length = new int [ncases];
         string code;

         //For each case, we only care for the length of the code
         for (int i = 0; i < ncases && !error; i++) {
            inputfile >> code;
            length[i] = code.length();
            if (length[i] < MIN_LENGTH || length[i] > MAX_LENGTH)
               error = true;
         }

      inputfile.close();
   }
   else
      error = true;

   return error;
}

//A class that let us use very long numbers
class HyperLongNumber {
private:
   static const int MAX_LENGTH = 50;
   int number[MAX_LENGTH];

public:
   HyperLongNumber () {
      for (int i = 0; i < MAX_LENGTH; i++)
         number[i] = 0;
   }

   void Reset () {
     for (int i = 0; i < MAX_LENGTH; i++)
        number[i] = 0;
   }

   int GetDigit (int position) const {
     return number[position];
   }

   void AddInt (int other_number) {
      bool Ci = false;
      int i = 0;
      while ((other_number > 0 || Ci == true) && i < MAX_LENGTH){
         number[i] += other_number % 10;
         other_number /= 10;

         if (Ci)
            number[i]++;

         if (number[i] > 9) {
            number[i] -= 10;
            Ci = true;
         }
         else
            Ci = false;

         i++;
      }
   }

   void SubInt (int other_number) {

      bool Ci = false;
      int i = 0;

      while ((other_number > 0 || Ci) && i < MAX_LENGTH){
         number[i] -= other_number % 10;
         other_number /= 10;

         if (Ci)
            number[i]--;

         if (number[i] < 0) {
            number[i] += 10;
            Ci = true;
         }
         else
            Ci = false;

         i++;
      }
   }

   void MultiplyInt (int other_number) {
      int Ci = 0;
      int i = 0;
      int multiple;

      int number2[MAX_LENGTH];
      for (int i = 0; i < MAX_LENGTH; i++)
         number2[i] = number[i];

      while (other_number > 0){
         multiple = other_number % 10;
         other_number /= 10;

         for (int j = 0; j < MAX_LENGTH; j++){
           if (i == 0){
             number[j] *= multiple;
             number[j] += Ci;
             Ci = number[j] / 10;
             number[j] = number[j] % 10;
           }
           else if (j < MAX_LENGTH - 1) {
             number[j + i] += number2[j] * multiple;
             number[j + i] += Ci;
             Ci = number[j + i] / 10;
             number[j + i] = number[j + 1] % 10;
           }

         }
         i++;
      }
   }

   void MultiplyPowInt (int base, int exponent) {
     for (int i = 0; i < exponent; i++)
        MultiplyInt(base);
   }

   void AddHyperLongNumber (HyperLongNumber other_number) {
      bool Ci = false;

      for (int i = 0; i < MAX_LENGTH; i++){
         number[i] += other_number.GetDigit(i);

         if (Ci)
            number[i]++;

         if (number[i] > 9) {
            number[i] -= 10;
            Ci = true;
         }
         else
            Ci = false;
      }
   }

   void SubHyperLongNumber (HyperLongNumber other_number) {
      bool Ci = false;

      for (int i = 0; i < MAX_LENGTH; i++){
         number[i] -= other_number.GetDigit(i);

         if (Ci)
            number[i]--;

         if (number[i] < 0) {
            number[i] += 10;
            Ci = true;
         }
         else
            Ci = false;
      }
   }

   string String () {
      string str_number = "";
      int i = MAX_LENGTH - 1;

      while (number[i] == 0 && i > 0)
         i--;


      while (i >= 0) {
         str_number = str_number + to_string(number[i]);
         i--;
      }

      return str_number;
   }

};

//Function that calculate the difference between the maximum and minimum value that
//the code can be
string CalculateDifference (int length) {
   HyperLongNumber difference;
   HyperLongNumber factor;

   for (int i = 0; i < length; i++) {
     factor.AddInt(i);
     factor.MultiplyPowInt(length, i);

     difference.AddHyperLongNumber(factor);
     factor.Reset();
   }

   factor.AddInt(1);
   factor.MultiplyPowInt(length, length - 1);
   difference.SubHyperLongNumber(factor);
   factor.Reset();

   for (int i = 0; i < length - 2; i++) {
     factor.AddInt(length - i - 1);
     factor.MultiplyPowInt(length, i);

     difference.SubHyperLongNumber(factor);
     factor.Reset();
   }

   return difference.String();
}

//Function that shows in screen the results
void ShowData (int ncases, string * results) {
   for (int i = 0; i < ncases; i++)
      cout << "Case #" << i + 1 << ": " << results[i] << endl;
}


int main () {

	const string FILENAME = "submitInput";
   int ncases;
   int * length;

   //Get the data
   bool posible_error = GetData(FILENAME, ncases, length);

   //If the data is correct
   if (!posible_error){
      //We reserve memory to archive the results
      string * results = new string [ncases];

      for (int i = 0; i < ncases; i++)
         results[i] = CalculateDifference(length[i]);

      ShowData(ncases, results);

      delete [] results;
   }
   else
      cout << "Test file is missing, the program can't do anything without it :(\n";

   delete [] length;

   return 0;
}
