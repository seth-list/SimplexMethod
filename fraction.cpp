#include <vcl.h>
#pragma hdrstop
#include "fraction.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
bool fraction::more(fraction const & a, fraction const & b)
{
 if(a.num * b.den > a.den * b.num)  return true;
 else return false;
}

//---------------------------------------------------------------------------
bool fraction::less(fraction const & a, fraction const & b)
{
 if(a.num * b.den < a.den * b.num)  return true;
 else return false;
}

//---------------------------------------------------------------------------
bool fraction::more_or_eq(fraction const & a, fraction const & b)
{
 if(a.num * b.den >= a.den * b.num)  return true;
 else return false;
}

//---------------------------------------------------------------------------
bool fraction::less_or_eq(fraction const & a, fraction const & b)
{
 if(a.num * b.den <= a.den * b.num)  return true;
 else return false;
}

//---------------------------------------------------------------------------
fraction fraction::men(fraction const & a)
{
  num=-a.num;
  den=a.den;
  return *this;
 

}
//---------------------------------------------------------------------------
fraction::fraction (int n, int d)
{num=n;den=d;}

//---------------------------------------------------------------------------
int fraction::get_num()
{
 return num;
}

//---------------------------------------------------------------------------
int fraction::get_den()
{ return den;}	

void fraction::set_den(int a)
{den = a;}

void fraction::set_num(int a)
{
 num = a;
}
//---------------------------------------------------------------------------
fraction fraction::operator+(fraction const & a)//сложение дробей
{
  int nd;
  fraction Sum;
  
  Sum.num=a.num*den+num*a.den;
  Sum.den=a.den*den;
  

  nd = nod(abs(Sum.den),abs(Sum.num));

  Sum.num=Sum.num/nd;
  Sum.den=Sum.den/nd;
  if(Sum.den<0)
  {
   Sum.num=Sum.num*(-1);
   Sum.den=Sum.den*(-1);
  } 
  
  if(Sum.num==0)
  Sum.den=1;
  return Sum;


}



//---------------------------------------------------------------------------
fraction fraction::operator-(fraction const & a)//вычитание дробей
{

  int nd;
  fraction Sum;
  
  Sum.num=a.num*den-num*a.den;
  Sum.den=a.den*den;
  
  if (Sum.num>Sum.den)
  nd = nod(Sum.den, Sum.num);
  else nd = nod(Sum.num, Sum.den);
  Sum.num=Sum.num/nd;
  Sum.den=Sum.den/nd;
  if(Sum.den<0)
  {
   Sum.num=Sum.num*(-1);
   Sum.den=Sum.den*(-1);
  } 
  
  if(Sum.num==0)
  Sum.den=1;
  return Sum;
}



//---------------------------------------------------------------------------
fraction fraction::operator*(fraction const & a)//умножение дробей
{

  int nd;
  fraction Sum;
  
  Sum.num=a.num*num;
  Sum.den=a.den*den;
  

  nd = nod(abs(Sum.den),abs(Sum.num));

  Sum.num=Sum.num/nd;
  Sum.den=Sum.den/nd;
  if(Sum.den<0)
  {
   Sum.num=Sum.num*(-1);
   Sum.den=Sum.den*(-1);
  }

  if(Sum.num==0)
  Sum.den=1;  
  return Sum;

}

//---------------------------------------------------------------------------
fraction fraction::operator/(fraction const & a)//деление дробей
{

  int nd;
  fraction Sum;
  
  Sum.num=num*a.den;
  Sum.den=den*a.num;
  if  (a.num==0)
  {exit (1);};
  
  nd = nod(abs(Sum.den),abs(Sum.num));
	
  Sum.num=Sum.num/nd;
  Sum.den=Sum.den/nd;
  if(Sum.den<0)
  {
   Sum.num=Sum.num*(-1);
   Sum.den=Sum.den*(-1);
  } 
  
  if(Sum.num==0)
  Sum.den=1;
  return Sum;

}
//---------------------------------------------------------------------------
fraction & fraction::operator = (fraction const & a)
{
  num=a.num;
  den=a.den;
   
  return *this;


}


//---------------------------------------------------------------------------
int fraction::nod (int n, int d) const //нод
{
        if(n==0||d==0) return 1;
        int k1=abs(n);
        int k2=abs(d);
		
        while(k1!=k2)
        if(k1>k2)k1-=k2;
        else k2-=k1;
        return k1;
}

