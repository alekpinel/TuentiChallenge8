/*
	28/4/2018
	Made by Alejandro Pinel Martínez
	Tuenti Challenge 8
	Challenge 4 - Brave Knight
*/

#include <iostream>
#include <fstream>

using namespace std;

class Board {
private:
   int rows;
   int columns;
   char * * squares;

public:

   Board (){
      squares = 0;
   }

   ~Board () {
      if (squares != 0) {
            for (int i = 0; i < rows; i++)
               delete [] squares[i];
            delete [] squares;
            squares = 0;
      }
   }

   void Inicialization (int nrows, int ncolumns){
      if (squares == 0){

            rows = nrows;
            columns = ncolumns;

            squares = new char * [rows];
            for (int i = 0; i < rows; i++)
               squares[i] = new char [columns];
      }
   }

   void Fill (char character) {
      for (int i = 0; i < rows; i++)
         for (int j = 0; j < columns; j++)
            AddElement(i ,j, character);
   }

   void CopyFrom (Board * other_board) {
      for (int i = 0; i < rows; i++)
         for (int j = 0; j < columns; j++)
            AddElement(i ,j, other_board -> GetElement(i ,j));
   }

   int Rows () const {
      return rows;
   }

   int Columns () const {
      return columns;
   }

   char GetElement (int row, int column) const {
      if(row >= 0 && row < rows && column >= 0 && column < columns){
         return squares[row][column];
      }
      else
         return '!';
   }

   void Find (char element, int & row, int & column) {
      bool found = false;
      int i = 0;
      while (!found && i < Rows()) {
         int j = 0;
         while (!found && j < Columns()){
            if (GetElement(i, j) == element) {
               row = i;
               column = j;
            }
            j++;
         }
         i++;
      }
   }

   bool SquareIsValid (int row, int column){
      return GetElement (row, column) != '!' && GetElement (row, column) != '#';
   }

   void AddElement (int row, int column, char element){
      squares[row][column] = element;
   }
};

class Step {
private:
   int x;
   int y;
   int n_steps;
   Step * next;

public:

   Step(int nx, int ny, int nn_steps){
         x = nx;
         y = ny;
      n_steps = nn_steps;
      next = 0;
   }

   int GetX(){
      return x;
   }

   int GetY(){
      return y;
   }

   int GetNSteps(){
      return n_steps;
   }

   Step * GetNext(){
      return next;
   }

   void SetNext (Step * newstep){
      next = newstep;
   }

   void Add (Step * newstep) {
      if (next != 0){
         Step * nnext = next;
         while (nnext -> GetNext() != 0)
            nnext = nnext -> GetNext();
         nnext -> SetNext(newstep);
      }
      else
         next = newstep;
   }

};

//Function to get the data if the file exits
bool GetData (string testfile, int & ncases, Board * & board) {
   int MIN_CASES = 1;
   int MAX_CASES = 1000;
   bool error = false;

   //Open the test file
   fstream inputfile;
   inputfile.open (testfile.c_str() , ios::in);

   //Check if the file exists
   if (inputfile.is_open()){

      inputfile >> ncases;

      if (ncases >= MIN_CASES && ncases <= MAX_CASES){
        board = new Board [ncases];
        int rows, columns;
        string board_string;

        for (int i = 0; i < ncases; i++){
           inputfile >> rows;
           inputfile >> columns;

           board[i].Inicialization(rows, columns);
            for (int j = 0; j < rows; j++){
                inputfile >> board_string;
                for (int k = 0; k < columns; k++) {
                   board[i].AddElement(j, k, board_string.at(k));
                }
            }

         }
      }

      inputfile.close();
   }
   else
      error = true;

   return error;
}

//Return if in the given square you arrive to the end
bool Arrives (Board * board, Step * candidate, char final_square){
   bool found;
   int param = 2;
   if (board -> GetElement(candidate -> GetX(), candidate-> GetY()) == '*')
      param = 4;

   int parameter_x [8] = {-param,-param,-param/2,-param/2,param/2,param/2,param,param};
   int parameter_y [8] = {-param/2,param/2,-param,param,-param,param,-param/2,param/2};

   for (int i = 0; i < 8 && !found; i++){
      found = board -> GetElement(candidate -> GetX() + parameter_x[i], candidate -> GetY() + parameter_y[i]) == final_square;
   }

   return found;
}

//Search the path to the goal
int SearchPath (Board * board, Board * checked_board, char final_square, int start_x, int start_y) {
   int nsteps = -1;
   bool found = false;
   Step * old_ptr;
   Step * ptr_step = new Step(start_x, start_y, 0);
   checked_board -> AddElement(start_x, start_y, 'X');

   int parameter_x [8] = {-2,-2,-1,-1,1,1,2,2};
   int parameter_y [8] = {-1,1,-2,2,-2,2,-1,1};
   int param;


   while (ptr_step != 0){
      found = Arrives (board, ptr_step, final_square);
      if (!found) {
         int x = ptr_step -> GetX();
         int y = ptr_step -> GetY();

         if (board -> GetElement(x, y) == '*')
            param = 2;
         else
            param = 1;

         for (int i = 0; i < 8; i++){
            int new_x = x + parameter_x[i] * param;
            int new_y = y + parameter_y[i] * param;
            if (board -> SquareIsValid(new_x, new_y) && checked_board -> GetElement(new_x, new_y) != 'X'){
                  checked_board -> AddElement(new_x, new_y, 'X');
                  Step * new_step = new Step(new_x, new_y, (ptr_step -> GetNSteps()) + 1);
                  ptr_step -> Add (new_step);
            }
         }

         old_ptr = ptr_step;
         ptr_step = ptr_step -> GetNext();
         delete old_ptr;
      }
      else {
         nsteps = (ptr_step -> GetNSteps()) + 1;

         while (ptr_step != 0){
            old_ptr = ptr_step;
            ptr_step = ptr_step -> GetNext();
            delete old_ptr;
         }
      }
   }

   return nsteps;
}

//Return the minimum distance to archive the challenge
int CalculateDistance (Board * board) {
   Board * checked_board = new Board;
   int distance1, distance2;
   checked_board -> Inicialization(board -> Rows(), board -> Columns());
   checked_board -> Fill('O');

   int start_x, start_y;
   board -> Find('S', start_x, start_y);
   distance1 = SearchPath(board, checked_board, 'P', start_x, start_y);

   if (distance1 != -1){
      checked_board -> Fill('O');
      board -> Find('P', start_x, start_y);
      distance2 = SearchPath(board, checked_board, 'D', start_x, start_y);
   }

   delete checked_board;

   if (distance1 != -1 && distance2 != -1)
      return distance1 + distance2;
   else
      return -1;
}

//Show the results on screen
void ShowResults (int results, int ncase) {
      cout << "Case #" << ncase + 1 << ": ";
      if (results != -1)
         cout << results;
      else
         cout << "IMPOSSIBLE";
      cout << endl;
}

int main () {
	const string FILENAME = "submitInput";
   int ncases;
   Board * board;
   int * rows;
   int * columns;

   //Get the data
   bool posible_error = GetData(FILENAME, ncases, board);

   //If the data is correct
   if (!posible_error){

      int result;

      for (int i = 0; i < ncases; i++){
         result = CalculateDistance(board + i);
         ShowResults(result, i);
      }


      delete [] board;
      board = 0;
   }
   else
      cout << "Test file is missing, the program can't do anything without it :(\n";


   return 0;
}
