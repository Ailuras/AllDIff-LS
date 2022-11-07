#include<iostream>
using namespace std;

int main() {
    int order  = 2;
    string puzzleString = string(order*order*order*order,'.');
    cout<<puzzleString<<endl;
}