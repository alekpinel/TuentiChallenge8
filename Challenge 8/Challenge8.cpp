/*
	28/4/2018
	Made by Alejandro Pinel Martínez
	Tuenti Challenge 8
	Challenge 8 - Button Hero
*/

#include <iostream>
#include <fstream>
#include "BigInteger.h"

using namespace std;


class Door {
private:
   BigInteger  period;
   BigInteger  next_open;
   Door * next;

public:

   Door (BigInteger  P, BigInteger  T){
      period = P;
      next_open = T;
      next = 0;
   }

   BigInteger  GetPeriod(){
      return period;
   }

   BigInteger  GetNextOpen(){
      return next_open;
   }

   Door * GetNext(){
      return next;
   }

   void SetNext (Door * newdoor){
      next = newdoor;
   }

};

class Hallway {
private:
   Door * firstdoor;

   BigInteger MCD (BigInteger  x, BigInteger  y) {
      BigInteger a;
      if (y > x){
         a = x;
         x = y;
         y = a;
      }
      do {
        a = y;
        y = x % y;
        x = a;
     } while(y != 0);

     return a;
   }

   BigInteger  MCM (BigInteger  x, BigInteger  y) {
      BigInteger a;
      if (y > x){
         a = x;
         x = y;
         y = a;
      }

      return (x/MCD(x,y)) *y;
   }

   BigInteger Inv (BigInteger number, BigInteger modulus) {
      BigInteger r, r1, q, v, v1, v2, a;
      r = number;
      r1 = modulus;
      v1 = 0;
      v = 1;
      while (r > 1){
         q = r1 / r;
         a = r;
         r = r1 % r;
         r1 = a;
         v2 = v1;
         v1 = v;
         v = (v2 + (modulus - (v1 * q) % modulus)) % modulus;
      }
      return v;
   }


   //Method that fusion the first and the second door
   bool Fusion (BigInteger  n_fusion) {
      bool ispossible = true;
      n_fusion++;
      Door * seconddoor = firstdoor -> GetNext();
      BigInteger  P1 = firstdoor -> GetPeriod();
      BigInteger  P2 = seconddoor -> GetPeriod();
      BigInteger  T1 = firstdoor -> GetNextOpen();
      BigInteger  T2 = seconddoor -> GetNextOpen();
      BigInteger mcd = MCD(P1,P2);

      BigInteger a = P1 - T1;
      BigInteger b = ((P2 - (n_fusion % P2)) + (P2 - T2) + (P2 - a));
      if (mcd != 1) {
         if ((b%mcd == 0)){
            P2 /= mcd;
            b /= mcd;
         }
         else
            ispossible = false;
      }


      if (ispossible) {
         BigInteger NP1 = Inv(P1/mcd, P2);

         BigInteger new_T = a + P1 * ((b * NP1) % P2);
         BigInteger new_period = P1 * P2;

         firstdoor -> SetNext(seconddoor -> GetNext());
         delete seconddoor;
         seconddoor = firstdoor;
         firstdoor = new Door(new_period, new_period - new_T);
         firstdoor -> SetNext(seconddoor -> GetNext());
         delete seconddoor;
      }
      return ispossible;
   }

public:

   Hallway (){
      firstdoor = 0;
   }

   ~Hallway (){
      Door * old_door;
      while (firstdoor != 0){
         old_door = firstdoor;
         firstdoor = firstdoor -> GetNext();
         delete old_door;
      }
   }

   Door * GetFirstDoor () const {
      return firstdoor;
   }

   void Add (Door * newdoor) {
      if (firstdoor != 0){
         Door * nnext = firstdoor;
         while (nnext -> GetNext() != 0)
            nnext = nnext -> GetNext();
         nnext -> SetNext(newdoor);
      }
      else
         firstdoor = newdoor;
   }

   //Method to fusion all the doors in a single one.
   BigInteger  CalculateTime () {
      bool ispossible = true;
      if (firstdoor != 0){
         BigInteger  n_door = 0;
         while (firstdoor -> GetNext() != 0 && ispossible) {
            ispossible = Fusion(n_door);
            n_door++;
         }
      }
      if (ispossible)
         return firstdoor -> GetPeriod() - firstdoor -> GetNextOpen();
      else
         return -1;
   }
};

//Function to get the data if the file exits
bool GetData (string testfile, int & ncases, Hallway * & hallway) {;
   bool error = false;

   //Open the test file
   fstream inputfile;
   inputfile.open (testfile.c_str() , ios::in);

   //Check if the file exists
   if (inputfile.is_open()){

      inputfile >> ncases;
      hallway = new Hallway [ncases];

      int ndoors, P, T;
      BigInteger Big_P, Big_T;
      for (int i = 0; i < ncases; i++){
         inputfile >> ndoors;
         for (int j = 0; j < ndoors; j++){
            inputfile >> P >> T;
            Big_P = P;
            Big_T = T;
            Door * newdoor = new Door(Big_P, Big_T);
            hallway[i].Add(newdoor);
         }

      }

      inputfile.close();
   }
   else
      error = true;

   return error;
}

//Show the results on screen
void ShowResults (BigInteger  result, int ncase) {
      cout << "Case #" << ncase + 1 << ": ";
      if (result != -1)
         cout << ((string) result);
      else
         cout << "NEVER";
      cout << endl;
}

int main () {
	const string FILENAME = "submitInput";
   int ncases;
   Hallway * hallway;

   //Get the data
   bool posible_error = GetData(FILENAME, ncases, hallway);

   //If the data is correct
   if (!posible_error){

      BigInteger  moment;
      for (int i = 0; i < ncases; i++){
         moment = hallway[i].CalculateTime();
         ShowResults(moment, i);
      }

   }
   else
      cout << "Test file is missing, the program can't do anything without it :(\n";


   return 0;
}
