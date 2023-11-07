#include "rsa.h"

RSA::RSA()
{

}

RSA::RSA(int num_p,int num_q,int E){
    if(primeNum(num_p)&&primeNum(num_q)){
        N = num_p * num_q;
        T = (num_p -1)*(num_q - 1);
        D= 1;
        while(((E*D)%T) != 1){
            D++;
        }
        if(E<T&&(coprime(E,T)==1)){

        }
    }
}

bool RSA::primeNum(int num){
    if(num<=1){
        return false;
    }
    for(int i = 2;i<(int)sqrt(num);i++){
        if(num%i == 0){
            return false;
        }
    }
    return true;
}

int RSA::coprime(int num1,int num2){
    int temp;
    if(num1 < num2){
        temp = num1;
        num1 = num2;
        num2 = temp;
    }
    while(num1 %num2){
        temp = num2;
        num2 = num1%num2;
        num1 = temp;
    }
    return num2;
}

int RSA::candp(int b,int p,int k){       //b(明文),p(E/D),k(N)

    if (p == 1)
    {
        return b%k;
    }
    if (p == 2)
    {
        return b * b % k;
    }
    if (p % 2 == 0)
    {
        int sum = candp(b, p / 2, k);
        return sum * sum % k;
    }
    if (p % 2 == 1)
    {
        int sun = candp(b, p / 2, k);
        return sun * sun * b % k;
    }
}

QString RSA::encrypt(QString text){
    int p = E/D;
    for(int i = 0;i<text.length();i++){
        candp(text[i].unicode(),p,N);
    }

}
