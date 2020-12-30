#include <iostream>
#include <vector>
using namespace std;

char terminal[10];
char noTerminal[10];
vector<string>proce;

int main(){
    cout<<"请输入终结符："<<endl;
    char x;
    int num=0;
    do
    {
        cin>>x;
        terminal[num]=x;
        num++;
    }while(cin.peek()!='\n');
    // for(int i =0;i<10;i++)
    //     cout <<terminal[i]<<endl;
    cout<<"请输入非终结符："<<endl;
    num=0;
    do
    {
        cin>>x;
        noTerminal[num]=x;
        num++;
    }while(cin.peek()!='\n');
    cout<<"输入产生式集合（空字用‘@’表示）,以‘end’结束:"<<endl;
    string pro;
     while(cin>>pro&&pro!="end")
     {
         string ss;
         ss+=pro[0];
         for(int i=3;i<pro.size();i++)
         {
            ss+=pro[i];
         }
    }
}