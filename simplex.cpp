#include <vcl.h>
#pragma hdrstop
#include "simplex.h"
#include "fraction.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
simplex::simplex()
{
 obj_function = 0;
 restriction = 0;
 freed = 0;
 symb = 0;
 colp = 0;
}

void simplex::nsimplex (int m, int n) //x, y
{
 obj_function = new fraction[2*(m+n)];	 
 restriction = new fraction*[n];
 freed = new fraction[n+1];
 for(int i=0;i<n;i++)
 restriction[i] = new fraction[m];
 symb = new fraction[n];
 dis = new fraction[n+m];
 colp = n;
}
	
	
	
//---------------------------------------------------------------------------
void simplex::del_simplex()
{
 delete[]obj_function;
 obj_function=NULL;
 for(int i=0;i<colp;i++)
 {delete[]restriction[i];
  restriction[i]=NULL;}
 delete[]restriction;
 restriction = NULL;
 delete[]symb;
 delete[]dis;
 dis=NULL;
 symb = NULL;
 delete[]freed;
 freed = NULL;

}




//---------------------------------------------------------------------------
void simplex::init(int m, int n)//m - число переменных, n - число ограничений
{
 
 no_dec=0;                   //задача мб решена 
 s=0;                       //итерация
 dif=FALSE;                 //задача еще не решена
 k=0;                       //число вводимых искусственных переменных для формы такера
 fake_fl = FALSE;           //флаг для введения иск целевой функции
 r=0;                       //число переменных для вводимой иск цел ф-ции (всегда 0 или n)   
 lll=0;

 for(int i=0; i<m; i++)
 if(dis[i].get_num()==1)     //если знак равно в пред то расщепление
 lll++;
 
 //если есть хоть одно равно то ввод иск цел ф-ции
 for(int i=0; i<n; i++)
 if(symb[i].get_num()==1)
 {fake_fl = TRUE;r=n;break;}
 
 
 
 new_free = new fraction[n+1];    //массив свободных членов для очередной итерации
 
 new_res = new fraction*[n];      //матрица коэффициентов для итерации 
 for(int i=0;i<n;i++)
 new_res[i] = new fraction[m+n+r+lll+1];

 new_obf = new fraction[m+n+r+lll+1];          //целевая функция для представления в формет такера или иск целевая ф-ция

 
 
 for(int j=0; j<m+n+r+lll; j++)
 new_obf[j]=0;

 int a=0;
 //если не иск цел ф-ция





 if(fake_fl == FALSE)
 for(int j=0; j<m; j++)
 {
  new_obf[j] = men(obj_function[j]); //привод целвой функции к форме такера
  a++;
 }



 
 if(fake_fl == FALSE)
 new_free[n] = men(freed[n]);     //целевая функция
 //====================




 for(int i=0; i<n; i++)          //цикл по числу ограничений для приведения к форме такера
 {
  if(symb[i].get_num()==0)     //если символ меньше или равно
  {
   new_free[i] = freed[i];       //свободный член в форме такера остается тем же
   for(int j=0; j<m; j++)
   new_res[i][j] = restriction[i][j];      //коэффициенты в форме такера не меняют знак
   for(int j=m; j<m+n; j++)
   new_res[i][j]=0;              //остальные коэффициенты нули
   new_res[i][m+k].set_num(1); k++;        //кроме коэффициента для единичной матрицы
  }


  if(symb[i].get_num()==2) //если символ больше или равно
  {
   new_free[i] = men(freed[i]);  //свободный член в форме такера с противоположным знаком
   for(int j=0; j<m; j++)
   new_res[i][j] = men(restriction[i][j]);//коэффициенты в форме такера меняют знак
   for(int j=m; j<m+n; j++)
   new_res[i][j]=0; //остальные коэффициенты нули
   new_res[i][m+k].set_num(1); k++; //кроме коэффициента для единичной матрицы
  }

  if(symb[i].get_num()==1)     //если символ равно
  {
   new_free[i] = freed[i];       //свободный член в форме такера остается тем же
   for(int j=0; j<m; j++)
   new_res[i][j] = restriction[i][j];      //коэффициенты в форме такера не меняют знак
   for(int j=m; j<m+n; j++)
   new_res[i][j]=0;              //остальные коэффициенты нули если равно
  }

 }

 int yy;
 //если это иск целевая ф-ция
 if(fake_fl == TRUE)
 {
  yy=0;
  //ед матрица для иск переменных
  for(int i=0; i<n; i++)
  for(int j=k+m+lll; j<k+m+r+lll; j++)
  if(i==j-k-m-lll)
  new_res[i][j]=1;
  else
  new_res[i][j]=0;

  //значение иск цел ф-ции
  for(int j=0; j<n; j++)
  {
   new_free[n]=new_free[n]+men(new_free[j]);
  }
  //зн-е коэф иск цел ф-ции
  for(int i=0; i<n; i++)
  for(int j=0; j<m+k+lll; j++)
  {
   new_obf[j]=new_obf[j]+men(new_res[i][j]);
   yy++;
  }
 }

 int l=0;
 int ss=0;
 //для расщепления
 if(lll>0)
  for(int j=l; j<m; j++)
  {
   if(dis[j].get_num()==1)
   {
    for(int i=0; i<n; i++)
    {
     new_res[i][ss+k+m]=men(new_res[i][j]);
     new_obf[ss+k+m]=men(new_obf[j]);
     yy++;
    }
    l=j+1;
    ss++;
   }
  }


 if(fake_fl == FALSE)
 {
  base = new int[k];
  for(int i=0; i<k; i++)
  base[i] = m+i+1;               //базис из искусственных переменных
 }
  else
 {
  base = new int[r];
  for(int i=0; i<r; i++)
  base[i] = m+k+lll+i+1;               //базис из искусственных переменных
 }
 
 //теперь есть первая таблица для симплекс метода
 
  int amo_1 = 0, amo_2 = 0;
  fraction zero;
  for(int i=0;i<n;i++)                //цикл по числу ограничений
  if(more_or_eq(new_free[i],zero))    //если все св чл больше нуля
  amo_1++;                            //флаг для положительных св членов
  
  for(int i=0;i<m+k+r+lll;i++)        //цикл по числу переменных
  if(more_or_eq(new_obf[i],zero))      //если  коэф целевой функции больше нуля
  amo_2++;                            //флаг для положительных коэффициентов целевой функции

  
  answ = new_free[n];

  
  if(amo_1==n&&amo_2==m+k+r+lll&&fake_fl==FALSE) //если все коэф и св члены >=0 то задача решена и это не иск ф-ция
  dif=TRUE;

}

