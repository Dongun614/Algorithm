//일단 기본적은 메뉴 출력까지만 해보자
//메뉴는 I,D,R,N,P,Q
//Quit을 하기 전에는 코드가 계속해서 돌아가야 한다
//insert를 어떻게 짜면 될까
//들어오면 element로 받아서
//h -> heap[index]에 넣기

#include <iostream>
#include <cctype>
using namespace std;

typedef struct{
    string name;
    int score;
    string className;
} element;

class Heap{
    private:
    element* arr;
    int size;
    int capacity;

    void resize(){
        capacity *= 2;
        element* newArr = new element[capacity];
        for(int i=1; i<size; i++){
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
    }

    public:
        Heap(){
            size = 1;
            capacity = 1;
            arr = new element[capacity];
        }

        ~Heap() {
            delete[] arr;
        }

        void insert(element e){
            if(size == capacity){
                resize();
            }
            arr[size++] = e;
        }
};

void showMenu(void);
void heapInit(Heap* h);
void insert(Heap* h, element e);

int main (void){
    Heap h;

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