// Лабораторна робота №3, Варіант 4
// Задача 1.4 – Паралелограм
// Задача 2.4 – Вектор (short)
// Задача 3.4 – Матриця (short)
// З інтерактивним введенням даних користувачем

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

// ===================== ЗАДАЧА 1.4 – Паралелограм =====================

class Parallelogram {
    double a, h, b;
    int color;
public:
    Parallelogram() : a(1), h(1), b(1), color(0) {}

    Parallelogram(double a, double h, double b, int color = 0) {
        if (a > 0 && h > 0 && b > 0 && h <= b) {
            this->a = a;
            this->h = h;
            this->b = b;
        }
        else {
            cout << "Error: invalid parameters, set to 1\n";
            this->a = this->h = this->b = 1;
        }
        this->color = (color >= 0) ? color : 0;
    }

    void setA(double v) {
        if (v > 0) a = v;
        else cout << "Error setA: value must be > 0\n";
    }

    void setH(double v) {
        if (v > 0 && v <= b) h = v;
        else cout << "Error setH: value must be > 0 and <= b(" << b << ")\n";
    }

    void setB(double v) {
        if (v >= h) b = v;
        else cout << "Error setB: value must be >= h(" << h << ")\n";
    }

    void setColor(int v) {
        if (v >= 0) color = v;
        else cout << "Error setColor: value must be >= 0\n";
    }

    double getA()     const { return a; }
    double getH()     const { return h; }
    double getB()     const { return b; }
    int    getColor() const { return color; }

    double area()      const { return a * h; }
    double perimeter() const { return 2 * (a + b); }

    void print() const {
        cout << "Parallelogram(a=" << a << " h=" << h << " b=" << b
            << " col=" << color << "): area=" << area()
            << " perimeter=" << perimeter() << "\n";
    }
};

// ===================== ЗАДАЧА 2.4 – Вектор (short) =====================

class VectorShort {
    short* v;
    int num, state;
    static int count;

    void alloc(int n, short val = 0) {
        num = (n > 0) ? n : 1;
        v = new(nothrow) short[num];
        state = v ? 0 : -1;
        if (v) 
            for (int i = 0; i < num; i++) v[i] = val;
        ++count;
    }
public:
    VectorShort() { alloc(1); }
    VectorShort(int n) { alloc(n); }
    VectorShort(int n, short val) { alloc(n, val); }

    VectorShort(const VectorShort& s) {
        alloc(s.num);
        if (v) for (int i = 0; i < num; i++) v[i] = s.v[i];
        state = s.state;
    }

    ~VectorShort() {
        delete[] v;
        --count;
    }

    VectorShort& operator=(const VectorShort& s) {
        if (this == &s) return *this;
        delete[] v;
        alloc(s.num);
        if (v) for (int i = 0; i < num; i++) v[i] = s.v[i];
        return *this;
    }

    void setElem(int i, short val = 0) {
        if (i < 0 || i >= num) { state = -2; cout << "Error: index " << i << " out of bounds [0," << num - 1 << "]\n"; return; }
        v[i] = val;
    }

    short getElem(int i) {
        if (i < 0 || i >= num) { state = -2; cout << "Error: index " << i << " out of bounds [0," << num - 1 << "]\n"; return 0; }
        return v[i];
    }

    int getNum()   const { return num; }
    int getState() const { return state; }
    static int getCount() { return count; }

    VectorShort add(const VectorShort& b) const {
        int n = num < b.num ? num : b.num;
        VectorShort r(n);
        for (int i = 0; i < n; i++) r.v[i] = v[i] + b.v[i];
        return r;
    }

    VectorShort sub(const VectorShort& b) const {
        int n = num < b.num ? num : b.num;
        VectorShort r(n);
        for (int i = 0; i < n; i++) r.v[i] = v[i] - b.v[i];
        return r;
    }

    VectorShort mul(unsigned char s) const {
        VectorShort r(num);
        for (int i = 0; i < num; i++) r.v[i] = (short)(v[i] * s);
        return r;
    }

    bool greaterThan(const VectorShort& b) const {
        if (num != b.num) return false;
        for (int i = 0; i < num; i++) if (v[i] <= b.v[i]) return false;
        return true;
    }

    bool notEqual(const VectorShort& b) const {
        if (num != b.num) return true;
        for (int i = 0; i < num; i++) if (v[i] != b.v[i]) return true;
        return false;
    }