//======================================================= 
int simplex::calculation(int m, int n)//m - число переменных, n - число ограничений
{

  s++;                             //кол-во итераций
  fraction zero;                  //ноль для сравнения
  fraction min_otr;
  int ind_m1;                    //индекс разреш столбца
  int ind_m2;                    //индекс разр строки  
  doub=FALSE;               //флаг использования дв симплекс метода 
  isk=FALSE;
  
  if(fake_fl==TRUE)
  {
   for(int j=0; j<m+k+r+lll; j++)
   if(more_or_eq(new_obf[j],zero))
   isk=TRUE;
   else {isk=FALSE;break;}
  }
  //если цел ф-ция после всех итераций равна нулю и эта была цел ф-ция
  
  if(fake_fl==TRUE&&isk==TRUE) //переход из иск в Y или S
  {

   if(less(new_free[n],zero))
   no_dec=3;

   if(no_dec==3)
   {dif=TRUE; return s;}

   fake_fl=FALSE;   //цел ф-ция теперь не иск-ная
   rr=r;
   r=0;              //нет

   fraction fr;
   int ll=0;
   new_free[n]= men(freed[n]);
   for(int i=0; i<n; i++)
   {
    fr=obj_function[base[i]-1];
    if(base[i]<m+k)
    new_free[n]=new_free[n]+ new_free[i]*obj_function[base[i]-1];
    else
    if(lll!=0)
    new_free[n]=new_free[n]+ new_free[i]*men(obj_function[base[i]-m-k-1]);
    ll++;
   }



   //переход в исходную целевую функцию
   for(int i=0; i<n; i++)
   for(int j=0; j<m+k; j++)
   {
    if(base[i]<m+k)
    new_obf[j] = new_obf[j] + new_res[i][j]*obj_function[base[i]-1];
    else
    if(lll!=0)
    new_obf[j] = new_obf[j] + new_res[i][j]*men(obj_function[base[i]-m-k-1]);
    ll++;
   }
   //переход в исходную целевую функцию
   for(int j=0; j<m+k; j++)
   {
    new_obf[j] = new_obf[j] + men(obj_function[j]);
    ll++;
   }



   //fraction fr;

   //переход в исходную целевую функцию
   for(int j=m+k; j<m+k+lll; j++)
   for(int i=0; i<n; i++)
   {
    fr = obj_function[base[i]-1];
    if(base[i]<m+k)
    new_obf[j] = new_obf[j] + new_res[i][j]*obj_function[base[i]-1];
    else
    if(lll!=0)
    new_obf[j] = new_obf[j] + new_res[i][j]*men(obj_function[base[i]-m-k-1]);
    ll++;
   }

   //переход в исходную целевую функцию
   int l=0;
   int nm=0;
   for(int i=l; i<m; i++)
   if(dis[i].get_num()==1)
   {
    new_obf[m+k+nm] = new_obf[m+k+nm] + obj_function[i];
    ll++;
    l=i+1;
    nm++;
   }

   int amo_1 = 0, amo_2 = 0;
   fraction zero;
   for(int i=0;i<n;i++)                //цикл по числу ограничений
   if(more_or_eq(new_free[i],zero))    //если все св чл больше нуля
   amo_1++;                            //флаг для положительных св членов

   for(int i=0;i<m+k+r+lll;i++)        //цикл по числу переменных
   if(more_or_eq(new_obf[i],zero))      //если  коэф целевой функции больше нуля
   amo_2++;                            //флаг для положительных коэффициентов целевой функции

  
   answ = new_free[n];

  
   if(amo_1==n&&amo_2==m+k+r+lll&&fake_fl==FALSE) //если все коэф и св члены >=0 то задача решена и это не иск ф-ция
   dif=TRUE;
   
   return s;
  }

  ind_m2=0;
  int amo_1 = 0, amo_2 = 0;
  for(int i=0;i<n;i++)                //цикл по числу ограничений
  if(more_or_eq(new_free[i],zero))    //если все св чл больше нуля
  amo_1++;                            //флаг для положительных св членов
  else 
  if(less(new_free[i],min_otr))
  {ind_m2=i; min_otr = new_free[i]; doub=TRUE;}             //нашли разреш строку при двойственном симплекс методе
  
  //==================================================================
 fraction min_el=new_obf[0];
 ind_m1=0;
 if(doub==FALSE)                //Не дв симплекс метод
 {
  for(int i=0;i<m+k+r+lll;i++)        //цикл по числу ограничений
  {
   if(more_or_eq(new_obf[i],zero))      //если  коэф целевой функции больше нуля
   amo_2++;                            //флаг для положительных коэффициентов целевой функции
   else
   if(less(new_obf[i],min_el))  //нахождение минимального отр элемента
   {
    min_el = new_obf[i];       //миним элементу присваивается очередной коэф целевой функции
    ind_m1 = i;                //находится его индекс - разреш столбец
   }
  }
 }

  //==================================================================
  if(amo_1==n&&amo_2==m+k+r+lll&&fake_fl==FALSE) //если все коэф и св члены >=0 то задача решена и это не иск ф-ция
  dif=TRUE;


  if(dif==FALSE) //иначе
  {




 //==================================================================
 fraction min_defr(32767, 1);           //минимальное отношение
 if(doub==FALSE) ind_m2=0;
 else ind_m1=0;
 if(doub==FALSE)                        //Не дв симплекс метод
 {
  no_dec=1;
  for(int j=0;j<n;j++)                  //проходим по столбцу
  {

   if(more(new_res[j][ind_m1],zero))                      //все положительные
   if(less(new_free[j]/new_res[j][ind_m1],min_defr))      //находим мин отношение св члена к коэф-ту разрешающего столбца
   {
    min_defr = new_free[j]/new_res[j][ind_m1];
    ind_m2 = j; //разрешающая строка
    no_dec=0;
   }
  }
 }
 else              //находим разр столбец при дв симплекс методе
 {
  no_dec=2;
  for(int j=0;j<m+k+r+lll;j++)                  //проходим по строке
  {
   if(less(new_obf[j],zero)&&less(new_res[ind_m2][j],zero))
   if(less(men(new_obf[j])/men(new_res[ind_m2][j]),min_defr)) //находим мин отношение коэф целевой ф-ции к коэф разр строки
    {
     min_defr = men(new_obf[j])/men(new_res[ind_m2][j]);
     ind_m1 = j;                                   //разрешающий столбец
     no_dec=0;
    }

   if(more_or_eq(new_obf[j],zero)&&less(new_res[ind_m2][j],zero))
   if(less(new_obf[j]/men(new_res[ind_m2][j]),min_defr)) //находим мин отношение коэф целевой ф-ции к коэф разр строки
   {
     min_defr = new_obf[j]/men(new_res[ind_m2][j]);
     ind_m1 = j;
     no_dec=0;                                   //разрешающий столбец
   }
  }
 }

  if(no_dec==1) {dif=TRUE; return s;}
  if(no_dec==2) {dif=TRUE; return s;}
  //теперь можно определить разрешающий элемент
  //==================================================================
  prev = base[ind_m2];
  base[ind_m2]=ind_m1+1;     //замена переменной в базисе
  next = base[ind_m2];

  fraction razr_el = new_res[ind_m2][ind_m1]; //разреш элемент
  fraction* razr_col = new fraction [n+1]; //разрешающий столбец с отр знаком


  for(int j=0;j<n;j++)
  razr_col[j] = men(new_res[j][ind_m1]);
  razr_col[n] = men(new_obf[ind_m1]);

  //получение строки в новой таблице для разр строки
  for(int j=0;j<m+k+r+lll;j++)
  new_res[ind_m2][j] = new_res[ind_m2][j]/razr_el;
  new_free[ind_m2] = new_free[ind_m2]/razr_el;

  //получение остальных строк

  int o;
  for(int i=0;i<n;i++)
  {
   if(i!=ind_m2)
   new_free[i] = new_free[i] + razr_col[i]*new_free[ind_m2]; //св. члены
   for(int j=0;j<m+k+r+lll;j++)
   {
    if(i!=ind_m2) //все кроме разр строки
    {
     new_res[i][j] = new_res[i][j] + razr_col[i]*new_res[ind_m2][j];//ограничения
     o++;
    }

   }
  }


  //получение целевой функции
  new_free[n] = new_free[n] + razr_col[n]*new_free[ind_m2];

  for(int j=0;j<m+k+r+lll;j++)
  {
   new_obf[j] = new_obf[j] + razr_col[n]*new_res[ind_m2][j];
   o++;
  }
   delete[]razr_col;

 } //if(dif)


  answ = new_free[n];


  amo_1 = 0, amo_2 = 0;

  for(int i=0;i<n;i++)                //цикл по числу ограничений
  if(more_or_eq(new_free[i],zero))    //если все св чл больше нуля
  amo_1++;                            //флаг для положительных св членов

  for(int i=0;i<m+k+r+lll;i++)        //цикл по числу переменных
  if(more_or_eq(new_obf[i],zero))      //если  коэф целевой функции больше нуля
  amo_2++;                            //флаг для положительных коэффициентов целевой функции

  if(amo_1==n&&amo_2==m+k+r+lll&&fake_fl==FALSE) //если все коэф и св члены >=0 то задача решена и это не иск ф-ция
  dif=TRUE;
  return s;
}





