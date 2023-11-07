#ifndef RSA_H
#define RSA_H

#include<math.h>
#include<QString>
class RSA
{
public:
    bool primeNum(int num);
    int coprime(int num1,int num2);
    int candp(int b,int p,int k);
    QString encrypt(QString text);
public:
    RSA();
    RSA(int num_p,int num_q,int E);
    int N,T,D,E;
};

#endif // RSA_H
