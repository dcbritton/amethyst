// // example.c
// // used as an example for proper LLVM output in example.ll

// struct Type {
//     int a;
//     float b;
// };

// // int add(struct Type T) {
// //     return T.firstMember + T.secondMember;
// // }

// struct Type GLOBAL = {123, 456};

// int main() {
//     // struct Type T = {1, 2};
//     // add(T);

//     // char* example = "This is how to handle string literals.\n";

//     if (GLOBAL.b > 100) {
//         int a = GLOBAL.a;
//         GLOBAL.b = 50.123;
//     }
//     else if (GLOBAL.b < 50) {
//         int c = GLOBAL.a + 1;
//     }
//     else {
//         int b = 1 + 2 + 3;
//     }

//     return 0;
// }

struct Type {
    float member;
};

int EXAMPLE = 1;
int EXAMPLE2 = 2;    

int* add(struct Type* a, int* b) {
    return &EXAMPLE;
}

int main() {
    int a = 123;
    EXAMPLE = 1;
    return 0;
}