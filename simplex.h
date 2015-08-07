#ifndef simplexH
#define simplexH
#include "fraction.h"

class simplex : public fraction
{
 private:

 fraction* obj_function; //������� �������
 fraction** restriction; //������� �����������
 fraction* freed;        //������� ��. ������
 fraction* symb;            //������ ��� �����, ������ ��� �����, �����
 fraction* dis;            //��� ����������� ����������
 fraction answ;           //�������� ������� �������
 int colp;               //���������� �����������
 int s;                   //����� ��������
 bool doub;               //��. �� ��� ���
 bool isk;                 //����� �� ��
 int prev;
 int next;



 
 fraction* new_free; //������ ��������� ������ ��� ��������� ��������
 fraction** new_res; //������� ������������� ��� ��������
 fraction* new_obf;  //������� ������� ��� �������� ��� ������������� ������� ������� ���� fake_fl=TRUE
 int k;              //����� �������� ������������� ����������
 bool dif;            //����, ������ ������ ��� ���, FALSE - �� ������, TRUE - ������
 int* base;           //������ � �����������, ������� � ������
 bool fake_fl;       //���� ��� ��� ��� �-���
 int r;              //����� ��� ��� ��� ��� �-���
 int no_dec;
 int lll;            //���������� ��� �����������
 int rr;

public:
simplex();
void nsimplex(int=2, int=2); //�����������
void del_simplex();    //������������ ������ �� ��� ���. ��������
void init(int=2, int=2); //������������� ������ �������
void de_init(int=2, int=2); //������������ ���. ������ �� ��� ����. �������
int calculation(int=2, int=2); //��������� ��������� ��������

//================================================
fraction* get_obj();     //���������� �������� ������� �������
fraction** get_res();    //���������� ������� �����������
fraction* get_freed();    //���������� ������� ��������� ������

fraction* get_nobj();
fraction** get_nres();
fraction* get_nfreed();    
fraction* get_dis();        //����������  �����������
fraction* get_symb();        //���������� ������� ������ ���������

//===============================================================
fraction get_answ();         //�������� �������� ������� �������
bool get_dif();                 //�������� ���� - ������ ������ ��� ���
bool get_doub();                 //�������� ���� - ��. �� ��� ���
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