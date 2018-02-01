#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <string.h>
#include <algorithm>
#include <iterator>
#include <conio.h>
#include <iostream>


using namespace std;
enum Parameters {gen_koef = 16, max_number = 20};
typedef multimap <int,int> TREE;
void OutputTree(TREE& tree, char name);
void include(TREE& tree, int start, int finish);

void Generate(TREE& tree, int count) {

    tree.clear();
    int r = count;
    int num, mask=0;
    for(int i = 0; i < r; ++i) {
        num=rand() % gen_koef + 1;
        if (!(mask&(1<<num)))
        {
            tree.insert(TREE::value_type( num,i));
            mask=mask|(1<<num);
        }
        else i--;
        //cout<<i;
        //getch();
    }
}


void OutputTree(TREE& tree, char name) {

    unsigned int size;
    if(!tree.empty()) {
        size = tree.size();
        //for(int i = 0; i < size; i++) {
            cout << name << ": "<<endl;
            for(auto iterator = tree.cbegin(); iterator != tree.cend(); ++iterator)
                cout << "{" << iterator->second << ";" << iterator->first << "}"<<endl;
            //cout << endl;
        //}
    }
    else
        cout << "Дерево" << " " << name << " " << "пусто." << endl;
}

void OutputConsist(TREE& tree, char name) {

    int size = tree.size();
    if(!tree.empty()) {
        cout << name << ": ";
        for(int i = 0; i < size; i++) {
                for (TREE::const_iterator iterator = tree.cbegin(); iterator != tree.cend(); ++iterator) {
                    if(iterator->second == i) {
                        cout << iterator->first << ' ';
                        break;
                    }
                }
        }
    }
    else
        cout << "Последовательность" << " " << name << " " << "пуста." << endl;
}

TREE Concat(TREE& left, TREE& right) {


    TREE tree;
    //set_intersection( left.cbegin(), left.cend(), right.cbegin(), right.cend(), back_inserter(tree) );
    if(!right.empty()) {
        int position = left.size();
        int size=right.size();
        for (int i=0; i<position; ++i)
        {
            for(TREE::const_iterator iterator = left.cbegin(); iterator != left.cend(); ++iterator) {
                if(iterator->second == i) {
                    tree.insert(TREE::value_type(((*iterator).first),i));
                    break;
                }
            }
        }
        for (int i=position; i<position+size; ++i)
        {
            for(TREE::const_iterator iterator = right.cbegin(); iterator != right.cend(); ++iterator) {
                if(iterator->second == i-position) {
                    tree.insert(TREE::value_type(((*iterator).first),i));
                    break;
                }
            }
        }
    }
    return tree;
}

TREE Subst(TREE& left, TREE& right, int position)
{
    TREE tree;
    if(!right.empty())
    {
        int size=right.size();
        for (int i=0; i<position; ++i)
        {
            for(TREE::const_iterator iterator = left.cbegin(); iterator != left.cend(); ++iterator) {
                if(iterator->second == i) {
                    tree.insert(TREE::value_type(((*iterator).first),i));
                    break;
                }
            }
        }
        for (int i=position; i<position+size; ++i)
        {
            for(TREE::const_iterator iterator = right.cbegin(); iterator != right.cend(); ++iterator) {
                if(iterator->second == i-position) {
                    tree.insert(TREE::value_type(((*iterator).first),i));
                    break;
                }
            }
        }
        for (int i=position+size; i<size+left.size(); ++i)
        {
            for(TREE::const_iterator iterator = left.cbegin(); iterator != left.cend(); ++iterator) {
                if(iterator->second == i-size) {
                    tree.insert(TREE::value_type(((*iterator).first),i));
                    break;
                }
            }
        }

    }
    return tree;
}

