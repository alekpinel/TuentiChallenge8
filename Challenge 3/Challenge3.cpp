/*
	27/4/2018
	Made by Alejandro Pinel Martínez
	Tuenti Challenge 8
	Challenge 3 - Scales
*/

#include <iostream>
#include <fstream>

using namespace std;

//Return the position of the input tone
int PositionTone (string tone) {
  string search1 [12] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};
  string search2 [12] = {"A", "Bb", "Cb", "B#", "Db", "D", "Eb", "Fb", "E#", "Gb", "G", "Ab"};

  int position = -1;
  int i = 0;
  while (i < 12 && position == -1){
    if (tone == search1 [i])
      position = i;
    i++;
  }

  i = 0;
  while (position == -1 && i < 12){
    if (tone == search2 [i])
      position = i;
    i++;
  }

  return position;
}

//Function to get the data if the file exits
bool GetData (string testfile, int & ncases, bool * * & tones) {
   int MIN_CASES = 0;
   int MAX_CASES = 1000;
   bool error = false;

   //Open the test file
   fstream inputfile;
   inputfile.open (testfile.c_str() , ios::in);

   //Check if the file exists
   if (inputfile.is_open()){

      inputfile >> ncases;

      if (ncases >= MIN_CASES && ncases <= MAX_CASES){
        tones = new bool * [ncases];
        int ntones;
        string singletone;

        for (int i = 0; i < ncases; i++) {
          tones[i] = new bool [12];

          for (int j = 0; j < 12; j++){
            tones[i][j] = false;
          }

          inputfile >> ntones;

          for (int j = 0; j < ntones; j++) {
            inputfile >> singletone;
            tones[i][PositionTone(singletone)] = true;
          }
        }
      }

      inputfile.close();
   }
   else
      error = true;

   return error;
}

//Function that calculates which tones are in each scale
void CalculatePosibleScales (bool * * posiblescale){

   bool ScaleMA[12] = {1,0,1,0,1,1,0,1,0,1,0,1};
   bool ScalemA[12] = {1,0,1,1,0,1,0,1,1,0,1,0};

   for (int i = 0; i < 12; i++){
      posiblescale[0][i] = ScaleMA[i];
      posiblescale[12][i] = ScalemA[i];
   }

   for (int i = 1; i < 12; i++){
      posiblescale[i][0] = posiblescale[i - 1][11];
      posiblescale[i + 12][0] = posiblescale[i + 11][11];
      for (int j = 1; j < 12; j++){
         posiblescale[i][j] = posiblescale[i - 1][j - 1];
         posiblescale[i + 12][j] = posiblescale[i + 11][j - 1];
      }
   }

}

//Function that compares each case with each scale
void FindScale (bool * * tones, int ncases, bool * * scales) {

   bool * * posiblescales = new bool * [24];
   for (int i = 0; i < 24; i++)
      posiblescales[i] = new bool [12];

   CalculatePosibleScales(posiblescales);

   for (int i = 0; i < ncases; i++){
      for (int j = 0; j < 24; j++){
         int k = 0;
         bool scaleisvalid = true;
         while(scaleisvalid && k < 12){
            if(tones[i][k] && !posiblescales[j][k])
               scaleisvalid = false;
            else
               k++;
         }

         scales[i][j] = scaleisvalid;

      }

   }

   for (int i = 0; i < 24; i++)
      delete [] posiblescales[i];
   delete [] posiblescales;
   posiblescales = 0;

}

//To show the results on screen
void ShowScale (bool * * scales, int ncases){
   string scalesname [12] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};
   char mayororminor;
   bool none;

   for (int i = 0; i < ncases; i++) {
      cout << "Case #" << i + 1 << ":";
      none = true;

      for (int j = 0; j < 24; j++) {
         if (scales[i][j]){
            if (j < 12)
               mayororminor = 'M';
            else
               mayororminor = 'm';

            cout << " "<< mayororminor << scalesname[j % 12];
            none = false;
         }
      }
      if (none)
         cout << " None";
      cout << "\n";
   }
}

int main () {

	const string FILENAME = "submitInput";
   int ncases;
   bool * * tones;

   //Get the data
   bool posible_error = GetData(FILENAME, ncases, tones);

   //If the data is correct
   if (!posible_error){

     bool * * scales = new bool * [ncases];
     for (int i = 0; i < ncases; i++) {
        scales[i] = new bool [24];
     }


     FindScale (tones, ncases, scales);

     ShowScale (scales, ncases);


     for (int i = 0; i < ncases; i++){
        delete [] scales[i];
        delete [] tones[i];
     }
     delete [] scales;
     scales = 0;
     delete [] tones;
     tones = 0;
   }
   else
      cout << "Test file is missing, the program can't do anything without it :(\n";


   return 0;
}