    bool equalTo(const VectorShort& b) const { return !notEqual(b); }

    void inputRandom() {
        for (int i = 0; i < num; i++) v[i] = (short)(rand() % 21 - 10);
    }

    void inputUser() {
        cout << "Enter " << num << " integers (short, from -32768 to 32767):\n";
        for (int i = 0; i < num; i++) {
            cout << "  [" << i << "]: ";
            int t; cin >> t;
            v[i] = (short)t;
        }
    }

    void inputFromFile(ifstream& f) {
        for (int i = 0; i < num && f; i++) {
            int t; f >> t; v[i] = (short)t;
        }
    }

    void print(const char* lbl = "") const {
        if (*lbl) cout << lbl << ": ";
        cout << "[";
        for (int i = 0; i < num; i++) cout << v[i] << (i + 1 < num ? " " : "");
        cout << "] state=" << state << "\n";
    }
};
int VectorShort::count = 0;

// ===================== ЗАДАЧА 3.4 – Матриця (short) =====================

class MatrixShort {
    short* data;
    int rows, cols, state;
    static int count;

    int idx(int i, int j) const { return i * cols + j; }

    void alloc(int r, int c, short val = 0) {
        rows = r > 0 ? r : 1;
        cols = c > 0 ? c : 1;
        data = new(nothrow) short[rows * cols];
        state = data ? 0 : -1;
        if (data) for (int i = 0; i < rows * cols; i++) data[i] = val;
        ++count;
    }
public:
    MatrixShort() { alloc(4, 4); }
    MatrixShort(int n) { alloc(n, n); }
    MatrixShort(int r, int c, short val) { alloc(r, c, val); }

    MatrixShort(const MatrixShort& s) {
        alloc(s.rows, s.cols);
        if (data) for (int i = 0; i < rows * cols; i++) data[i] = s.data[i];
        state = s.state;
    }

    ~MatrixShort() {
        delete[] data;
        --count;
    }

    MatrixShort& operator=(const MatrixShort& s) {
        if (this == &s) return *this;
        delete[] data;
        alloc(s.rows, s.cols);
        if (data) for (int i = 0; i < rows * cols; i++) data[i] = s.data[i];
        return *this;
    }

    void setElem(int i, int j, short val = 0) {
        if (i < 0 || i >= rows || j < 0 || j >= cols) {
            state = -2;
            cout << "Error: index [" << i << "][" << j << "] out of bounds " << rows << "x" << cols << "\n";
            return;
        }
        data[idx(i, j)] = val;
    }

    short getElem(int i, int j) {
        if (i < 0 || i >= rows || j < 0 || j >= cols) {
            state = -2;
            cout << "Error: index [" << i << "][" << j << "] out of bounds " << rows << "x" << cols << "\n";
            return 0;
        }
        return data[idx(i, j)];
    }

    int getRows()  const { return rows; }
    int getCols()  const { return cols; }
    int getState() const { return state; }
    static int getCount() { return count; }

    MatrixShort add(const MatrixShort& b) const {
        if (rows != b.rows || cols != b.cols) { cout << "Size error (add)\n"; return MatrixShort(1, 1, 0); }
        MatrixShort r(rows, cols, 0);
        for (int i = 0; i < rows * cols; i++) r.data[i] = (short)(data[i] + b.data[i]);
        return r;
    }

    MatrixShort sub(const MatrixShort& b) const {
        if (rows != b.rows || cols != b.cols) { cout << "Size error (sub)\n"; return MatrixShort(1, 1, 0); }
        MatrixShort r(rows, cols, 0);
        for (int i = 0; i < rows * cols; i++) r.data[i] = (short)(data[i] - b.data[i]);
        return r;
    }

