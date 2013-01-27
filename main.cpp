#include <iostream>
#include <fstream>
using namespace std;

int **nextAge(int **current, int n);
void printAge(ostream &os, int **age, int n);
void deleteField(int **f, int n);

int main(int argc, char **argv)
{
    ifstream in("input_state");
    ofstream out("output_ages");
    int n, a;
    int **age;

    in >> n >> a; // размер поля и число поколений

    // квадратное поле
    int **field = new int*[n];
    for(int i=0; i<n; ++i) {
        field[i] = new int[n];
        for(int j=0; j<n; ++j) {
            in >> field[i][j];
        }
    }

    for(int i=0; i<a; ++i) {
        age = nextAge(field, n);
        printAge(out, age, n);
        deleteField(field, n);
        field = age;
        out << endl;
    }

    deleteField(field, n);

    return 0;
}

void deleteField(int **f, int n)
{
    for(int i=0; i<n; ++i) {
        delete [] f[i];
    }
}

bool isValid(int y, int x, int n)
{
    if(y < 0 || x < 0 || y == n || x == n) {
        return false;
    }

    return true;
}

int neighborhood(int **field, int n, int y, int x)
{
    int count = 0;
    int i, j;

    for(i = y - 1; i <= (y+1); ++i) {
        for(j = x - 1; j <= (x+1); ++j) {
            if(isValid(i, j, n)) {
                if(field[i][j]) {
                    ++count;
                }
            }
        }
    }

    // избавляемся от дублирования
    if(field[y][x]) {
        --count;
    }

    return count;
}

int **nextAge(int **current, int n)
{
    int neighbors;
    int **age = new int*[n];
/* 
    cout << "==========log=============\n";
    printAge(cout, current, n);
    cout << "==========log=============\n\n";
*/
    for(int i=0; i<n; ++i) {
        age[i] = new int[n];
        for(int j=0; j<n; ++j) {
            neighbors = neighborhood(current, n, i, j);

//            cout << "neighbors for [" << i << "][" << j << "] = " << neighbors << endl;

            if(current[i][j]) { // живая клетка
                if(neighbors == 2 || neighbors == 3) {
                    age[i][j] = 1;
                } else {
                    age[i][j] = 0;
                }
            } else {    // метрвая
                if(neighbors == 3) {
                    age[i][j] = 1;
                } else {
                    age[i][j] = 0;
                }
            }
        }
    }

    return age;
}

void printAge(ostream &os, int **age, int n)
{
    for(int i=0; i<n; ++i) {
        for(int j=0; j<n; ++j) {
            os << age[i][j] << ' ';
        }
        os << '\n';
    }
}

