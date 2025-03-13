//일단 기본적은 메뉴 출력까지만 해보자
//메뉴는 I,D,R,N,P,Q

#include <iostream>
using namespace std;

void showMenu(void);

int main (void){
    char choice;
    showMenu();

    //여기에 스위치 문
    cout << "Choose menu: ";
    cin >> choice;
    cout << choice << endl;

    return 0;
}

void showMenu(void){
    cout << "***********MENU****************" << endl;
    cout << "I : Insert a new element into the queue." << endl;
    cout << "D : Delete the element with the largest kev from the queue." << endl;
    cout << "R : Retrieve the element with the largest kev." << endl;
    cout << "N : Increase the kev of an element in the queue." << endl;
    cout << "P : Print all elements in the queue." << endl;
    cout << "Q : Quit." << endl;
}