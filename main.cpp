#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "stdc++.h"

#define AMOUNT_TO_PRINT_PER_ONCE 20
#define AMOUNT_OF_RECORDS 4000

using namespace std;

struct Record {
    unsigned char FIO[32];
    unsigned char street[18];
    short int Hnum;
    short int Rnum;
    unsigned char Date[10];
};


struct Vertex {
    Record data;
    int w = 0;
    Vertex* left = nullptr;
    Vertex* right = nullptr;
};

class Node {
public:
    Node* next = nullptr;
    Record data;
};

std::ostream& operator<<(std::ostream& strm, const Record& data) {
    return strm << setw(32) << data.FIO << setw(18) << data.street << setw(5) << data.Hnum
        << setw(5) << data.Rnum << setw(10) << data.Date << endl;
};

class Queue {
public:
    Queue() {
        head = nullptr;
        tail = head;
    }
	bool isSorted = false;
	unsigned int getSize();
	void addNode(Record record);
	void print();
	Record **buildIndexArray();
	Node *head;
	Node *tail;
};


unsigned int Queue::getSize() {
	int size = 0;
	if (!head) {
		return size;
	}
	else {
		Node *temp = head;
		while (temp) {
			size++;
			temp = temp->next;
		}
	}
	return size;
}

void Queue::addNode(Record record) {
    Node* newnode = new Node;
    newnode->data = record;
    if (!head) {
        head = newnode;
    }
    else {
        Node* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newnode;
        tail = temp->next;
    }
}

void Queue::print() {
    if (!head) {
        return;
    }
    else {
        Node* temp = head;
        bool choiceToContinue = true;
        int i = 1, start_i = i;
        do {
            cout << setw(6) << i << ") " << temp->data;

            if (i == start_i + AMOUNT_TO_PRINT_PER_ONCE - 1 &&
                i != AMOUNT_OF_RECORDS) {
                start_i = i + 1;
                cout << "? 0/1 : ";
                cin >> choiceToContinue;
            }
            i++;
            temp = temp->next;
        } while (temp && choiceToContinue);
    }
}

Queue readFromFile(string path) {
    std::ifstream fin(path, std::ios::binary);
    Queue list;
    int counter = 0;
    while (counter < AMOUNT_OF_RECORDS) {
        Record tempRecord;
        fin.read((char*)&tempRecord, sizeof(tempRecord));
        list.addNode(tempRecord);
        counter++;
    }
    return list;
}

void digitalSort(Node*& S) {
    Node* p;
    int j, i, L = sizeof(p->data.street);
    int d;
    Queue Q[256];
    for (j = L - 1; j >= 0; j--) {
        for (i = 0; i < 256; i++) {
            Q[i].tail = (Node*)&Q[i].head;
        }
        p = S;
        while (p) {
            d = int(p->data.street[j]);
            Q[d].tail->next = p;
            Q[d].tail = p;
            p = p->next;
        }
        p = (Node*)&S;
        for (i = 0; i < 256; i++) {
            if (Q[i].tail != (Node*)&Q[i].head) {
                p->next = Q[i].head;
                p = Q[i].tail;
            }
        }
        p->next = nullptr;
    }

    L = sizeof(p->data.Hnum);
    for (j = L - 1; j >= 0; j--) {
        for (i = 0; i < 256; i++) {
            Q[i].tail = (Node*)&Q[i].head;
        }
        p = S;
        while (p) {
            d = int(p->data.Hnum);
            Q[d].tail->next = p;
            Q[d].tail = p;
            p = p->next;
        }
        p = (Node*)&S;
        for (i = 0; i < 256; i++) {
            if (Q[i].tail != (Node*)&Q[i].head) {
                p->next = Q[i].head;
                p = Q[i].tail;
            }
        }
        p->next = nullptr;
    }

    /* L = sizeof(p->data.Rnum);
     for (j = L - 1; j >= 0; j--) {
         for (i = 0; i < 256; i++) {
             Q[i].tail = (Node*)&Q[i].head;
         }
         p = S;
         while (p) {
             d = int(p->data.Rnum);
             Q[d].tail->next = p;
             Q[d].tail = p;
             p = p->next;
         }
         p = (Node*)&S;
         for (i = 0; i < 256; i++) {
             if (Q[i].tail != (Node*)&Q[i].head) {
                 p->next = Q[i].head;
                 p = Q[i].tail;
             }
         }
         p->next = nullptr;
     }*/
}

std::string charArrayToString(unsigned char* charArray, int size) {
    std::string stringToBeReturned = "";
    for (int i = 0; i < size; i++) {
        stringToBeReturned += charArray[i];
    }
    return stringToBeReturned;
}

