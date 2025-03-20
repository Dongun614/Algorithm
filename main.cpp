//22000712 조동운

//Reference
//Heap sort LMS 강의
//Heap sort 강의 Slide 5-8p, 10p, 16p, 19p, 20p, 24-26p, 29-30p
//Heap sort 강의 Slide 32p, 34-36p
//Blog URL: https://kbj96.tistory.com/15

#include <iostream>
#include <cctype>
#include <string>
#include <algorithm>

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

        element extractMax(){
            if(size <= 1){
                cout << "Cannot delete from an empty queue." << endl;
                return {"", -1, ""}; //빈 element 리턴하는데 점수에 -1을 넣어서 리턴
            }
            element maxNode = arr[1];
            //cout << "Deleted element: [" << maxNode.name << ", " << maxNode.score << ", " << maxNode.className << "]" << endl;
            swap(arr[1], arr[size-1]);
            size--;
            heapifyDown(1);

            return maxNode;
        }

        void retrieveMaxNode(){
            if(size == 1){
                cout << "Queue is empty." << endl;
                return;
            }
            element maxNode = maxium();
            cout << "Element with the largest key: [" << maxNode.name << ", " << maxNode.score << ", " << maxNode.className << "]" << endl;
            cout << endl;
        }

        void showHeap(){
            if(size == 1){
                cout << "Queue is empty." << endl;
                return;
            }
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

        void increaseKey(){
            if(size == 1){
                cout << "Queue is empty." << endl;
                return;
            }
            int index; // 사용자에게 받은 인덱스 값 저장하는 변수
            while(true){
                cout << "Enter the index of the element: ";
                cin >> index;
                cin.ignore();
                if(index <= 0){
                    cout << "Index is start by 1, try again.." << endl;
                    continue;
                }

                if(index > 0 && index < size){
                    break;
                }
                cout << "Your input is out of size (size: " << size-1 << ")" << endl;
            }

            int temp; // 사용자에게 받은 점수 임시로 저장할 변수
            while(true){
                cout << "Enter the new score: ";
                cin >> temp;
                cin.ignore();
                
                if (temp < 0 || temp > 100) {
                    cout << "Invalid score. Please enter a valid integer between 0 and 100." << endl;
                    continue;
                }

                if (temp >= arr[index].score || arr[index].score == 100) {
                    arr[index].score = temp;
                    cout << "Key updated. [" << arr[index].name << ", " << arr[index].score << ", " << arr[index].className << "] has been repositioned in the queue." << endl;
                    heapifyUp(index);
                    break;
                } else {
                    cout << "New score should be larger than the current score. Please enter again." << endl;
                }
            }
        }
};

void showMenu(void);
string trim(const string& str);

int main (void){
    Heap h;

    string choice;

    while(true){
        showMenu();
        cout << "Choose menu: ";
        getline(cin, choice);
        choice = trim(choice);

        if(choice == "I" || choice == "i"){
            element e;
            int temp;
            cout << "Enter the name of the student: ";
            getline(cin, e.name);
            while(true){
                cout << "Enter the score of the element: ";
                cin >> temp;
                cin.ignore();
                if(temp >= 0 && temp <= 100){
                    e.score = temp;
                    break;
                }
                cout << "Invalid score. Please enter a valid integer between 0 and 100." << endl;
            }
            cout << "Enter the class name: ";
            getline(cin, e.className);
            h.insert(e);
        }else if(choice == "D" || choice == "d"){
            element result = h.extractMax();
            if(result.score != -1) {
                cout << "Deleted element. [" << result.name << ", " << result.score << ", " << result.className << "]" << endl;
            }
        }else if(choice == "R" || choice == "r"){
            h.retrieveMaxNode();
        }else if(choice == "N" || choice == "n"){
            h.increaseKey();
        }else if(choice == "P" || choice == "p"){
            h.showHeap();
        }else if(choice == "Q" || choice == "q"){
            cout << "Program terminated." << endl;
            break;
        }else cout << "You enter wrong input.. Try again..\n" << endl;
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

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}