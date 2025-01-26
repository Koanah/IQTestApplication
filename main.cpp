#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<random>
#include<algorithm>
#include<cmath>
#include<set>
#include<chrono>
#include<thread>
#include<windows.h>

using namespace std;

struct Question{
  string question;
  vector<string>options;
  char answer;
};

//global variables;
int total=0;
int score=0;
int que=0;

//Reading questions from a file
vector<Question> readQuestionsFromFile(const string& filename){
  ifstream file(filename);//opening/reading file
  vector<Question> theQuestions;
  string line;

  while(getline(file,line)){
    Question aQuestion;//holds the question,options and answer
    //find 0-10. and take substring after position/index 3 of the current line
    aQuestion.question=line.substr(line.find_first_of("0123456789.")+3);

    for(int i=0;i < 4; i++){
      getline(file,line);
      //take substring after position 3 of line
      aQuestion.options.push_back(line.substr(line.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")+3));
      }
    getline(file,line);
    string corr=line.substr(8);
    aQuestion.answer=corr[0];
    theQuestions.push_back(aQuestion);
    }

    file.close();//close file
    return theQuestions;
}

//display Instructions to the user

void welcome(){
  //Read and print out file instruction
  string filename="C:\\Users\\Student\\Desktop\\Comp315 Project\\Running Code\\Files\\Instructions.txt";
  string line;
  ifstream file(filename);

  while(getline(file,line)){
    cout<<line<<endl;
  }
  string bar;
  cout<<"press any key then enter to begin"<<endl;
  cin>>bar;
}

//time fuction

//reading files for each level and setting num of questions for each level
vector<Question> levelQuestions(string filename,int numQuestions){

  srand(time(0));//seed for using random

  vector<Question>questions=readQuestionsFromFile(filename);//stores all quetions in file
  vector<int> indices(questions.size());//for randomizing question selection

  for(unsigned long int i=0;i<questions.size();i++){
    indices[i]=i;//vector indices will have n= total num of questions in each level
  }
  random_shuffle(indices.begin(),indices.end());//random shuflling vector indices

  vector<Question> levelQuestion;//stores the randomized quetions

  for(int i=0;i<numQuestions;i++){
    levelQuestion.push_back(questions[indices[i]]);//getting questions at random index
  }

  return levelQuestion;//returning n=numQuestions questions in a vector
}



//Display questions and options to the user
void displayQuestion(const Question& q){

  cout<<endl;
  ::que++; //incrementing que to keep track of current question being displayed
  cout<<"\t"<<"\t                 Question "<<que<<" of 37"<<endl;
  cout<<"\t"<<"\t                 Score:"<<::score<<"/"<<::total<<endl;
  cout<<endl;

//prints out the question to be answered
  cout<<q.question<<endl;

  //for t/f questions
  if(q.question.substr(0,4)=="True"){
    for(int i=0;i<2;i++){
      cout<<static_cast<char>('a'+i)<<")"<<q.options[i]<<endl;
    }
    //For 4 option questions
    }else{
      for(int i=0;i<4;i++){
        cout<<static_cast<char>('a'+i)<<")"<<q.options[i]<<endl;
      }
    }

}

//score accumulator
void scoreCount(char userAnswer,char QuestionAnswer){
  if((toupper(userAnswer))==QuestionAnswer){
    ::score+=1;
  }
    ::total+=1;
    system("cls");

}

//returns the score of user
int getScore(){
  return ::score;
}

//getting user answer

char getAnswer(){
  char userAnswer;
  cout<<"Answer: ";cin>>userAnswer;
  return userAnswer;
}

//Calculating IQ level of the user
 double getIqScore(int userScore){
  userScore=::score;
  ::total=double(total); //type casting tatal into double to obtain a proper mean
  double mean=total/2;

  //calculating variance and stdviation
  double stdev=(total/(total-1))*(pow(1-mean,2));
  stdev=sqrt(stdev);

  //getting IQ score
  double z=(score-mean)/stdev;
  double IqScore=(z*15)+100;

  return std::round(IqScore);
}

struct player{
    string name;
    int IqScore;
    player(const string& n,int score): name(n),IqScore(score) {}
    bool operator==(const player& other)const{
      return name==other.name;
}
};

//for the set in the leaderBoard

bool operator > (const player& obj1,const player& obj2){
  return obj1.IqScore > obj2.IqScore;
}

//creating player to play game
player createPlayer(){
  string name;
  cout<<"Please enter your name: ";
  cin>>name;

return player(name,0);//initialize player with IQ score zero
}


//attempting the quiz

player play(const player& thePlayer){
  //files containing questiions of each level
  string filename1 = "C:\\Users\\Student\\Desktop\\Comp315 Project\\Running Code\\Files\\EASYTF.txt";
  string filename2 = "C:\\Users\\Student\\Desktop\\Comp315 Project\\Running Code\\Files\\MEDIUMS.txt";
  string filename3 = "C:\\Users\\Student\\Desktop\\Comp315 Project\\Running Code\\Files\\Diffi.txt";

  //vectors of questions for each level
  vector<Question> level1=levelQuestions(filename1,10);
  vector<Question> level2=levelQuestions(filename2,12);
  vector<Question> level3=levelQuestions(filename3,15);



  //vector for all randomimize and limited questions for each level
  vector<vector<Question>> levels={level1,level2,level3};

  for (unsigned long int level=0;level<levels.size();level++){
    vector <Question> currentLevel=levels[level];

    for (const auto& question : currentLevel){
      displayQuestion(question);
      scoreCount(getAnswer(),question.answer);
    }

    if(level<levels.size()-1){
      cout<<"Level "<<level+2<<" unlocked: Press any key and enter to continue "<<endl;
      string bar;
      cin>>bar;
    }

  }

  player updatedPlayer=thePlayer;
  updatedPlayer.IqScore=getIqScore(getScore());

  if(updatedPlayer.IqScore<100){
    cout<<endl;
    cout<<"Your IQ is below average,you can do better than that!!"<<endl;
  }
  else if(updatedPlayer.IqScore==100){
    cout<<endl;
    cout << "\"Don't settle for average.Bring your best to the moment\""<< endl;
    cout<<"-Angela Bassette"<<endl;
  }
  else{
    cout<<endl;
    cout<<"Well done King/Queen, keep slaying :)"<<endl;
  }

  cout<<"Thank You for playing"<<endl;
  cout<<endl;
  cout<<endl;

  return updatedPlayer;
}

//playing and computing the leaderboard using a set
template <class T>



int getIndex(set<player,std::greater<player>> s,const T& tofind){
   int index=1;



   for(const auto& u:s){
    if(u==tofind){
        return index;
    }
      index++;



   }
    return -1;//element not found
}
set<player,std::greater<player>>leads (set<player,std::greater<player>>& leader){
  player playing=createPlayer();
  playing=play(playing);



  leader.insert(playing);
  cout<<"\t\t LEADERBOARD"<<endl;
  cout<<"Name"<<"\t\t\t IQ Score"<<endl;



  for(const auto& obj : leader){
    cout<<getIndex(leader,obj)<<". "<<obj.name<<"\t\t\t "<<obj.IqScore<<endl;
  }
  cout<<endl;
  cout<<endl;



  return leader;



}

class guessGame{
    private :
        string oriWord,scrWord,inpWord,intWord;
        int oriNum,inpNum,point=0;
    public:
        template <typename g>
        void match(g ori,g inp){
            if(ori==inp){
                cout<<"Your answer is correct"<<endl;
                point+=1;
            }
            else{
                cout<<"Sorry try again next time"<<endl;
            }
        }

        void scrambleWordGame(){
            vector<string> words = {"great","wild","sunny","night","month"};
            string filename="C:\\Users\\Student\\Desktop\\Comp315 Project\\Running Code\\Files\\Scrambled.txt";
            string line;
            ifstream file(filename);

    while(getline(file,line)){
    cout<<line<<endl;
  }

            cout<<"  "<<endl;
            for(int i=0; i<3; i++){
                int num = words.size();
                srand(time(0));
                int randomNum = rand() % num;
                oriWord = words[randomNum];
                string temp = oriWord;
                std::random_shuffle(temp.begin(), temp.end());
                scrWord = temp;
                cout<<"  "<<endl;
                cout<<"Scrambled word is : "<<scrWord<<endl;
                cout<<"Correct word : ";
                cin>>inpWord;
                match(oriWord,inpWord);
            }
            cout<<"  "<<endl;
            cout<<"Game is finished thank you for playing"<<endl;
            cout<<endl;

        }
        void numberStringGame(){
            cout<<" Game Two"<<endl;
            cout<<endl;
            vector<string> words = {"vibe","god","comp","good","study"};
            string filename="C:\\Users\\Student\\Desktop\\Comp315 Project\\Running Code\\Files\\StringGameFile.txt";
            string line;
            ifstream file(filename);

    while(getline(file,line)){
    cout<<line<<endl;
  }
    cout << endl;



    int totalWords = words.size();
    int correctAnswers = 0;



    for (int i = 0; i < 1; i++) {
        srand(time(0));
        int randomNum = rand() % totalWords;
        string word = words[randomNum];
        string number = "";



        for (char c : word) {
            switch (c) {
                case 'a':
                case 'b':
                case 'c':
                    number += '2';
                    break;
                case 'd':
                case 'e':
                case 'f':
                    number += '3';
                    break;
                case 'g':
                case 'h':
                case 'i':
                    number += '4';
                    break;
                case 'j':
                case 'k':
                case 'l':
                    number += '5';
                    break;
                case 'm':
                case 'n':
                case 'o':
                    number += '6';
                    break;
                case 'p':
                case 'q':
                case 'r':
                case 's':
                    number += '7';
                    break;
                case 't':
                case 'u':
                case 'v':
                    number += '8';
                    break;
                case 'w':
                case 'x':
                case 'y':
                case 'z':
                    number += '9';
                    break;
            }
        }



        cout << "String: " << word << endl;
        cout << "Enter the corresponding number: ";



        string input;
        cin >> input;



        if (input == number) {
            cout << "Correct!" << endl;
            correctAnswers++;
        }
        else {
            cout << "Incorrect. The correct number is: " << number << endl;
        }



        cout << endl;
    }
        }
};


int main(){
  set<player,std::greater<player>> mySet;

  for(int i=0;i<4;i++){
    welcome();
    mySet=leads(mySet);
    guessGame m;
    m.scrambleWordGame();
    m.numberStringGame();
    if(i==2){
        i=4;
    }
    ::que=0;
    ::score=0;
    ::total=0;

  }
    return 0;


}
