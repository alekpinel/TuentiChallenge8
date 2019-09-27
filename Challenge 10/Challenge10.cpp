/*
	16/2/2019
	Made by Alejandro Pinel Martínez
	Tuenti Challenge 8
	Challenge 10 - Dance
*/

#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <utility>

#include "BigIntegerLibrary.hh"

using namespace std;

const long int MODULE = 1000000007;

class Dance;

class FilteredList {
private:
  vector<list<pair<Dance, BigInteger> > > lists;

public:
  FilteredList() {
    lists.resize(500);
  }

  FilteredList(const FilteredList & other) {
    lists = other.lists;
  }

  void Insert(const Dance & dance, const BigInteger result);
  BigInteger Search(const Dance & dance) const;
  void Clear();

};

//////////////////////////////// Dance ////////////////////////////////

class Dance {
public:
   struct Hate {
       int hater[2];
   };
private:

   static FilteredList * memory;
   int n_people;
   int n_hates;
   Hate * hates;

   void copy(const Dance & other) {
      this -> memory = other.memory;
      this -> n_people = other.n_people;
      this -> n_hates = other.n_hates;
      hates = new Hate[n_hates];
      for (int i = 0; i < n_hates; i++) {
         hates[i] = other.hates[i];
      }
   }

public:
   Dance(int n_people = 0, int n_hates = 0) {
      initialize(n_people, n_hates);
   }

   Dance (const Dance & other) {
      copy(other);
   }

   ~Dance() {
   	if (n_hates > 0)
      	delete [] hates;
   }

   int GetNPeople() const{
     return n_people;
   }

   Dance & operator=(const Dance & other) {
      copy(other);
      return *this;
   }

   bool operator==(const Dance & other) const {
      bool equal = (this -> n_people == other.n_people) && (this -> n_hates == other.n_hates);
      int i = 0;
      while (equal && i < n_hates) {
         equal = (this -> hates[i].hater[0] == other.hates[i].hater[0] && this -> hates[i].hater[1] == other.hates[i].hater[1])
            || (this -> hates[i].hater[1] == other.hates[i].hater[0] && this -> hates[i].hater[0] == other.hates[i].hater[1]);
         i++;
      }
      return equal;
   }

   void initialize (int n_people, int n_hates) {
      this -> n_people = n_people;
      this -> n_hates = n_hates;
      if (n_hates > 0)
      	hates = new Hate[n_hates];
   }

   void addHate(int index, int hater, int hated) {
   	hates[index].hater[0] = hater;
   	hates[index].hater[1] = hated;
   }

   static void setMemory(FilteredList * mem) {
      memory = mem;
   }

   bool areHaters(int hater1, int hater2) const {
      bool arehaters = false;
      int i = 0;
      while (i < n_hates && !arehaters) {
         if ((hates[i].hater[0] == hater1 && hates[i].hater[1] == hater2) || (hates[i].hater[0] == hater2 && hates[i].hater[1] == hater1))
            arehaters = true;
         else
            i++;
      }

      return arehaters;
   }

   struct DanceDivided {
      Dance * left;
      Dance * right;
   };

   DanceDivided divide(int person1, int person2) {
      //Organize the hate
      bool * hateright = new bool[n_hates];
      bool * hateleft = new bool[n_hates];
      int n_hatesr = 0, n_hatesl = 0;
      for (int i = 0; i < n_hates; i++) {
         if (hates[i].hater[0] > person1 && hates[i].hater[0] < person2 &&
               hates[i].hater[1] > person1 && hates[i].hater[1] < person2) {
            hateright[i] = true;
            n_hatesr++;
         }
         else
            hateright[i] = false;

         if ((hates[i].hater[0] < person1 || hates[i].hater[0] > person2) &&
               (hates[i].hater[1] < person1 || hates[i].hater[1] > person2)) {
            hateleft[i] = true;
            n_hatesl++;
         }
         else
            hateleft[i] = false;
      }
      DanceDivided toreturn;
      toreturn.right = new Dance(person2 - person1 - 1, n_hatesr);
      toreturn.left = new Dance(n_people - (person2 - person1 - 1) -2, n_hatesl);
      int indexr = 0, indexl = 0;
      for (int i = 0; i < n_hates; i++) {
         if (hateright[i]) {
            toreturn.right -> addHate(indexr, hates[i].hater[0] - person1 - 1, hates[i].hater[1] - person1 - 1);
            indexr++;
         }
         else if (hateleft[i]) {
            toreturn.left -> addHate(indexl, hates[i].hater[0] - person2 - 1, hates[i].hater[1] - person2 - 1);
            indexl++;
         }
      }

      return toreturn;
   }