Record** Queue::buildIndexArray() {
    Record** newIndexArray = new Record * [AMOUNT_OF_RECORDS];
    Node* temp = this->head;
    int i = 0;
    while (temp && i < AMOUNT_OF_RECORDS) {
        newIndexArray[i++] = &(temp->data);
        temp = temp->next;
    }
    return newIndexArray;
}

bool isLarger(Record record1, Record record2) {
    if (record1.street > record2.street) {
        return true;
    }
    else if (record1.street == record2.street) {
        if (record1.Hnum > record2.Hnum) {
            return true;
        }
        else if (record1.Hnum == record2.Hnum) {
            if (record1.Rnum > record2.Rnum) {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool isEqual(Record record1, Record record2) {
    return (record1.street == record2.street) && (record1.Rnum == record2.Rnum) &&
        (record1.Hnum == record2.Hnum);
}

bool treeSearch(Vertex* p, Record record) {
    while (p) {
        if (isEqual(p->data, record)) {
            return true;
        }
        else if (isLarger(p->data, record)) {
            p = p->left;
        }
        else {
            p = p->right;
        }
    };
    return false;
}

void addToSDPrec(Record record, Vertex*& p) {
    if (!p) {
        p = new Vertex;
        p->data = record;
    }
    else if (isLarger(record, p->data)) {
        addToSDPrec(record, p->right);
    }
    else {
        addToSDPrec(record, p->left);
    }
}

void A1(Vertex*& root, Record** indexArray, int size) {
    int W[size];
    for (int i = 0; i < size; i++) {
        W[i] = rand() % 100;
    };
    int i, j;
    for (i = 0; i < size - 1; i++)
        for (j = 0; j < size - i - 1; j++)
            if (W[j] < W[j + 1]) {
                int tmp = W[j];
                W[j] = W[j + 1];
                W[j + 1] = tmp;
                Record tmpRecord = *indexArray[j];
                *indexArray[j] = *indexArray[j + 1];
                *indexArray[j + 1] = tmpRecord;
            }
    for (int i = 0; i < size; i++) {
        addToSDPrec(*indexArray[i], root);
    }
}

int cnt = 1;

void printFromLeftToRight(Vertex* p) {
    if (p) {
        printFromLeftToRight(p->left);
        cout << setw(6) << cnt++ << ") " << p->data;
        printFromLeftToRight(p->right);
    }
}

double entropy(vector <double> probability) {
    double result = 0;
    for (int i = 0; i < probability.size(); i++) {
        result -= probability[i] * log2(probability[i]);
    };
    return result;
};

vector <char> removeDuplicates(vector<char> s) {
    auto last = s.end();
    for (auto first = s.begin(); first != last; ++first)
    {
        last = std::remove(std::next(first), last, *first);
    }
    s.erase(last, s.end());
    return s;
}

std::string gilbertMooreEncode(std::string path) {
    char charArr[sizeof(Record) * AMOUNT_OF_RECORDS];
    std::ifstream fin(path, std::ios::binary);
    fin.read((char*)&charArr, sizeof(Record) * AMOUNT_OF_RECORDS);
    fin.close();
    std::map <char, int> amount;
    int size = 0;
    for (auto chr : charArr) {
        amount[chr]++;
        size++;
    }

    std::cout << size << std::endl;
    std::vector <double> probability;
    for (const auto& pair : amount) {
        probability.push_back(double(pair.second) / double(size));
    }

    int n = probability.size();
    double Q[n];
    int L[n];
    char C[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = '\0';
        }
    }
    std::sort(probability.begin(), probability.end(), greater<>());
    double pr = 0;
    L[0] = -floor(log2(probability[0]));
    Q[0] = probability[0] / 2;
    for (int i = 1; i < n; i++) {
        double tmp = 0;
        for (int k = 0; k < i; k++)
            tmp += probability[k];
        Q[i] = tmp + probability[i] / 2;
        L[i] = -floor(log2(probability[i])) + 1;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < L[i]; j++) {
            Q[i] *= 2;
            C[i][j] = floor(Q[i]) + '0';
            if (Q[i] > 1) {
                Q[i] -= 1;
            }
        }
    }

    std::map <char, std::string> codeTable;
    double averageLength = 0;
    int i = 0;
    for (const auto& pair : amount) {
        std::cout << pair.first << " -> " << C[i] << " - " << probability[i] << std::endl;
        averageLength += L[i] * probability[i];
        codeTable.insert({ pair.first, C[i] });
        i++;
    }


    std::cout << "Entropy = " << entropy(probability) << std::endl;
    std::cout << "Average L = " << averageLength << std::endl;
    std::string output = "";
    for (auto chr : charArr) {
        output += codeTable[chr];
    }
    return output;
}

void menuForTree(Queue list) {
    Vertex* root = nullptr;
    A1(root, list.buildIndexArray(), list.getSize());
    printFromLeftToRight(root);
    while (true) {
        int yourlist = 0;
        std::cout << "1. Print tree\n2. Tree search\n3. Exit from tree menu\n\n";
        std::cout << ">> ";
        std::cin >> yourlist;
        switch (yourlist) {
        case 1: {
            cnt = 1;
            printFromLeftToRight(root);
            break;
        }
        case 2: {
            std::cout << "Search mode\n";
            std::cout << "Input street >>";
            short c;
            std::cin >> c;
            std::cout << "Input House number >>";
            short d;
            std::cin >> d;
            std::cout << "Input Room number >>";
            unsigned char e[10];
            std::cin >> e;
            Record record{ "", "", c, d };
            for (int i = 0; i < 10; i++) {
                record.street[i] = e[i];
            }
            std::cout << "Searching..." << record << "\n\n";
            if (treeSearch(root, record)) {
                std::cout << "Found\n\n";
            }
            else {
                std::cout << "Not found\n\n";
            }
            break;
        }
        case 3: {
            return;
        }
        }
    }
}

void binarySearch(Record** indexArray, std::string key) {
    int L = 0;
    int R = AMOUNT_OF_RECORDS - 1;
    Queue listWithFound;
    while (L < R) {
        int m = (L + R) / 2;

        if (charArrayToString(indexArray[m]->street, 3) < key) {
            L = m + 1;
        }
        else {
            R = m;
        }
    }
    while (charArrayToString(indexArray[R]->street, 3) == key) {
        listWithFound.addNode(*indexArray[R++]);
    }
    Node* temp = listWithFound.head;
    int number = 1;
    while (temp) {
        cout << setw(6) << number++ << ") " << temp->data;
        temp = temp->next;
    }
    if (number > 1) {
        int yourlist;
        std::cout << "Do you want to see a tree? 0/1 :";
        std::cin >> yourlist;
        if (yourlist) {
            menuForTree(listWithFound);
        }
    }
}

int main()
{
	string db = "testBase4.dat";
	Queue list = readFromFile(db);
	while (true) {
		short int yourlist;
		cout << "1. Print DataBase\n2. Sort DataBase\n3. Binary Search\n4. Encode\n5. Tree Menu\n0. "
			"Exit\n\n";
		cout << ">> ";
		cin >> yourlist;
		switch (yourlist)
		{
			case 1: 
			{
				list.print();
				break;
			}
			case 2: 
			{
				if (!list.isSorted) 
				{
					digitalSort(list.head);
					list.isSorted = true;
					cout << "Sorting...\n";
				}
				else 
				{
					cout << "List already sorted\n";
				}
				break;
			}
			case 3: 
			{
				if (!list.isSorted)
				{
					digitalSort(list.head);
					list.isSorted = true;
				}
				cout << "Input key >> ";
				string key;
				cin >> key;
				binarySearch(list.buildIndexArray(), key);
				break;
			}
			case 4: 
			{
				std::ofstream fout("out.txt");
				std::string out = gilbertMooreEncode("testBase4.dat");
				fout.write(reinterpret_cast <char*> (&out), sizeof(out));
				break;
			}
			case 5: 
			{
				Vertex* root = nullptr;
				A1(root, list.buildIndexArray(), list.getSize());
				printFromLeftToRight(root);
				while (true) {
					int yourlist = 0;
					std::cout << "1. Print tree\n2. Tree search\n3. Exit from tree menu\n\n";
					std::cout << ">> ";
					std::cin >> yourlist;
					switch (yourlist)
					{
					case 1:
					{
						cnt = 1;
						printFromLeftToRight(root);
						break;
					}
					case 2:
					{
						std::cout << "Search mode\n";
						std::cout << "Input street >>";
						char* poisk = new char[3];
						cin >> poisk[0] >> poisk[1] >> poisk[2];
						std::cout << "Input House number >>";
						short d;
						std::cin >> d;
						std::cout << "Input Room number >>";
						short e;
						std::cin >> e;
						Record record{ "", "", poisk[3], d };
						std::cout << "Searching..." << record << "\n\n";
						if (treeSearch(root, record)) {
							std::cout << "Found\n\n";
						}
						else {
							std::cout << "Not found\n\n";
						}
						break;
					}
					case 3:
					{
						return 0;
						break;
					}
					break;
					}
				}
			}
		case 0: 
		{
			return EXIT_SUCCESS;
		}
		default: 
		{
			cerr << "There's no any option with that number\n";
		}
		}
	}
}