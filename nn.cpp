#include<iostream>

 using namespace std;

int main(){
    string mystring = {"-121²+12421²"};
    cout<<mystring<<endl;
    cout<<mystring.find("²")<<endl;
    cout<<stod(mystring)<<endl;
    cout<<mystring.max_size()<<endl;
    return 0;
}