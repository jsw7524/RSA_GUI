#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned long long P,Q,E,D,PQ,PQ_Len;

long long ExGCD(long long A,long long B, long long *S, long long *T)
{
    long long I,J;
    if (A%B==0)
    {
        (*S)=0;
        (*T)=1;
        return B;
    }
    else
    {
        I = ExGCD(B,A%B,S,T);
        J=(*S);
        (*S)=(*T);
        (*T)=(*T)*(-A/B)+J;
        return I;
    }
}

void GenerateKeys()
{
    long long I,J;
    P=179424673;  /*944659*/
    Q=373587883;  /*817907*/
    PQ=P*Q;
    PQ_Len=ceil(log(PQ));
    E=65537;
    ExGCD((P-1)*(Q-1),E,&I,&J);
    while (J<=0)
        J+=(P-1)*(Q-1);
    D=J;
}

unsigned long long LongMulMod(unsigned long long A, unsigned long long B, unsigned long long M)
{
    unsigned long long R=0;
    unsigned long long T=B%M;
    while (A>0)
    {
        if (A%2==1)
        {
            R=(R+T)%M;
        }
        T=(T*2)%M;
        A/=2;
    }
    return R;
}

unsigned long long ExpByDouble(unsigned long long V,unsigned long long Exp)
{
    unsigned long long I;
    if (Exp==0)
    {
        return 1;
    }
    else if (Exp==1)
    {
        return V%PQ;
    }
    else
    {
        if (Exp%2==0)
        {
            I=ExpByDouble(V,Exp/2);
            I=LongMulMod(I,I,PQ);
        }
        else
        {
            I=ExpByDouble(V,Exp/2);
            I=LongMulMod(I,I,PQ);
            I=LongMulMod(I,V,PQ);
        }
        return I;
    }
}

unsigned long long MyEncrypt(unsigned long long M)
{
    return ExpByDouble(M,E);
}

unsigned long long MyDecrypt(unsigned long long C)
{
    return ExpByDouble(C,D);
}
/*
int main()
{
    GenerateKeys();
    printf("%llu\n",65llu);
    printf("%016llx\n",(MyEncrypt(65llu)));
    printf("%llu\n",MyDecrypt(MyEncrypt(65llu)));
    return 0;
}
*/