TREE Excl(TREE& left, TREE& right) {

    TREE tree;
    int sizel=left.size(), sizer=right.size(), count=0;
    vector<int> v1, v2;
    for(int i = 0; i < left.size(); i++) {
                for (TREE::const_iterator iterator = left.cbegin(); iterator != left.cend(); ++iterator) {
                    if(iterator->second == i) {
                        v1.push_back(iterator->first);
                        break;
                    }
                }
            }
    for(int j = 0; j < right.size(); j++) {
                for (TREE::const_iterator iteratorr = right.cbegin(); iteratorr != right.cend(); ++iteratorr) {
                    if(iteratorr->second == j) {
                        v2.push_back(iteratorr->first);
                        break;
                    }
                }
            }
            for (int i=0; i<sizel-sizer; i++)
            {
                if(includes(v1.begin()+i, v1.begin()+sizer+i, v2.begin(), v2.end()))
                {
                    v1.erase(v1.begin()+i, v1.begin()+sizer+i);
                    i-=sizer;
                }
            }
            for(int i = 0; i < v1.size(); ++i) tree.insert(TREE::value_type( v1[i],i));
            return tree;


/*
    int current_position = -1,
        position = -1,
        size_left = left.size(),
        size_right = right.size();

    if (!left.empty() && !right.empty()) {
		if (left.size() < right.size())
            return 0;
        else {
            for(int i = 0; i < size_left && (position - current_position + 1) != size_right; ++i) {
                for(TREE::const_iterator iterator_left = left.cbegin(); iterator_left != left.cend(); ++iterator_left){
                    if(iterator_left->first == i) {
                        if(position == -1) {
                            for(TREE::const_iterator iterator_right = right.cbegin(); iterator_right != right.cend(); ++iterator_right) {
                                    if(iterator_right->second == iterator_left->second)
                                        current_position = position = iterator_left->first;
                            }
                        }
                        else
                            for(TREE::const_iterator iterator_right = right.cbegin(); iterator_right != right.cend(); ++iterator_right) {
                                if(iterator_right->second == iterator_left->second) {
                                    if (iterator_left->first > current_position && iterator_left->first - current_position == 1) {
                                        current_position = iterator_left->first;
                                    }
                                }
                                if ((current_position - position + 1) == size_right)
                                    break;
                                if (current_position != iterator_left->first)
                                    position = -1;
                            }
                    }
                }
            }
        }
    }*/
}
/*
vector<int> operator & (vector<int> & v2)
{
    vector<int> v3;
    set_intersection(vector<int>.begin(), vector<int>.end(), v2.begin(), v2.end(), back_inserter(v3));
    for ( int n: v3) cout<<n<<" ";
            cout<<endl;
}*/


int main () {

    try {
            SetConsoleCP(1251);
            SetConsoleOutputCP(1251);
            system("title = STL. Стандартная библиотека шаблонов. Лабораторная № 6.");
            srand(time(0));
            TREE A, B, C, D,E,F,G,H;
            Generate(A, 15);
            //cout << "Дерево: " << endl;
            //OutputTree(A, 'A');
            cout << "\nПоследовательность " ;
            OutputConsist(A, 'A');
            Generate(B,4);
            //cout << "\n\nДерево: " << endl;
            //OutputTree(B, 'B');
            cout << "\nПоследовательность ";
            OutputConsist(B, 'B');
            Generate(C, 1);
            cout << "\nПоследовательность ";
            OutputConsist(C, 'C');
            Generate(D, 8);
            cout << "\nПоследовательность ";
            OutputConsist(D, 'D');
            Generate(E, 5);
            cout << "\nПоследовательность ";
            OutputConsist(E, 'E');
            cout << "\nСлияние последовательностей A и B: " << endl;
            F = Concat(A,B);
            OutputConsist(F, 'F');
            cout << "\nВключение последовательности B в A с 4-го номера: " << endl;
            G = Subst(A,B, 3);
            OutputConsist(G, 'G');
            cout<<endl;
            cout << "Исключение последовательности C из A: " << endl;
            H=Excl(A, C);
            OutputConsist(H, 'H');
            vector<int> v1, v2,v3,v4,v5,v6;

            cout<<endl;
cout<<endl;
            for (TREE::const_iterator iterator = A.cbegin(); iterator != A.cend(); ++iterator) {
                v1.push_back((*iterator).first);
            }
            cout<<"Множество А: ";
            for ( int n: v1) cout<<n<<" ";
            cout<<endl;
            for (TREE::const_iterator iterator = B.cbegin(); iterator != B.cend(); ++iterator) {
                v2.push_back((*iterator).first);
            }
            cout<<"Множество B: ";
            for ( int n: v2) cout<<n<<" ";
            cout<<endl;
            for (TREE::const_iterator iterator = C.cbegin(); iterator != C.cend(); ++iterator) {
                v3.push_back((*iterator).first);
            }
            cout<<"Множество C: ";
            for ( int n: v3) cout<<n<<" ";
            cout<<endl;

            for (TREE::const_iterator iterator = D.cbegin(); iterator != D.cend(); ++iterator) {
                v4.push_back((*iterator).first);
            }
            cout<<"Множество D: ";
            for ( int n: v4) cout<<n<<" ";
            cout<<endl;

            for (TREE::const_iterator iterator = E.cbegin(); iterator != E.cend(); ++iterator) {
                v5.push_back((*iterator).first);
            }
            cout<<"Множество E: ";
            for ( int n: v5) cout<<n<<" ";
            cout<<endl;
            set_union(v1.begin(),v1.end(),v2.begin(),v2.end(), back_inserter(v2));
            sort(v2.begin(), v2.end());
            set_union(v2.begin(),v2.end(),v3.begin(),v3.end(), back_inserter(v3));
            sort(v3.begin(), v3.end());
            set_symmetric_difference(v3.begin(),v3.end(),v4.begin(),v4.end(), back_inserter(v4));
            sort(v4.begin(), v4.end());
            set_intersection(v4.begin(),v4.end(),v5.begin(),v5.end(), back_inserter(v6));
            sort(v6.begin(), v6.end());


            cout<<"Множество H = A|B|C^D&E: ";
            for ( int n: v6) cout<<n<<" ";
            cout<<endl;

    }
    catch(...) {
        cout << "Произошла ошибка." << endl;
    }
}
