//일단 기본적은 메뉴 출력까지만 해보자
//메뉴는 I,D,R,N,P,Q
//Quit을 하기 전에는 코드가 계속해서 돌아가야 한다
//while문을 사용하는데 음

#include <iostream>
#include <cctype>
using namespace std;

void showMenu(void);

int main (void){
    string choice;
    bool running = true;

    while(running){
        showMenu();
        cout << "Choose menu: ";
        cin >> choice;

        if(choice == "I" || choice == "i"){
            cout << "I 입니다" << endl;
        }else if(choice == "D" || choice == "d"){
            cout << "D 입니다" << endl;
        }else if(choice == "R" || choice == "r"){
            cout << "R 입니다" << endl;
        }else if(choice == "N" || choice == "n"){
            cout << "N 입니다" << endl;
        }else if(choice == "P" || choice == "p"){
            cout << "P 입니다" << endl;
        }else if(choice == "Q" || choice == "q"){
            cout << "프로그램을 종료합니다." << endl;
            running = false;
        }else cout << "잘못된 입력입니다.\n" << endl;
    }

    return 0;
}

void showMenu(void){
    cout << "***********MENU****************" << endl;
    cout << "I : Insert a new element into the queue." << endl;
    cout << "D : Delete the element with the largest kev from the queue." << endl;
    cout << "R : Retrieve the element with the largest kev." << endl;
    cout << "N : Increase the kev of an element in the queue." << endl;
    cout << "P : Print all elements in the queue." << endl;
    cout << "Q : Quit.\n" << endl;
}