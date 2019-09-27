/*
	28/4/2018
	Made by Alejandro Pinel Martínez
	Tuenti Challenge 8
	Challenge 6 - Button Hero
*/

#include <iostream>
#include <fstream>

using namespace std;

class Note {
private:
   int score;
   int pos_1;
   int pos_2;

public:
   Note (int x, int l, int v, int s) {
      score = s;
      pos_1 = x / v;
      pos_2 = (x + l) / v;
   }

   Note () {
      score = 0;
      pos_1 = 0;
      pos_2 = 0;
   }

   int GetScore (){
      return score;
   }

   int GetPos1 (){
      return pos_1;
   }

   int GetPos2 (){
      return pos_2;
   }

   void SetScore (int new_score) {
      score = new_score;
   }
};

class Song {
private:
   Note * notes;
   int nnotes;

   void ChangePosition (int pos1, int pos2) {
      Note new_note;
      new_note = notes[pos1];
      notes[pos1] = notes[pos2];
      notes[pos2] = new_note;
   }

   void FusionNotes (int pos1, int pos2) {
      notes[pos1].SetScore(notes[pos1].GetScore() + notes[pos2].GetScore());
      for (int i = pos2; i < nnotes - 1; i++) {
         notes[i] = notes[i + 1];
      }
      nnotes--;
   }

public:
   Song () {
      nnotes = 0;
      notes = 0;
   }

   ~Song (){
      delete [] notes;
   }

   void Inicializate (int new_nnotes) {
      if (nnotes == 0){
         nnotes = new_nnotes;
         notes = new Note [nnotes];
      }
   }

   int GetNNotes () const {
      return nnotes;
   }

   Note GetNote (int pos) const {
      return notes[pos];
   }

   int GetNotesInTime (int time, Note * & notesintime) {
      int numberfoundnotes;

      int firstfound = 0;
      while (firstfound <  nnotes && time >= notes[firstfound].GetPos1())
         firstfound++;

      if (firstfound < nnotes){
         int newtime = notes[firstfound].GetPos1();
         numberfoundnotes = 1;
         while (firstfound + numberfoundnotes < nnotes && notes[firstfound + numberfoundnotes].GetPos1() == newtime)
            numberfoundnotes++;
         notesintime = new Note [numberfoundnotes];
         for (int i = 0; i < numberfoundnotes; i++)
            notesintime[i] = notes[firstfound + i];
      }
      else
         numberfoundnotes = 0;



      return numberfoundnotes;
   }

   void AddNote (Note new_note, int pos){
      notes[pos] = new_note;
   }

   void Order () {

      for (int i = 1; i < nnotes; i++) {
         int j = 0;
         bool can_continue = true;
         while ((i - j) > 0 && can_continue) {
            if (notes[i - j].GetPos1() == notes[i - j - 1].GetPos1() && notes[i - j].GetPos2() == notes[i - j - 1].GetPos2()){
               FusionNotes(i - j -1, i - j);
               i --;
            }
            else if (notes[i - j].GetPos1() < notes[i - j - 1].GetPos1() || notes[i - j].GetPos1() == notes[i - j - 1].GetPos1()
               && notes[i - j].GetPos2() < notes[i - j - 1].GetPos2()){
                  ChangePosition(i - j, i - j - 1);
                  j++;
            }
            else
               can_continue = false;
         }
      }
   }
};


class Step {
private:
   int time;
   int score;
   Step * next;

public:

   Step(int ntime, int nscore){
      time = ntime;
      score = nscore;
      next = 0;
   }

   int GetTime(){
      return time;
   }

   int GetScore(){
      return score;
   }

   Step * GetNext(){
      return next;
   }

   void SetNext (Step * newstep){
      next = newstep;
   }

   void SetScore(int nscore){
      score = nscore;
   }

   void Add (Step * newstep) {
      if (next != 0){
         Step * previous = this;
         Step * nnext = next;
         while (nnext != 0 && newstep -> GetTime() > nnext -> GetTime()){
            previous = nnext;
            nnext = nnext -> GetNext();
         }
         newstep -> SetNext(nnext);
         previous -> SetNext(newstep);
      }
      else
            next = newstep;

   }

   bool Update (int ntime, int nscore) {
      bool found = false;
      if (next != 0) {
         Step * nnext = next;
         while (nnext != 0 && !found){
            if (nnext -> GetTime() == ntime) {
               found = true;
               if (nscore > nnext -> GetScore())
                  nnext -> SetScore(nscore);
            }
            else
               nnext = nnext -> GetNext();
         }
      }

      return found;
   }

};

//Function to get the data if the file exits
bool GetData (string testfile, int & ncases, Song * & song) {
   int MIN_CASES = 1;
   int MAX_CASES = 100;
   bool error = false;

   //Open the test file
   fstream inputfile;
   inputfile.open (testfile.c_str() , ios::in);

   //Check if the file exists
   if (inputfile.is_open()){

      inputfile >> ncases;

      if (ncases >= MIN_CASES && ncases <= MAX_CASES){
        song = new Song [ncases];
        int nnotes, x, l, v, s;

        for (int i = 0; i < ncases; i++){
           inputfile >> nnotes;
           song[i].Inicializate(nnotes);

           for (int j = 0; j < nnotes; j++){
             inputfile >> x >> l >> v >> s;
             Note new_note(x,l,v,s);
             song[i].AddNote(new_note, j);
          }

          song[i].Order();
        }
      }

      inputfile.close();
   }
   else
      error = true;

   return error;
}

int MaximumScore (Song * song) {
   Step * step = new Step(0, 0);
   Step * newstep;
   int actual_maximum = 0;

   while (step != 0) {

      int time = step -> GetTime();
      Note * possibility;
      int notesintime = song -> GetNotesInTime(time, possibility);
      bool update;

      if (notesintime > 0) {
         for (int i = 0; i < notesintime; i++){
            update = step -> Update(possibility[i].GetPos2(), step -> GetScore() + possibility[i].GetScore());
            if (!update) {
               newstep = new Step(possibility[i].GetPos2(), (step -> GetScore()) + possibility[i].GetScore());
               step -> Add(newstep);
            }
         }
         time = possibility[0].GetPos1();
         update = step -> Update(time, step -> GetScore());
         if (!update) {
            newstep = new Step(time, (step -> GetScore()));
            step -> Add(newstep);
         }
      }
      else {
         if (step -> GetScore() > actual_maximum)
            actual_maximum = step -> GetScore();
      }
      newstep = step;
      step = step -> GetNext();
      delete newstep;
   }
   while (step != 0){
      newstep = step;
      step = step -> GetNext();
      delete newstep;
   }
   return actual_maximum;
}

//Show the results on screen
void ShowResults (int result, int ncase) {
      cout << "Case #" << ncase + 1 << ": " << result << endl;
}

int main () {
	const string FILENAME = "submitInput";
   int ncases;
   Song * song;

   //Get the data
   bool posible_error = GetData(FILENAME, ncases, song);

   //If the data is correct
   if (!posible_error){

      int result;
      for (int i = 0; i < ncases; i++){
         result = MaximumScore(song + i);
         ShowResults(result, i);
      }


   }
   else
      cout << "Test file is missing, the program can't do anything without it :(\n";


   return 0;
}
