#include<iostream>
#include<string>
#include <math.h>
#include "sha3.h"
#include <iostream>


int main() {
    string message = "";
    sha3 sha0(224);
    sha3 sha1(256);
    sha3 sha2(384);
    sha3 sha3(512);
    cout << "SHA224: " << sha0.printHash(message) << endl;
    cout << "SHA256: " << sha1.printHash(message) << endl;
    cout << "SHA384: " << sha2.printHash(message) << endl;
    cout << "SHA512: " << sha3.printHash(message) << endl;
}