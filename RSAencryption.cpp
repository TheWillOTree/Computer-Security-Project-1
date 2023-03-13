//This program computes the trigraph code for a message using the RSA algorithm.
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstddef>
#include <math.h>
#include <sstream>
#include <time.h>
#include "BigIntegerLibrary.hh"


using namespace std;

//Global variables


//function prototypes

BigUnsigned getPrime();
vector<char> storeMessage(vector<char>); //Reads plaintext file and puts all characters into vector
vector<BigUnsigned> getTrigraphCode(char[], vector<BigUnsigned>); //reads every 3 chars and converts to trigraph code
vector<BigUnsigned> encipher(vector<BigUnsigned>, BigUnsigned, BigUnsigned);
vector<char> quadraGraph(vector<BigUnsigned>);
vector<BigUnsigned> decipher(vector<BigUnsigned>, BigUnsigned, BigUnsigned);
BigUnsigned modExp(BigUnsigned, BigUnsigned, BigUnsigned);

//Main program method
int main(){
    vector<char> plaintext;
    vector<char> ciphertext;
    vector<BigUnsigned>    trigraph_code;
    vector<BigUnsigned>    enciphered_code;
    vector<BigUnsigned>    deciphered_code;
    
    //strings will be replaced with value gotten from getPrime function
    BigUnsigned p = stringToBigUnsigned("601226901190101306339707032778070279008174732520529886901066488712245510429339761526706943586500787976175363847");
    BigUnsigned q = stringToBigUnsigned("53631231719770388398296099992823384509917463282369573510894245774887056120294187907207497192667613710760127432745944203415015531247786279785734596024343867");
    BigUnsigned n = p * q; //modulus
    BigUnsigned totient = (p - 1)* (q - 1);
    BigUnsigned e = stringToBigUnsigned("46517678354918840995156723704832290198633047083988355858015372747560914439257467092876227245680868195888801382801035387746214504231362051");
    BigUnsigned d;
    d = modinv(e, totient);

    plaintext = storeMessage(plaintext);
    //Iterate through the plaintext vector and calculate the trigraph codes
    int size = plaintext.size();
    int numPadding = 0;
    if(size % 3 != 0){
        numPadding = 3 - (size % 3);
        int i = 0;
        while(i < numPadding){
            plaintext.push_back('\0');
            i++;
        }
    }

    //Ready for trigraph code
    int A,B,C; //Place holders in vector
    A = 0, B = 1,C = 2; 
    int num = plaintext.size()/3;
    for(int i = 0; i < num; i++){
        char trigraph[] = {plaintext[A], plaintext[B], plaintext[C]};
        trigraph_code = getTrigraphCode(trigraph, trigraph_code);
        A = A + 3;
        B = B + 3;
        C = C + 3;
    }
    
    enciphered_code = encipher(trigraph_code, e, n);
    ciphertext = quadraGraph(enciphered_code);// will need to create file shortly after
    //create file here with ciphertext
    // 
    // 
    deciphered_code = decipher(enciphered_code, d, n);
    
    
    return 0;
}
//function to get random prime numbers. 
BigUnsigned getPrime() {
    return 0;
}


//Function to store the input file into a vector of strings
vector<char> storeMessage(vector<char> trigraph){

    ifstream inputFile("message.txt");

    char c;
    while(inputFile.get(c)){
        trigraph.push_back(c);
    }
  
    inputFile.close();
    return trigraph;
}

//Function to convert plaintext trigraphs into trigraph code
vector <BigUnsigned> getTrigraphCode(char plaintext[3], vector<BigUnsigned> tricode){
    BigUnsigned num1, num2, num3, sum;
    if(plaintext[1] == '\0'){ //sets termination char to A which is 0
        plaintext[1] = 'A';
        plaintext[2] = 'A';
    }
    if(plaintext[2] == '\0'){
        plaintext[2] = 'A';
    }

    num1 = int(plaintext[0] - int('A'));
    num2 = int(plaintext[1] - int('A'));
    num3 = int(plaintext[2] - int('A'));
    sum = (num1 * 26 * 26) + (num2 * 26) + num3;

    tricode.push_back(sum);
    return tricode;
}

BigUnsigned modExp(BigUnsigned x, BigUnsigned y, BigUnsigned N){
    if(y == 0){
        return 1;
    }

    BigUnsigned floor;
    BigUnsigned parity = y % 2;
    y.divideWithRemainder(2, floor);
    BigUnsigned z = modExp(x, floor, N);

    if(parity == 0){
        return ((z * z) % N);
    }
    else{
        return ((x * z * z) % N);
    }
}

//Function to encipher the trigraph code
vector<BigUnsigned> encipher(vector<BigUnsigned> trigraph, BigUnsigned e, BigUnsigned n){
    vector<BigUnsigned> enciphered_code;

    for(int i = 0; i < trigraph.size(); i++){
        BigUnsigned val = modExp(trigraph[i], e, n);
        enciphered_code.push_back(val);
    }
    return enciphered_code;
}

//function to create quadragraph
vector<char> quadraGraph(vector<BigUnsigned> ciphercode) {
    vector<char> ciphertext;
    BigUnsigned three, two, one;
    three = 26 * 26 * 26;
    two = 26 * 26;
    one = 26;

    for (int i = 0; i < ciphercode.size(); i++) {
        BigUnsigned quotient = 0;
        BigUnsigned remainder = ciphercode[i];
        //cout << hex << ciphercode[i] << "\n";
        

        remainder.divideWithRemainder(three, quotient);
        cout << bigUnsignedToString(quotient + int('A')) << "\n";
        remainder.divideWithRemainder(two, quotient);
        cout << bigUnsignedToString(quotient + int('A')) << "\n";
        remainder.divideWithRemainder(one, quotient);
        cout << bigUnsignedToString(quotient + int('A')) << "\n";
        cout << bigUnsignedToString(remainder + int('A')) << "\n\n";
    }
    return ciphertext;
}


//Function to decipher the enciphered code 
vector<BigUnsigned> decipher(vector<BigUnsigned> code, BigUnsigned d, BigUnsigned n){
    vector<BigUnsigned> deciphered_code;
    
    for(int i = 0; i < code.size(); i++){
        BigUnsigned val = modExp(code[i], d, n);
        deciphered_code.push_back(val);
    }
    
    return deciphered_code;
}