   BigInteger posibilities() {
      BigInteger n_posibilities;

      if (n_people == 0)
         n_posibilities = 1;
      else if (n_people % 2 == 1)
         n_posibilities = 0;
      else if (n_people == 2)
         n_posibilities = areHaters(0, 1)? 0: 1;
      else {
         n_posibilities = memory -> Search(*this);

         if (n_posibilities == -1) {
            n_posibilities = 0;
            BigInteger p;
            for (int i = 1; i < n_people; i += 2) {
               if (!areHaters(0, i)) {
                  DanceDivided newdances = divide(0 , i);
                  p = (newdances.right -> posibilities())%MODULE;
                  if (p > 0) {
                     BigInteger aux = newdances.left -> posibilities()%MODULE;
                     p = (p * (aux))%MODULE;
                  }
                  n_posibilities =(n_posibilities + p)%MODULE;

                  delete newdances.left;
                  delete newdances.right;
               }
            }
            memory -> Insert(*this, n_posibilities);
         }
      }
      return n_posibilities;
   }

   friend ostream & operator<<(ostream & f, const Dance & d);
   friend ifstream & operator>>(ifstream & f, Dance & d);

};

ostream & operator<<(ostream & f, const Dance & d) {
   f << d.n_people << " " << d.n_hates << endl;
   for (int i = 0; i < d.n_hates; i++)
      f << d.hates[i].hater[0] << " " << d.hates[i].hater[1] << endl;

   return f;
}

ifstream & operator>>(ifstream & f, Dance & d) {
   int n_people, n_hates, hater1, hater2;
   f >> n_people >> n_hates;
   d.initialize(n_people, n_hates);
   for (int j = 0; j < n_hates; j++) {
      f >> hater1 >> hater2;
      d.addHate(j, hater1, hater2);
   }

   return f;
}

//////////////////////////////// FilteredList ////////////////////////////////

void FilteredList::Insert(const Dance & dance, const BigInteger result) {
    unsigned index = dance.GetNPeople();
    if (lists.size() < index) {
      lists.resize(lists.size() * 2);
    }
    pair<Dance, BigInteger> newpair;
    newpair.first = dance;
    newpair.second = result;
    lists[index].push_back(newpair);
}

BigInteger FilteredList::Search(const Dance & dance) const{
  unsigned index = dance.GetNPeople();
  BigInteger n_posibilities = -1;
  for (list<pair<Dance, BigInteger> >::const_iterator i = lists[index].begin(); i != lists[index].end() && n_posibilities == -1; i++)
     if ((*i).first == dance) {
        n_posibilities = (*i).second;
     }

  return n_posibilities;
}

void FilteredList::Clear() {
  for (unsigned i = 0; i < lists.size(); i++)
    lists[i].clear();
}

//////////////////////////////// Obtaining the data ////////////////////////////////

bool GetData(const char * pathname, Dance * & dances, int & n_cases) {
   ifstream file;
   file.open(pathname);
   bool error = !file.is_open();
   if (!error){
     file >> n_cases;

     dances = new Dance[n_cases];
     for (int i = 0; i < n_cases; i++){
        file >> dances[i];
     }

     file.close();
   }


   return error;
}

FilteredList * Dance::memory;
int main() {
   const char pathname[] = "submitInput.txt";
   int n_cases;
   Dance * dances;
   FilteredList Memory;
   Dance::setMemory(&Memory);
   bool error = GetData(pathname, dances, n_cases);

   if (!error) {

     ofstream output("output.txt");
     BigInteger result;
     for (int i = 0; i < n_cases; i++) {
        result = dances[i].posibilities();
        Memory.Clear();
        cout << "Case #" << i + 1 << ": " << result << "\n";
        output << "Case #" << i + 1 << ": " << result << "\n";
     }

     output.close();
   }
   else {
     cout << "The file is missing\n";
   }

    return 0;
}