    MatrixShort mul(const MatrixShort& b) const {
        if (cols != b.rows) { cout << "Size error (mul): cols(A)=" << cols << " != rows(B)=" << b.rows << "\n"; return MatrixShort(1, 1, 0); }
        MatrixShort r(rows, b.cols, 0);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < b.cols; j++) {
                int s = 0;
                for (int k = 0; k < cols; k++) s += data[idx(i, k)] * b.data[b.idx(k, j)];
                r.data[r.idx(i, j)] = (short)s;
            }
        return r;
    }

    MatrixShort mulScalar(short s) const {
        MatrixShort r(rows, cols, 0);
        for (int i = 0; i < rows * cols; i++) r.data[i] = (short)(data[i] * s);
        return r;
    }

    bool greaterThan(const MatrixShort& b) const {
        if (rows != b.rows || cols != b.cols) return false;
        for (int i = 0; i < rows * cols; i++) if (data[i] <= b.data[i]) return false;
        return true;
    }

    bool lessThan(const MatrixShort& b) const {
        if (rows != b.rows || cols != b.cols) return false;
        for (int i = 0; i < rows * cols; i++) if (data[i] >= b.data[i]) return false;
        return true;
    }

    bool notEqual(const MatrixShort& b) const {
        if (rows != b.rows || cols != b.cols) return true;
        for (int i = 0; i < rows * cols; i++) if (data[i] != b.data[i]) return true;
        return false;
    }

    void inputRandom() {
        for (int i = 0; i < rows * cols; i++) data[i] = (short)(rand() % 21 - 10);
    }

    void inputUser() {
        cout << "Enter matrix " << rows << "x" << cols << " (integers):\n";
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++) {
                cout << "  [" << i << "][" << j << "]: ";
                int t; cin >> t;
                data[idx(i, j)] = (short)t;
            }
    }

    void inputFromFile(ifstream& f) {
        for (int i = 0; i < rows * cols && f; i++) {
            int t; f >> t; data[i] = (short)t;
        }
    }

    void print(const char* lbl = "") const {
        if (*lbl) cout << lbl << ":\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) cout << data[idx(i, j)] << "\t";
            cout << "\n";
        }
        cout << "state=" << state << "\n";
    }
};
int MatrixShort::count = 0;

// ===================== МЕНЮ ПАРАЛЕЛОГРАМА =====================

void menuParallelogram() {
    cout << "\n=== Task 1.4: Parallelogram ===\n";
    double a, h, b;
    int col;
    cout << "Enter a (base > 0): ";   cin >> a;
    cout << "Enter h (height > 0): ";   cin >> h;
    cout << "Enter b (side > 0, >= h): "; cin >> b;
    cout << "Enter color (>= 0): ";     cin >> col;

    Parallelogram p(a, h, b, col);
    p.print();

    int choice;
    do {
        cout << "\n--- Operations ---\n";
        cout << "1 - Change a\n";
        cout << "2 - Change h\n";
        cout << "3 - Change b\n";
        cout << "4 - Change color\n";
        cout << "5 - Print\n";
        cout << "0 - Back\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            double v; cout << "New a: "; cin >> v; p.setA(v); p.print();
        }
        else if (choice == 2) {
            double v; cout << "New h: "; cin >> v; p.setH(v); p.print();
        }
        else if (choice == 3) {
            double v; cout << "New b: "; cin >> v; p.setB(v); p.print();
        }
        else if (choice == 4) {
            int v; cout << "New color: "; cin >> v; p.setColor(v); p.print();
        }
        else if (choice == 5) {
            p.print();
        }
    } while (choice != 0);
}

// ===================== МЕНЮ ВЕКТОРА =====================

void menuVector() {
    cout << "\n=== Task 2.4: Vector (short) ===\n";
    srand((unsigned)time(nullptr));

    int nA, nB;
    cout << "Size of vector A: "; cin >> nA;
    cout << "Size of vector B: "; cin >> nB;

    VectorShort A(nA), B(nB);

    int choice;
    do {
        cout << "\n--- Operations ---\n";
        cout << "1  - Input A manually\n";
        cout << "2  - Input B manually\n";
        cout << "3  - Fill A randomly\n";
        cout << "4  - Fill B randomly\n";
        cout << "5  - Print A and B\n";
        cout << "6  - A + B\n";
        cout << "7  - A - B\n";
        cout << "8  - A * scalar\n";
        cout << "9  - Compare A and B\n";
        cout << "10 - Change element of A\n";
        cout << "11 - Get element of A\n";
        cout << "0  - Back\n";
        cout << "Choice: "; cin >> choice;

        if (choice == 1) {
            A.inputUser();
        }
        else if (choice == 2) {
            B.inputUser();
        }
        else if (choice == 3) {
            A.inputRandom(); cout << "A filled randomly\n"; A.print("A");
        }
        else if (choice == 4) {
            B.inputRandom(); cout << "B filled randomly\n"; B.print("B");
        }
        else if (choice == 5) {
            A.print("A"); B.print("B");
        }
        else if (choice == 6) {
            A.add(B).print("A+B");
        }
        else if (choice == 7) {
            A.sub(B).print("A-B");
        }
        else if (choice == 8) {
            int s; cout << "Scalar (0-255): "; cin >> s;
            A.mul((unsigned char)s).print("A*s");
        }
        else if (choice == 9) {
            cout << "A>B:  " << A.greaterThan(B) << "\n";
            cout << "A==B: " << A.equalTo(B) << "\n";
            cout << "A!=B: " << A.notEqual(B) << "\n";
        }
        else if (choice == 10) {
            int i; short val;
            cout << "Index: "; cin >> i;
            cout << "New value: "; cin >> val;
            A.setElem(i, val);
        }
        else if (choice == 11) {
            int i; cout << "Index: "; cin >> i;
            cout << "A[" << i << "] = " << A.getElem(i) << "\n";
        }
    } while (choice != 0);
}