//=======================================================
void simplex::de_init(int m, int n)
{ 
 delete[]new_obf;	 
 for(int i=0;i<n;i++)
 delete[]new_res[i];
 delete[]new_res;	
 delete[]new_free;
 delete[]base;


}

//=======================================================
fraction simplex::get_answ()
{ return answ;}

//=======================================================
bool simplex::get_dif()
{ return dif;}
//=======================================================
bool simplex::get_doub()
{ return doub;}
//=======================================================
bool simplex::get_isk()
{ return isk;}
//=======================================================
fraction* simplex::get_obj()
{return obj_function;}

//=======================================================
fraction**  simplex::get_res()
{return restriction;}
//=======================================================
fraction* simplex::get_freed()
{return freed;}

//=======================================================
fraction* simplex::get_nobj()
{return new_obf;}

//=======================================================
fraction**  simplex::get_nres()
{return new_res;}
//=======================================================
fraction* simplex::get_nfreed()
{return new_free;}
//=======================================================
fraction* simplex::get_symb()
{return symb;}
//=======================================================
fraction* simplex::get_dis()
{return dis;}
//=======================================================
int* simplex::get_base()
{return base;}
//======================================================
int simplex::get_k()
{return k;}
//=====================================================
int simplex::get_rr()
{return rr;}
//=====================================================
int simplex::get_r()
{return r;}
//=====================================================
int simplex::get_l()
{return lll;}
//=====================================================
int simplex::get_prev()
{return prev;}
//=====================================================
int simplex::get_next()
{return next;}
//=====================================================
int simplex::get_not()
{return  no_dec;}