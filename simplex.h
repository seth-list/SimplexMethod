#ifndef simplexH
#define simplexH
#include "fraction.h"

class simplex : public fraction
{
 private:

 fraction* obj_function; //целевая функция
 fraction** restriction; //матрица ограничений
 fraction* freed;        //столбец св. членов
 fraction* symb;            //меньше или равно, больше или равно, равно
 fraction* dis;            //для расщепления переменной
 fraction answ;           //значение целевой функции
 int colp;               //количество ограничений
 int s;                   //число итераций
 bool doub;               //дв. см или нет
 bool isk;                 //оконч дв см
 int prev;
 int next;



 
 fraction* new_free; //массив свободных членов для очередной итерации
 fraction** new_res; //матрица коэффициентов для итерации
 fraction* new_obf;  //целевая функция для итерации или искусственная целевая функция если fake_fl=TRUE
 int k;              //число вводимых искусственных переменных
 bool dif;            //флаг, решена задача или нет, FALSE - не решена, TRUE - решена
 int* base;           //массив с переменными, которые в базисе
 bool fake_fl;       //флаг для иск цел ф-ции
 int r;              //число огр для иск цел ф-ции
 int no_dec;
 int lll;            //переменные для расщепления
 int rr;

public:
simplex();
void nsimplex(int=2, int=2); //конструктор
void del_simplex();    //освобождение памяти из под дин. массивов
void init(int=2, int=2); //инициализация первой таблицы
void de_init(int=2, int=2); //освобождение дин. памяти из под иниц. таблицы
int calculation(int=2, int=2); //получение очередной итерации

//================================================
fraction* get_obj();     //установить исходную целевую функцию
fraction** get_res();    //установить матрицу ограничений
fraction* get_freed();    //установить столбец свободных членов

fraction* get_nobj();
fraction** get_nres();
fraction* get_nfreed();    
fraction* get_dis();        //установить  расщепление
fraction* get_symb();        //установить столбец знаков сравнения

//===============================================================
fraction get_answ();         //получить значение целевой функции
bool get_dif();                 //получить флаг - решена задача или нет
bool get_doub();                 //получить флаг - дв. см или нет
bool get_isk();
int* get_base();
int get_k();
int get_prev();
int get_next();
int get_not();
int get_r();
int get_rr();
int get_l();
};


//-------------------------------------------------------------------------------
#endif 