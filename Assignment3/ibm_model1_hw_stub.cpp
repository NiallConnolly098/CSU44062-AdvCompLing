#include<vector>
#include<string>
#include<iostream>
#include<iomanip>
#include<cmath>

using namespace std;

// want to represents vocab items by integers because then various tables 
// need by the IBM model and EM training can just be represented as 2-dim 
// tables indexed by integers

// the following #defines, defs of VS, VO, S, O, and create_vocab_and_data()
// are set up to deal with the specific case of the two pair corpus
// (la maison/the house)
// (la fleur/the flower)

// S VOCAB
#define LA 0
#define MAISON 1
#define FLEUR 2
// O VOCAB
#define THE 0
#define HOUSE 1
#define FLOWER 2

#define VS_SIZE 3
#define VO_SIZE 3
#define D_SIZE 2


vector<string> VS(VS_SIZE); // S vocab: VS[x] gives Src word coded by x 
vector<string> VO(VO_SIZE); // O vocab: VO[x] gives Obs word coded by x

vector<vector<int> > S(D_SIZE); // all S sequences; in this case 2
vector<vector<int> > O(D_SIZE); // all O sequences; in this case 2

vector<vector<double> > counts(VO_SIZE, vector<double>(VS_SIZE));
vector<vector<double> > probs(VO_SIZE, vector<double>(VS_SIZE));

// sets S[0] and S[1] to be the int vecs representing the S sequences
// sets O[0] and O[1] to be the int vecs representing the O sequences
void create_vocab_and_data(); 

// functions which use VS and VO to 'decode' the int vecs representing the 
// Src and Obs sequences
void show_pair(int d);
void show_O(int d); 
void show_S(int d);

int main() {
  create_vocab_and_data();

  // guts of it to go here
  // you may well though want to set up further global data structures
  // and functions which access them 
  for (int i = 0; i < VO_SIZE; i++){
    for (int j = 0; j < VS_SIZE; j++){
        probs[i][j] = 1 / (double)VS_SIZE;
    }
  }

  for(int i = 0; i < 50; i++){
    for(int j = 0; j < VO_SIZE; j++){
      for(int k = 0; k < VS_SIZE; k++){
        counts[j][k] = 0;
      }
    }

    for(int x = 0; x < D_SIZE; x++){
      for(int y = 0; y < O[x].size(); y++){
        int o = O[x][y];
        double value = 0;
        for(int z = 0; z < S[x].size(); z++){
          int s = S[x][z];
          value += probs[o][s];
        }
        for(int z = 0; z < S[x].size(); z++){
          int s = S[x][z];
          counts[o][s] += probs[o][s] / value;
        }
      }
    }

    for(int x = 0; x < VS_SIZE; x++){
      double value = 0;
      for(int y = 0; y < VO_SIZE; y++){
        value += counts[y][x];
      }
      for(int y = 0; y < VO_SIZE; y++){
        probs[y][x] = counts[y][x] / value;
      }
    }
  }

}

void create_vocab_and_data() {

  VS[LA] = "la";
  VS[MAISON] = "maison";
  VS[FLEUR] = "fleur";

  VO[THE] = "the";
  VO[HOUSE] = "house";
  VO[FLOWER] = "flower";

  cout << "source vocab\n";
  for(int vi=0; vi < VS.size(); vi++) {
    cout << VS[vi] << " ";
  }
  cout << endl;
  cout << "observed vocab\n";
  for(int vj=0; vj < VO.size(); vj++) {
    cout << VO[vj] << " ";
  }
  cout << endl;

  // make S[0] be {LA,MAISON}
  //      O[0] be {THE,HOUSE}
  S[0] = {LA,MAISON};
  O[0] = {THE,HOUSE};

  // make S[1] be {LA,FLEUR}
  //      O[1] be {THE,FLOWER}
  S[1] = {LA,FLEUR};
  O[1] = {THE,FLOWER};

  for(int d = 0; d < S.size(); d++) {
    show_pair(d);
  }
}

void show_O(int d) {
  for(int i=0; i < O[d].size(); i++) {
    cout << VO[O[d][i]] << " ";
  }
}

void show_S(int d) {
  for(int i=0; i < S[d].size(); i++) {
    cout << VS[S[d][i]] << " ";
  }
}

void show_pair(int d) {
  cout << "S" << d << ": ";
  show_S(d);
  cout << endl;
  cout << "O" << d << ": ";
  show_O(d);
  cout << endl;
}

 