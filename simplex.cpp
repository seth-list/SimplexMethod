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
void simplex::init(int m, int n)//m - ����� ����������, n - ����� �����������
{
 
 no_dec=0;                   //������ �� ������ 
 s=0;                       //��������
 dif=FALSE;                 //������ ��� �� ������
 k=0;                       //����� �������� ������������� ���������� ��� ����� ������
 fake_fl = FALSE;           //���� ��� �������� ��� ������� �������
 r=0;                       //����� ���������� ��� �������� ��� ��� �-��� (������ 0 ��� n)   
 lll=0;

 for(int i=0; i<m; i++)
 if(dis[i].get_num()==1)     //���� ���� ����� � ���� �� �����������
 lll++;
 
 //���� ���� ���� ���� ����� �� ���� ��� ��� �-���
 for(int i=0; i<n; i++)
 if(symb[i].get_num()==1)
 {fake_fl = TRUE;r=n;break;}
 
 
 
 new_free = new fraction[n+1];    //������ ��������� ������ ��� ��������� ��������
 
 new_res = new fraction*[n];      //������� ������������� ��� �������� 
 for(int i=0;i<n;i++)
 new_res[i] = new fraction[m+n+r+lll+1];

 new_obf = new fraction[m+n+r+lll+1];          //������� ������� ��� ������������� � ������ ������ ��� ��� ������� �-���

 
 
 for(int j=0; j<m+n+r+lll; j++)
 new_obf[j]=0;

 int a=0;
 //���� �� ��� ��� �-���





 if(fake_fl == FALSE)
 for(int j=0; j<m; j++)
 {
  new_obf[j] = men(obj_function[j]); //������ ������ ������� � ����� ������
  a++;
 }



 
 if(fake_fl == FALSE)
 new_free[n] = men(freed[n]);     //������� �������
 //====================




 for(int i=0; i<n; i++)          //���� �� ����� ����������� ��� ���������� � ����� ������
 {
  if(symb[i].get_num()==0)     //���� ������ ������ ��� �����
  {
   new_free[i] = freed[i];       //��������� ���� � ����� ������ �������� ��� ��
   for(int j=0; j<m; j++)
   new_res[i][j] = restriction[i][j];      //������������ � ����� ������ �� ������ ����
   for(int j=m; j<m+n; j++)
   new_res[i][j]=0;              //��������� ������������ ����
   new_res[i][m+k].set_num(1); k++;        //����� ������������ ��� ��������� �������
  }


  if(symb[i].get_num()==2) //���� ������ ������ ��� �����
  {
   new_free[i] = men(freed[i]);  //��������� ���� � ����� ������ � ��������������� ������
   for(int j=0; j<m; j++)
   new_res[i][j] = men(restriction[i][j]);//������������ � ����� ������ ������ ����
   for(int j=m; j<m+n; j++)
   new_res[i][j]=0; //��������� ������������ ����
   new_res[i][m+k].set_num(1); k++; //����� ������������ ��� ��������� �������
  }

  if(symb[i].get_num()==1)     //���� ������ �����
  {
   new_free[i] = freed[i];       //��������� ���� � ����� ������ �������� ��� ��
   for(int j=0; j<m; j++)
   new_res[i][j] = restriction[i][j];      //������������ � ����� ������ �� ������ ����
   for(int j=m; j<m+n; j++)
   new_res[i][j]=0;              //��������� ������������ ���� ���� �����
  }

 }

 int yy;
 //���� ��� ��� ������� �-���
 if(fake_fl == TRUE)
 {
  yy=0;
  //�� ������� ��� ��� ����������
  for(int i=0; i<n; i++)
  for(int j=k+m+lll; j<k+m+r+lll; j++)
  if(i==j-k-m-lll)
  new_res[i][j]=1;
  else
  new_res[i][j]=0;

  //�������� ��� ��� �-���
  for(int j=0; j<n; j++)
  {
   new_free[n]=new_free[n]+men(new_free[j]);
  }
  //��-� ���� ��� ��� �-���
  for(int i=0; i<n; i++)
  for(int j=0; j<m+k+lll; j++)
  {
   new_obf[j]=new_obf[j]+men(new_res[i][j]);
   yy++;
  }
 }

 int l=0;
 int ss=0;
 //��� �����������
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
  base[i] = m+i+1;               //����� �� ������������� ����������
 }
  else
 {
  base = new int[r];
  for(int i=0; i<r; i++)
  base[i] = m+k+lll+i+1;               //����� �� ������������� ����������
 }
 
 //������ ���� ������ ������� ��� �������� ������
 
  int amo_1 = 0, amo_2 = 0;
  fraction zero;
  for(int i=0;i<n;i++)                //���� �� ����� �����������
  if(more_or_eq(new_free[i],zero))    //���� ��� �� �� ������ ����
  amo_1++;                            //���� ��� ������������� �� ������
  
  for(int i=0;i<m+k+r+lll;i++)        //���� �� ����� ����������
  if(more_or_eq(new_obf[i],zero))      //����  ���� ������� ������� ������ ����
  amo_2++;                            //���� ��� ������������� ������������� ������� �������

  
  answ = new_free[n];

  
  if(amo_1==n&&amo_2==m+k+r+lll&&fake_fl==FALSE) //���� ��� ���� � �� ����� >=0 �� ������ ������ � ��� �� ��� �-���
  dif=TRUE;

}

