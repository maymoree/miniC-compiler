extern void print(int);
extern int read();

int func(int p){
    int a;
    int b;
    b = p + a;
    if (a < b) {
        a = 1;
    } else {
        while (a < p) {
            a = a - 1;
        }
    }
    p = p - 1;
    return a;

}