//일단 기본적은 메뉴 출력까지만 해보자
//메뉴는 I,D,R,N,P,Q
//Quit을 하기 전에는 코드가 계속해서 돌아가야 한다
//insert를 어떻게 짜면 될까
//들어오면 element로 받아서
//h -> heap[index]에 넣기
//메뉴 입력받을때 라인으로 입력받아서 여백 지우는 것 필요
//Insert할 때 Line으로 입력받는 것 필요

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
            capacity = 2;
            arr = new element[capacity];
        }

        ~Heap() {
            delete[] arr;
        }

        void insert(element e){
            if(size == capacity){
                resize();
            }
            arr[size] = e;
            heapifyUp(size);
            size++;
            cout << "New element [" << e.name << ", " << e.score << ", " << e.className << "] has been inserted.\n" << endl;
        }

        void deleteNode(){
            if(size <= 1){
                cout << "Heap is empty" << endl;
                return;
            }
            element maxNode = maxium();
            cout << "Deleted element: [" << maxNode.name << ", " << maxNode.score << ", " << maxNode.className << "]" << endl;
            swap(maxNode, arr[size]);
            size--;
            heapifyDown(1);
        }

        void retrieveMaxNode(){
            element maxNode = maxium();
            cout << "Element with the largest key: [" << maxNode.name << ", " << maxNode.score << ", " << maxNode.className << "]" << endl;
            cout << endl;
        }

        void showHeap(){
            cout << "Current queue elements:" << endl;
            for(int i=1; i<size; i++){
                cout << i << ". [" << arr[i].name << ", " << arr[i].score << ", " << arr[i].className << "]" << endl;
            }
            cout << endl;
        }

        void heapifyUp(int index){
            while(index > 1 && arr[index].score > arr[index/2].score){
                swap(arr[index], arr[index/2]);
                index /= 2;
            }
        }

        void heapifyDown(int index){
            int largest = index;
            int left = index * 2;
            int right = index * 2 + 1;

            if(left < size && arr[left].score > arr[largest].score){
                largest = left;
            }
            if(right < size && arr[right].score > arr[largest].score){
                largest = right;
            }
            if(largest != index){
                swap(arr[index], arr[largest]);
                heapifyDown(largest);
            }
        }

        element maxium(){
            return arr[1];
        }
};

void showMenu(void);

int main (void){
    Heap h;

    string choice;
    bool running = true;

    while(running){
        showMenu();
        cout << "Choose menu: ";
        cin >> choice;

        if(choice == "I" || choice == "i"){
            element e;
            cout << "Enter the name of the student: ";
            cin >> e.name;
            cout << "Enter the score of the element: ";
            cin >> e.score;
            cout << "Enter the class name: ";
            cin >> e.className;
            h.insert(e);
        }else if(choice == "D" || choice == "d"){
            h.deleteNode();
        }else if(choice == "R" || choice == "r"){
            h.retrieveMaxNode();
        }else if(choice == "N" || choice == "n"){
            cout << "N 입니다" << endl;
        }else if(choice == "P" || choice == "p"){
            h.showHeap();
        }else if(choice == "Q" || choice == "q"){
            cout << "Program terminated." << endl;
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