//======================================================= 
int simplex::calculation(int m, int n)//m - ����� ����������, n - ����� �����������
{

  s++;                             //���-�� ��������
  fraction zero;                  //���� ��� ���������
  fraction min_otr;
  int ind_m1;                    //������ ������ �������
  int ind_m2;                    //������ ���� ������  
  doub=FALSE;               //���� ������������� �� �������� ������ 
  isk=FALSE;
  
  if(fake_fl==TRUE)
  {
   for(int j=0; j<m+k+r+lll; j++)
   if(more_or_eq(new_obf[j],zero))
   isk=TRUE;
   else {isk=FALSE;break;}
  }
  //���� ��� �-��� ����� ���� �������� ����� ���� � ��� ���� ��� �-���
  
  if(fake_fl==TRUE&&isk==TRUE) //������� �� ��� � Y ��� S
  {

   if(less(new_free[n],zero))
   no_dec=3;

   if(no_dec==3)
   {dif=TRUE; return s;}

   fake_fl=FALSE;   //��� �-��� ������ �� ���-���
   rr=r;
   r=0;              //���

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



   //������� � �������� ������� �������
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
   //������� � �������� ������� �������
   for(int j=0; j<m+k; j++)
   {
    new_obf[j] = new_obf[j] + men(obj_function[j]);
    ll++;
   }



   //fraction fr;

   //������� � �������� ������� �������
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

   //������� � �������� ������� �������
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
   for(int i=0;i<n;i++)                //���� �� ����� �����������
   if(more_or_eq(new_free[i],zero))    //���� ��� �� �� ������ ����
   amo_1++;                            //���� ��� ������������� �� ������

   for(int i=0;i<m+k+r+lll;i++)        //���� �� ����� ����������
   if(more_or_eq(new_obf[i],zero))      //����  ���� ������� ������� ������ ����
   amo_2++;                            //���� ��� ������������� ������������� ������� �������

  
   answ = new_free[n];

  
   if(amo_1==n&&amo_2==m+k+r+lll&&fake_fl==FALSE) //���� ��� ���� � �� ����� >=0 �� ������ ������ � ��� �� ��� �-���
   dif=TRUE;
   
   return s;
  }

  ind_m2=0;
  int amo_1 = 0, amo_2 = 0;
  for(int i=0;i<n;i++)                //���� �� ����� �����������
  if(more_or_eq(new_free[i],zero))    //���� ��� �� �� ������ ����
  amo_1++;                            //���� ��� ������������� �� ������
  else 
  if(less(new_free[i],min_otr))
  {ind_m2=i; min_otr = new_free[i]; doub=TRUE;}             //����� ������ ������ ��� ������������ �������� ������
  
  //==================================================================
 fraction min_el=new_obf[0];
 ind_m1=0;
 if(doub==FALSE)                //�� �� �������� �����
 {
  for(int i=0;i<m+k+r+lll;i++)        //���� �� ����� �����������
  {
   if(more_or_eq(new_obf[i],zero))      //����  ���� ������� ������� ������ ����
   amo_2++;                            //���� ��� ������������� ������������� ������� �������
   else
   if(less(new_obf[i],min_el))  //���������� ������������ ��� ��������
   {
    min_el = new_obf[i];       //����� �������� ������������� ��������� ���� ������� �������
    ind_m1 = i;                //��������� ��� ������ - ������ �������
   }
  }
 }

  //==================================================================
  if(amo_1==n&&amo_2==m+k+r+lll&&fake_fl==FALSE) //���� ��� ���� � �� ����� >=0 �� ������ ������ � ��� �� ��� �-���
  dif=TRUE;


  if(dif==FALSE) //�����
  {




 //==================================================================
 fraction min_defr(32767, 1);           //����������� ���������
 if(doub==FALSE) ind_m2=0;
 else ind_m1=0;
 if(doub==FALSE)                        //�� �� �������� �����
 {
  no_dec=1;
  for(int j=0;j<n;j++)                  //�������� �� �������
  {

   if(more(new_res[j][ind_m1],zero))                      //��� �������������
   if(less(new_free[j]/new_res[j][ind_m1],min_defr))      //������� ��� ��������� �� ����� � ����-�� ������������ �������
   {
    min_defr = new_free[j]/new_res[j][ind_m1];
    ind_m2 = j; //����������� ������
    no_dec=0;
   }
  }
 }
 else              //������� ���� ������� ��� �� �������� ������
 {
  no_dec=2;
  for(int j=0;j<m+k+r+lll;j++)                  //�������� �� ������
  {
   if(less(new_obf[j],zero)&&less(new_res[ind_m2][j],zero))
   if(less(men(new_obf[j])/men(new_res[ind_m2][j]),min_defr)) //������� ��� ��������� ���� ������� �-��� � ���� ���� ������
    {
     min_defr = men(new_obf[j])/men(new_res[ind_m2][j]);
     ind_m1 = j;                                   //����������� �������
     no_dec=0;
    }

   if(more_or_eq(new_obf[j],zero)&&less(new_res[ind_m2][j],zero))
   if(less(new_obf[j]/men(new_res[ind_m2][j]),min_defr)) //������� ��� ��������� ���� ������� �-��� � ���� ���� ������
   {
     min_defr = new_obf[j]/men(new_res[ind_m2][j]);
     ind_m1 = j;
     no_dec=0;                                   //����������� �������
   }
  }
 }

  if(no_dec==1) {dif=TRUE; return s;}
  if(no_dec==2) {dif=TRUE; return s;}
  //������ ����� ���������� ����������� �������
  //==================================================================
  prev = base[ind_m2];
  base[ind_m2]=ind_m1+1;     //������ ���������� � ������
  next = base[ind_m2];

  fraction razr_el = new_res[ind_m2][ind_m1]; //������ �������
  fraction* razr_col = new fraction [n+1]; //����������� ������� � ��� ������


  for(int j=0;j<n;j++)
  razr_col[j] = men(new_res[j][ind_m1]);
  razr_col[n] = men(new_obf[ind_m1]);

  //��������� ������ � ����� ������� ��� ���� ������
  for(int j=0;j<m+k+r+lll;j++)
  new_res[ind_m2][j] = new_res[ind_m2][j]/razr_el;
  new_free[ind_m2] = new_free[ind_m2]/razr_el;

  //��������� ��������� �����

  int o;
  for(int i=0;i<n;i++)
  {
   if(i!=ind_m2)
   new_free[i] = new_free[i] + razr_col[i]*new_free[ind_m2]; //��. �����
   for(int j=0;j<m+k+r+lll;j++)
   {
    if(i!=ind_m2) //��� ����� ���� ������
    {
     new_res[i][j] = new_res[i][j] + razr_col[i]*new_res[ind_m2][j];//�����������
     o++;
    }

   }
  }


  //��������� ������� �������
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

  for(int i=0;i<n;i++)                //���� �� ����� �����������
  if(more_or_eq(new_free[i],zero))    //���� ��� �� �� ������ ����
  amo_1++;                            //���� ��� ������������� �� ������

  for(int i=0;i<m+k+r+lll;i++)        //���� �� ����� ����������
  if(more_or_eq(new_obf[i],zero))      //����  ���� ������� ������� ������ ����
  amo_2++;                            //���� ��� ������������� ������������� ������� �������

  if(amo_1==n&&amo_2==m+k+r+lll&&fake_fl==FALSE) //���� ��� ���� � �� ����� >=0 �� ������ ������ � ��� �� ��� �-���
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