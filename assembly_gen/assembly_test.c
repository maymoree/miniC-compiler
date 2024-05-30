extern void print(int);
extern int read();

int func(int p){

    int a;
    while (a < p) {
        int b;
        b = read();
        a = a + b;
    }

    return a;

}