// ===================== МЕНЮ МАТРИЦІ =====================

void menuMatrix() {
    cout << "\n=== Task 3.4: Matrix (short) ===\n";
    srand((unsigned)time(nullptr));

    int rA, cA, rB, cB;
    cout << "Rows of matrix A: ";    cin >> rA;
    cout << "Cols of matrix A: ";  cin >> cA;
    cout << "Rows of matrix B: ";    cin >> rB;
    cout << "Cols of matrix B: ";  cin >> cB;

    MatrixShort A(rA, cA, 0), B(rB, cB, 0);

    int choice;
    do {
        cout << "\n--- Operations ---\n";
        cout << "1  - Input A manually\n";
        cout << "2  - Input B manually\n";
        cout << "3  - Fill A randomly\n";
        cout << "4  - Fill B randomly\n";
        cout << "5  - Print A and B\n";
        cout << "6  - A + B\n";
        cout << "7  - A - B\n";
        cout << "8  - A * B (matrix multiply)\n";
        cout << "9  - A * scalar\n";
        cout << "10 - Compare A and B\n";
        cout << "11 - Change element of A\n";
        cout << "12 - Get element of A\n";
        cout << "0  - Back\n";
        cout << "Choice: "; cin >> choice;

        if (choice == 1) {
            A.inputUser();
        }
        else if (choice == 2) {
            B.inputUser();
        }
        else if (choice == 3) {
            A.inputRandom(); cout << "A filled randomly\n"; A.print("A");
        }
        else if (choice == 4) {
            B.inputRandom(); cout << "B filled randomly\n"; B.print("B");
        }
        else if (choice == 5) {
            A.print("A"); B.print("B");
        }
        else if (choice == 6) {
            A.add(B).print("A+B");
        }
        else if (choice == 7) {
            A.sub(B).print("A-B");
        }
        else if (choice == 8) {
            A.mul(B).print("A*B");
        }
        else if (choice == 9) {
            short s; cout << "Scalar: "; cin >> s;
            A.mulScalar(s).print("A*s");
        }
        else if (choice == 10) {
            cout << "A>B:  " << A.greaterThan(B) << "\n";
            cout << "A<B:  " << A.lessThan(B) << "\n";
            cout << "A!=B: " << A.notEqual(B) << "\n";
        }
        else if (choice == 11) {
            int i, j; short val;
            cout << "Row: "; cin >> i;
            cout << "Col: "; cin >> j;
            cout << "New value: "; cin >> val;
            A.setElem(i, j, val);
        }
        else if (choice == 12) {
            int i, j;
            cout << "Row: "; cin >> i;
            cout << "Col: "; cin >> j;
            cout << "A[" << i << "][" << j << "] = " << A.getElem(i, j) << "\n";
        }
    } while (choice != 0);
}

// ===================== ГОЛОВНЕ МЕНЮ =====================

int main() {
    setlocale(LC_ALL, "");

    int choice;
    do {
        cout << "\n============================\n";
        cout << " Lab work #3, Variant 4\n";
        cout << "============================\n";
        cout << "1 - Parallelogram\n";
        cout << "2 - Vector (short)\n";
        cout << "3 - Matrix (short)\n";
        cout << "0 - Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) menuParallelogram();
        else if (choice == 2) menuVector();
        else if (choice == 3) menuMatrix();
    } while (choice != 0);

    cout << "Goodbye!\n";
    return 0;
}