//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit.h"
#include "Unit2.h"
#include "Unit3.h"
#include "fraction.h"
#include "simplex.h"
#include "ShellAPI.h"
#include <stdio.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma link "Word_2K_SRVR"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------


int m=0, n=0;//размеры матрицы ограничений
BOOL Doubl;  //дв задача
BOOL Line;  //пр задача
simplex s;   //элемент класса
simplex li;   //пошаг режим пр задача
simplex du;   //пошаг режим дв задача
int minmax;   //минимум или максимум
fraction* zz;  //массив для указания расщ переменных


void __fastcall TForm1::Button1Click(TObject *Sender)
{
   //очистка таблиц
   for(int i=0;i<StringGrid2->ColCount;i++)
   for(int j=0;j<StringGrid2->ColCount;j++)
   {
    StringGrid1->Cells[i][j]="";
    StringGrid2->Cells[i][j]="";
   }
   //очистка ответов
   Panel3->Caption="";
   Panel4->Caption="";

    //убираем кнопки
    Button4->Visible=false;
    Button5->Visible=false;
    Button6->Visible=false;
    Button7->Visible=false;
    N1->Enabled=false;
    N2->Enabled=false;

    //установка числа переменных и ограничений
    n = CSpinEdit1->Value+1;
    m = CSpinEdit2->Value+1;
    
    if(n==11)
    {N8->Enabled=false;SpeedButton3->Enabled=false;}
     else
    {N8->Enabled=true;SpeedButton3->Enabled=true;}


    if(m==11)
    {N9->Enabled=false;SpeedButton4->Enabled=false;}
    else
    {N9->Enabled=true;SpeedButton4->Enabled=true;}

    LockWindowUpdate(Form1->Handle);
    
    if(zz!=NULL) delete[]zz;
    zz = new fraction[m-1];

    //удаление старых элементов
    for (int i = 0; i < m1; i++)
    for (int j = 0; j < n1; j++)
    delete Mass_edit[i][j];

    for (int i = 0; i < m1; i++)
    for (int j = 0; j < n1-1; j++)
    delete Mass_label[i][j];

    for (int j = 0; j < m1; j++)
    delete Mass_cb[j];

    for (int j = 0; j < n1-1; j++)
    delete Mass_ch[j];

    int sm=0;
    //создание новых
    for (int i = 0; i < m; i++)
    {
     for (int j = 0; j < n; j++)
     {

        Mass_edit[i][j]  = new TEdit(this);
        Mass_edit[i][j] = &TEdit(Form1->GroupBox1);
        Mass_edit[i][j]->Height=20;
        Mass_edit[i][j]->Width=40;
        Mass_edit[i][j]->Left = 20+70*j;
        if(j==n-1)
        Mass_edit[i][j]->Left += 70;
        else sm = Mass_edit[i][j]->Left;
        if(i==0&&j==n-1)
        Mass_edit[i][j]->Left -= 70;
        Mass_edit[i][j]->Top = 45+30*i;
        if(i>=1) Mass_edit[i][j]->Top += 20;
        Mass_edit[i][j]->Visible = true;
        Mass_edit[i][j]->Parent = Form1->GroupBox1;
        Mass_edit[i][j]->Text="0";

     }


        for (int j = 0; j < n-1; j++)
        {
         Mass_label[i][j]  = new TLabel(this);
         Mass_label[i][j]= &TLabel(Form1->GroupBox1);
         Mass_label[i][j]->Height=20;
         Mass_label[i][j]->Width=40;
         Mass_label[i][j]->Caption="X"+IntToStr(j+1);
         if(j<n-2)
         Mass_label[i][j]->Caption=Mass_label[i][j]->Caption+"+";
         if(i==0&&j==n-2)
         Mass_label[i][j]->Caption=Mass_label[i][j]->Caption+"+";
         Mass_label[i][j]->Left = 65+70*j;
         Mass_label[i][j]->Top = 48+30*i;
         if(i>=1) Mass_label[i][j]->Top += 20;
         Mass_label[i][j]->Visible = true;
         Mass_label[i][j]->Parent = Form1->GroupBox1;
        }
    } // for (int i = 0; i < m; i++) заканчивается

    for (int j = 0; j < m; j++)
    {
        Mass_cb[j]  = new TComboBox(this);
        Mass_cb[j] = &TComboBox(Form1->GroupBox1);
        Mass_cb[j]->Height=30;
        if(j>=1)
        Mass_cb[j]->Width=50;
        else Mass_cb[j]->Width=60;
        if(j>=1)
        Mass_cb[j]->Left = sm+75;
        else
        Mass_cb[j]->Left = sm+125;
        Mass_cb[j]->Top = 45+30*j;
        if(j>=1)  Mass_cb[j]->Top += 20;
        Mass_cb[j]->Visible = true;
        Mass_cb[j]->Parent = Form1->GroupBox1;

        if(j>=1)
        {
         Mass_cb[j]->Text="<=";
         Mass_cb[j]->Items->Add("<=");
         Mass_cb[j]->Items->Add("=");
         Mass_cb[j]->Items->Add(">=");
        }
        else
        {
         Mass_cb[j]->Text="MAX";
         Mass_cb[j]->Items->Add("MAX");
         Mass_cb[j]->Items->Add("MIN");
        }
    }

    for (int j = 0; j < n-1; j++)
    {

    Mass_ch[j]  = new TCheckBox(this);
    Mass_ch[j]= &TCheckBox(Form1->GroupBox1);
    Mass_ch[j]->Height=20;
    Mass_ch[j]->Width=80;
    Mass_ch[j]->Caption="X"+IntToStr(j+1)+">=0";
    Mass_ch[j]->Left = 30+70*j;
    Mass_ch[j]->Top = 70+30*m;
    Mass_ch[j]->Visible = true;
    Mass_ch[j]->Parent = Form1->GroupBox1;
    Mass_ch[j]->Checked=true;
    }
    m1=m;
    n1=n;

    LockWindowUpdate(NULL);
    
    li.de_init(2, 2); li.del_simplex();
    du.de_init(2, 2); du.del_simplex();
    li.nsimplex(n-1, m-1); li.init(n-1, m-1);
    du.nsimplex(m-1, n-1); du.init(m-1, n-1);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{

  LockWindowUpdate(Form1->Handle);
  //удаление массива edit
  for (int i = 0; i < m1; i++)
  for (int j = 0; j < n1; j++)
  if(Mass_edit[i][j]!=NULL)
  delete Mass_edit[i][j];
  //удаление массива label
  for (int i = 0; i < m1; i++)
  for (int j = 0; j < n1-1; j++)
  if(Mass_label[i][j]!=NULL)
  delete Mass_label[i][j];
  //удаление массива combobox
  for (int j = 0; j < m1; j++)
  delete Mass_cb[j];
  //удаление массива checkbox
  for (int j = 0; j < n1-1; j++)
  delete Mass_ch[j];
  //удаление memo
  delete Memo;
  //удаление элемента класса
  s.del_simplex();
  li.de_init(n1-1, m1-1);
  li.del_simplex();
  //==============================================
  du.de_init(m1-1, n1-1);
  du.del_simplex();
  LockWindowUpdate(NULL);
  //удаление массива для расщепления переменных
  if(zz!=NULL)
  delete[]zz;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{

 Button6->Visible=false;
 Button7->Visible=false;
 //Проверка правильного ввода
 char Key;
 int l;
 for (int i = 0; i < m; i++)
 for (int j = 0; j < n; j++)
 {
  if(Mass_edit[i][j]->Text=="") Mass_edit[i][j]->Text="0";
  for (int k = 0; k < Mass_edit[i][j]->Text.Length(); k++)
  {
   Key=Mass_edit[i][j]->Text.c_str()[k];
   if ((Key!='1')&&(Key!='2')&&(Key!='3')&&(Key!='4')&&(Key!='5')
   &&(Key!='6')&&(Key!='7')&&(Key!='8')&&(Key!='9')&&(Key!='0')
   &&(Key!='/')&&(Key!=',')&&(Key!='-'))
   {MessageBox(0,"Ошибка ввода","",MB_ICONINFORMATION);return;}
  }
 }
 //проверка combobox
  if(Mass_cb[0]->Text!="MAX"&&Mass_cb[0]->Text!="MIN") Mass_cb[0]->Text="MAX";
  for (int i = 0; i < m-1; i++)
  if(Mass_cb[i+1]->Text!="<="&&Mass_cb[i+1]->Text!="="&&Mass_cb[i+1]->Text!=">=")
  Mass_cb[i+1]->Text="<=";

  //удаление старой таблицы для дв задачи
 if(CheckBox1->Checked==False)
 {
  for(int i=0;i<StringGrid2->ColCount;i++)
  for(int j=0;j<StringGrid2->ColCount;j++)
  StringGrid2->Cells[i][j]="";
  Panel4->Caption="";
  Button5->Visible=false;
 }

 //удаление старой таблицы для пр задачи
 if(CheckBox2->Checked==False)
 {
  for(int i=0;i<StringGrid2->ColCount;i++)
  for(int j=0;j<StringGrid2->ColCount;j++)
  StringGrid1->Cells[i][j]="";
  Panel3->Caption="";
  Button4->Visible=false;
 }

 if(CheckBox1->Checked==False&&CheckBox2->Checked==False)
 {MessageBox(0,"Выберите задачу для решения","",MB_ICONINFORMATION);return;}

 //если "прямая задача"
 if(Line==FALSE&&Doubl==FALSE)
 if(CheckBox2->Checked==True)
 {
  Line=TRUE;
  Button2->Click();
  Line=FALSE;
 }
 else Line=FALSE;

 //если "двойств задача"
 int a;
 if(Doubl==FALSE&&Line==False)
 if(CheckBox1->Checked==True)
 {
  Doubl=TRUE;
  Button2->Click();
  Doubl=FALSE;
  a = n; n = m; m = a;
 }
 else Doubl=FALSE;

 if(CheckBox1->Checked==False&&CheckBox2->Checked==False)
 return;

 //установка меню и кнопок для сохранения //прямая задача
 if(Line==TRUE)
 {
  N1->Enabled=true;
  Button4->Visible=true;
 }
  //установка меню и кнопок для сохранения //двойств задача
 if(Doubl==TRUE)
 {
  N2->Enabled=true;
  Button5->Visible=true;
 }


 //n - количество переменных +1
 //m - количество ограничений +1



 //минимум или максимум
 if(Line==TRUE||(Line==FALSE&&Doubl==True))//если есть прямая задача
 //или только двойственная
 if(Mass_cb[0]->Text=="MAX")
 minmax=0;
 else minmax=1;

 //число переменных и ограничений меняется при переходе к дв задаче
 if(Doubl==TRUE)
 {a = n; n = m; m = a;}


 //буфер для записи
 AnsiString decs="";



 //установка элемента класса и удаление старого
  s.del_simplex();
  s.nsimplex(n-1, m-1);


 //установка целевой функции пр задача
 if(Line==True)
 {
  for(int i = 0;i<n-1;i++)
  if(minmax==0)
  s.get_obj()[i]=get_fr(Mass_edit[0][i]->Text);
  else
  s.get_obj()[i]=get_fr("-"+Mass_edit[0][i]->Text);//при мин меняется знак
 }
 
 //установка целевой функции обр задача
 if(Doubl==True)
 {
   for(int i = 0;i<n-1;i++)
   if(zz[i].get_num()==2) //если был знак >=
   s.get_obj()[i]=get_fr(Mass_edit[i+1][m-1]->Text);
   else
   s.get_obj()[i]=get_fr("-"+Mass_edit[i+1][m-1]->Text);
 }

 //установка свободных членов пр задача
 if(Line==True)
 for(int i = 0;i<m-1;i++)
 s.get_freed()[i]=get_fr(Mass_edit[i+1][n-1]->Text);

 //установка свободных членов дв задача
 if(Doubl==True)
 {
  for(int i = 0;i<m-1;i++)
  if(minmax==0)
  s.get_freed()[i]=get_fr(Mass_edit[0][i]->Text);
  else
  s.get_freed()[i]=get_fr("-"+Mass_edit[0][i]->Text);
 }

 //установка нач значения цел функции пр задача
 if(Line==True)
 {
  if(minmax==0)
  s.get_freed()[m-1]=get_fr("-"+Mass_edit[0][n-1]->Text);
  else
  s.get_freed()[m-1]=get_fr(Mass_edit[0][n-1]->Text);
 }
 
 //установка нач значения цел функции обр задача
 if(Doubl==True)
 {
  if(minmax==0)
  s.get_freed()[m-1]=get_fr(Mass_edit[0][m-1]->Text);
  else
  s.get_freed()[m-1]=get_fr("-"+Mass_edit[0][m-1]->Text);
 }



  
 //установка знаков 
 if(Line==true)
 for(int i = 0;i<m-1;i++) //цикл по числу ограничений
 {
  if(Mass_cb[i+1]->Text=="<=")
  s.get_symb()[i].set_num(0);
  if(Mass_cb[i+1]->Text=="=")
  s.get_symb()[i].set_num(1);
  if(Mass_cb[i+1]->Text==">=")
  s.get_symb()[i].set_num(2);
  zz[i].set_num(s.get_symb()[i].num);
 }

 fraction frac;
 if(Doubl==true)
 for(int i = 0;i<n-1;i++) //цикл по числу переменных
 {
  if(Mass_cb[i+1]->Text=="<=")
  frac.set_num(0);
  if(Mass_cb[i+1]->Text=="=")
  frac.set_num(1);
  if(Mass_cb[i+1]->Text==">=")
  frac.set_num(2);
  zz[i].set_num(frac.num);
 }

  //установка для дв задачи знаков
 if(Doubl==TRUE)
 for(int i = 0;i<m-1;i++)
 if(Mass_ch[i]->Checked==false)
 s.get_symb()[i].set_num(1);
 else
 s.get_symb()[i].set_num(2);

 //установка коэффициентов пр задача
 if(Line==true)
 for(int i=0;i<m-1;i++)
 for(int j=0;j<n-1;j++)
 s.get_res()[i][j]=get_fr(Mass_edit[i+1][j]->Text);

 //установка коэффициентов обр задача
 if(Doubl==true)
 for(int i=0;i<m-1;i++)
 for(int j=0;j<n-1;j++)
 {
  if(zz[j].get_num()==2)
  s.get_res()[i][j]=get_fr("-"+Mass_edit[j+1][i]->Text);
  else
  s.get_res()[i][j]=get_fr(Mass_edit[j+1][i]->Text);
 }

 //установка какие переменные надо расщепить
 if(Doubl==TRUE)
 for(int j=0;j<n-1;j++)
 s.get_dis()[j]=zz[j]; //при дв задаче проверка по предыдущей

 if(Line==TRUE)
 for(int j=0;j<n-1;j++)
 if(Mass_ch[j]->Checked==false)
 s.get_dis()[j].set_num(1);   //в пр задаче проверка по checkbox
 

  s.init(n-1, m-1);//получение первой таблицы

  //Memo для HTML
  Memo->Lines->Clear();
  Memo->Lines->Add( "<HTML>" );

  if(Line==True)
  Memo->Lines->Add( "<HEAD><TITLE>Решение задачи линейного программирования</TITLE></HEAD>" );
  if(Doubl==True)
  Memo->Lines->Add( "<HEAD><TITLE>Решение двойственной задачи линейного программирования</TITLE></HEAD>" );

  Memo->Lines->Add( "<BODY>" );

  //вывод в буфер
  decs=add_title(s);
  Memo->Lines->Add(decs);


  //добавление таблицы
  decs="";

  decs=add_table(s, m, n);
  Memo->Lines->Add(decs);
  //таблица добавлена

  decs="";


 //=======================================================================================//
 //                                                                                       //
 //=======================================================================================//
 int n_it=0;
 while(s.get_dif()==FALSE)
 {

  n_it=s.calculation(n-1, m-1); //получение остальных таблиц


  if(s.get_not()!=0)break;   //если получена ошибка то break
  
  if(s.get_isk()==TRUE)
  decs+="<p>Переходим к исходной целевой функции.<p> ";


  if(s.get_isk()==FALSE)
  if(s.get_doub()==TRUE)
  decs+="Используем двойственный симплекс-метод. ";
  else decs+="Используем обычный симплекс-метод. ";

  if(s.get_next()!=0)
  if(s.get_isk()==FALSE)
  if(Line==TRUE)
  {
   decs+="Вводим в базис X"+IntToStr(s.get_next())+", ";
   decs+="выводим из базиса X"+IntToStr(s.get_prev())+"<p>";
  }

  if(s.get_next()!=0)
  if(s.get_isk()==FALSE)
  if(Doubl==TRUE)
  {
   decs+="Вводим в базис Y"+IntToStr(s.get_next())+", ";
   decs+="выводим из базиса Y"+IntToStr(s.get_prev())+"<p>";
  }

  //добавление таблицы

  decs+=add_table(s, m, n);
  Memo->Lines->Add(decs);
  decs="";
 }

//
 //добавление оконч таблицы
 if(Line==True)
 {
  StringGrid1->Cells[0][0]="БП";
  StringGrid1->Cells[1][0]="СЧ";
  StringGrid1->ColCount=1+n+s.get_k()+s.get_r()+s.get_l();
  for (int k = 1; k < n+s.get_k()+s.get_r()+s.get_l(); k++)
  StringGrid1->Cells[k+1][0]= "X"+IntToStr(k);

  StringGrid1->RowCount=m+1;

  for (int i = 0; i < m; i++)
  for (int j = 0; j < n+s.get_k()+s.get_r()+s.get_l()+1; j++)
   {

     if(j==0&&i!=m-1)
     StringGrid1->Cells[j][i+1] = "X" + IntToStr(s.get_base()[i]);


     if(j==0&&i==m-1)
     {
      if(s.get_r()==0)
      StringGrid1->Cells[j][i+1]= "Y";
      else StringGrid1->Cells[j][i+1]= "F'";
     }

     if(j==1)
     {
      StringGrid1->Cells[j][i+1]=IntToStr(s.get_nfreed()[i].get_num());
      if(s.get_nfreed()[i].get_num()!=0&&s.get_nfreed()[i].get_den()!=1)
      StringGrid1->Cells[j][i+1]=StringGrid1->Cells[j][i+1]+"/"+IntToStr(s.get_nfreed()[i].get_den());
     }

     if(j>1&&i!=m-1)
     {
       StringGrid1->Cells[j][i+1]=IntToStr(s.get_nres()[i][j-2].get_num());
       if(s.get_nres()[i][j-2].get_num()!=0&&s.get_nres()[i][j-2].get_den()!=1)
       StringGrid1->Cells[j][i+1]=StringGrid1->Cells[j][i+1]+"/"+IntToStr(s.get_nres()[i][j-2].get_den());
     }

      if(j>1&&i==m-1)
      {
       StringGrid1->Cells[j][i+1]=IntToStr(s.get_nobj()[j-2].get_num());
       if(s.get_nobj()[j-2].get_num()!=0&&s.get_nobj()[j-2].get_den()!=1)
       StringGrid1->Cells[j][i+1]=StringGrid1->Cells[j][i+1]+"/"+IntToStr(s.get_nobj()[j-2].get_den());
      }
  }
 }

 if(Doubl==True) //оконч таблица для дв задачи
 {

  StringGrid2->Cells[0][0]="БП";
  StringGrid2->Cells[1][0]="СЧ";
  StringGrid2->ColCount=1+n+s.get_k()+s.get_r()+s.get_l();
  for (int k = 1; k < n+s.get_k()+s.get_r()+s.get_l()+1; k++)
  StringGrid2->Cells[k+1][0]= "Y"+IntToStr(k);

  StringGrid2->RowCount=m+1;

  for (int i = 0; i < m; i++)
  for (int j = 0; j < n+s.get_k()+s.get_r()+s.get_l()+1; j++)
   {

     if(j==0&&i!=m-1)
     StringGrid2->Cells[j][i+1] = "Y" + IntToStr(s.get_base()[i]);


     if(j==0&&i==m-1)
     {
      if(s.get_r()==0)
      StringGrid2->Cells[j][i+1]= "S";
      else StringGrid2->Cells[j][i+1]= "F'";
     }

     if(j==1)
     {
      StringGrid2->Cells[j][i+1]=IntToStr(s.get_nfreed()[i].get_num());
      if(s.get_nfreed()[i].get_num()!=0&&s.get_nfreed()[i].get_den()!=1)
      StringGrid2->Cells[j][i+1]=StringGrid2->Cells[j][i+1]+"/"+IntToStr(s.get_nfreed()[i].get_den());
     }

     if(j>1&&i!=m-1)
     {
       StringGrid2->Cells[j][i+1]=IntToStr(s.get_nres()[i][j-2].get_num());
       if(s.get_nres()[i][j-2].get_num()!=0&&s.get_nres()[i][j-2].get_den()!=1)
       StringGrid2->Cells[j][i+1]=StringGrid2->Cells[j][i+1]+"/"+IntToStr(s.get_nres()[i][j-2].get_den());
     }

      if(j>1&&i==m-1)
      {
       StringGrid2->Cells[j][i+1]=IntToStr(s.get_nobj()[j-2].get_num());
       if(s.get_nobj()[j-2].get_num()!=0&&s.get_nobj()[j-2].get_den()!=1)
       StringGrid2->Cells[j][i+1]=StringGrid2->Cells[j][i+1]+"/"+IntToStr(s.get_nobj()[j-2].get_den());
      }
  }
 }

 decs=add_answer(s);
 fraction x[50];

 fraction answ = s.get_answ();
 if(s.get_not()==0)
 {

  int j=0;
  fraction mm;
  fraction zero;

  for(int i=0;i<n-1+s.get_k();i++)
  {
   x[i]=zero;
  }
  //получение ответа
  for(int i=0;i<m-1;i++)
  {
   j=s.get_base()[i];
   x[j-1]=s.get_nfreed()[i];;
   j++;
  }

  //возвращение переменной
  l=0;
  fraction fr;
  for(int i=0;i<n-1;i++)
  {
   if(s.get_dis()[i].get_num()==1)
   {
    x[i]=x[i]+x[n+l+s.get_k()-1].men(x[n+l+s.get_k()-1]);
    fr=x[i];
    l++;
   }
  }


  decs+=" Число итераций: "+IntToStr(n_it);
 }

 Memo->Lines->Add(decs);
 decs="";

 Memo->Lines->Add( "</BODY>" );
 Memo->Lines->Add( "</HTML>" );


 //================================================================
 if(Line==true)  Memo->Lines->SaveToFile("Simplex.htm");
 if(Doubl==true) Memo->Lines->SaveToFile("Double_simplex.htm");
 Memo->Lines->Clear();


 if(Line==true)  Panel3->Caption="Ответ: ";
 if(Doubl==true) Panel4->Caption="Ответ: ";

 //если целевая ф-ция оптимальна
 if(s.get_not()==0)
 {
  if(Line==true)
  for(int i=0;i<n-1+s.get_k();i++)
  {
   Panel3->Caption=Panel3->Caption+"X"+IntToStr(i+1)+"="+IntToStr(x[i].get_num());
   if(x[i].get_num()!=0&&x[i].get_den()!=1)
   Panel3->Caption=Panel3->Caption+"/"+IntToStr(x[i].get_den())+"; ";
   else Panel3->Caption=Panel3->Caption+"; ";
  }

  if(Doubl==true)
  for(int i=0;i<n-1+s.get_k();i++)
  {
   Panel4->Caption=Panel4->Caption+"Y"+IntToStr(i+1)+"="+IntToStr(x[i].get_num());
   if(x[i].get_num()!=0&&x[i].get_den()!=1)
   Panel4->Caption=Panel4->Caption+"/"+IntToStr(x[i].get_den())+"; ";
   else Panel4->Caption=Panel4->Caption+"; ";
  }
 }

 if(Line==true)
 {
  if(s.get_not()!=0)
  {
   Panel3->Caption="Ответ: Целевая функция не ограничена, причина №"+IntToStr(s.get_not());
   if(s.get_not()==1)
   Panel3->Caption=Panel3->Caption+": В столбце над отриц. элементом нет положительных";
   if(s.get_not()==2)
   Panel3->Caption=Panel3->Caption+": В строке справа от отриц. элемента нет отрицательных";
   if(s.get_not()==3)
   Panel3->Caption=Panel3->Caption+": Искусственная целевая функция осталась отрицательна";
  }
 }
 
 if(Doubl==true)
 {
  if(s.get_not()!=0)
  Panel4->Caption="Ответ: Целевая функция не ограничена, причина №"+IntToStr(s.get_not());
  if(s.get_not()==1)
  Panel4->Caption=Panel4->Caption+": В столбце над отриц. элементом нет положительных";
  if(s.get_not()==2)
  Panel4->Caption=Panel4->Caption+": В строке справа от отриц. элемента нет отрицательных";
  if(s.get_not()==3)
  Panel4->Caption=Panel4->Caption+": Искусственная целевая функция осталась отрицательна";
 }

 //деинициализация массивов для итераций
 s.de_init(n-1, m-1);




 if(s.get_not()==0)
 {
  if(Line==true)
  {
   if(minmax==1)
   Panel3->Caption = Panel3->Caption+" Y="+IntToStr(-answ.get_num());
   else
   Panel3->Caption = Panel3->Caption+" Y="+IntToStr(answ.get_num());
   if(answ.get_num()!=0&&answ.get_den()!=1)
   Panel3->Caption = Panel3->Caption+"/"+IntToStr(answ.get_den());
   Panel3->Caption = Panel3->Caption+" Число итераций: "+IntToStr(n_it);
  }
  
   if(Doubl==true)
   {
   if(minmax==0)
   Panel4->Caption  = Panel4->Caption + " S="+IntToStr(-answ.get_num());
   else
   Panel4->Caption  = Panel4->Caption + " S="+IntToStr(answ.get_num());
   if(answ.get_num()!=0&&answ.get_den()!=1)
   Panel4->Caption = Panel4->Caption + "/"+IntToStr(answ.get_den());
   Panel4->Caption = Panel4->Caption+" Число итераций: "+IntToStr(n_it);
  }
 }
 
 TabSheet2->Show();

}
//---------------------------------------------------------------------------
//создание формы
void __fastcall TForm1::FormCreate(TObject *Sender)
{

 Memo = new TCustomMemo(this);
 Memo->Visible = false;
 Memo->Parent = this;
 s.nsimplex(1, 1);
 li.nsimplex(2, 2);
 li.init(2, 2);
 du.nsimplex(2, 2);
 du.init(2, 2);
 Doubl = False;
 Line = False;
 Button1->Click();
 Button4->Visible=false;
 Button5->Visible=false;

 N1->Enabled=false;
 N2->Enabled=false;


}
//---------------------------------------------------------------------------

AnsiString Str="";
//сохранение отчета 1
void __fastcall TForm1::Button4Click(TObject *Sender)
{
 if( SaveDialog1->Execute())
 {

  Str = ExtractFilePath(Application->ExeName);
  Str=Str+"Simplex.htm";
  CopyFile(Str.c_str(),(SaveDialog1->FileName+".doc").c_str(),false);
  ShellExecute(Handle, NULL,(SaveDialog1->FileName+".doc").c_str(),NULL, NULL,SW_RESTORE);

 }
}
//---------------------------------------------------------------------------
//сохранение 2
void __fastcall TForm1::Button5Click(TObject *Sender)
{
 if( SaveDialog2->Execute())
 {

  Str = ExtractFilePath(Application->ExeName);
  Str=Str+"Double_simplex.htm";
  CopyFile(Str.c_str(),(SaveDialog2->FileName+".doc").c_str(),false);
  ShellExecute(Handle, NULL,(SaveDialog2->FileName+".doc").c_str(),NULL, NULL,SW_RESTORE);

 }
}
//---------------------------------------------------------------------------
//ввод из строки в дробь
fraction get_fr(AnsiString text)
{
 fraction num;
 AnsiString text2;
 if(text.c_str()[0]=='-'&&text.c_str()[1]=='-')
 {
  for(int i=2;i<text.Length();i++)
  text2+=text.c_str()[i];
  text=text2;
 } 
 AnsiString n="", d="1";
 int n1, d1;
 int a=0;
 for(int i=0;i<text.Length();i++)
 {
  if(text.c_str()[i]!='/'&&text.c_str()[i]!=','&&a!=1)
  n+=text.c_str()[i];
  if(text.c_str()[i]=='/')
  {a++;i++;d="";}
  if(a==1)
  d+=text.c_str()[i];
  if(text.c_str()[i]==',')
  a+=2;
  if(a==2&&i!=text.Length()-1)
  d+="0";
 }

 a=num.nod(StrToInt(n),StrToInt(d));
 n1=StrToInt(n)/a;
 d1=StrToInt(d)/a;
 num.set_num(n1);
 num.set_den(d1);
 return num;
}
//---------------------------------------------------------------------------
AnsiString add_title(simplex s) //добавление заголовка в буфер
{
  AnsiString decs;
  decs="<meta http-equiv=\"Content-Type\" content=\"text/html;charset=windows-1251\">";
  decs+="Решение задачи линейного программирования<p>Целевая функция:<p>";


  //вывод целевой функции при max  и прямой задаче
  if(Line==True)
  {decs+="Y=";
   if(minmax==0)
   {
    for(int i=0;i<n-1;i++)
    {
     decs+=IntToStr(s.get_obj()[i].get_num());
     if(s.get_obj()[i].get_num()!=0&&s.get_obj()[i].get_den()!=1)
     decs+="/"+IntToStr(s.get_obj()[i].get_den());
     decs+="*X";
     decs+=IntToStr(i+1);

     if(i!=n-2&&s.get_obj()[i+1].get_num()>=0)
     decs+='+';
     
     if(i==n-2)
     {
     if(-s.get_freed()[m-1].get_num()>=0)
     decs+="+";
     decs+=IntToStr(-s.get_freed()[m-1].get_num());
     if(-s.get_freed()[m-1].get_num()!=0&&s.get_freed()[m-1].get_den()!=1)
     decs+="/"+IntToStr(s.get_freed()[m-1].get_den());
     }
     if(i==n-2)
     decs+="->MAX<p>";
    }
   }
  }
  
  //вывод целевой функции при обратной задаче
  if(Doubl==TRUE)
  {
   decs+="S=";
   for(int i=0;i<n-1;i++)
   {
    if(minmax==0)
    decs+=IntToStr(-s.get_obj()[i].get_num());
    else
    decs+=IntToStr(s.get_obj()[i].get_num());

    if(s.get_obj()[i].get_num()!=0&&s.get_obj()[i].get_den()!=1)
    decs+="/"+IntToStr(s.get_obj()[i].get_den());
    decs+="*Y";
    decs+=IntToStr(i+1);
    if(minmax==0)
    {
     if(i!=n-2&&-s.get_obj()[i+1].get_num()>=0)
     decs+='+';
    }
     else
    {
     if(i!=n-2&&s.get_obj()[i+1].get_num()>=0)
     decs+='+';
    }
    
     if(i==n-2)
     {
      if(minmax==0)
      if(s.get_freed()[m-1].get_num()>=0) decs+="+";
      if(minmax==1)
      if(-s.get_freed()[m-1].get_num()>=0)  decs+="+";
      if(minmax==0)
      decs+=IntToStr(s.get_freed()[m-1].get_num());
      else
      decs+=IntToStr(-s.get_freed()[m-1].get_num());
      if(s.get_freed()[m-1].get_num()!=0&&s.get_freed()[m-1].get_den()!=1)
      decs+="/"+IntToStr(s.get_freed()[m-1].get_den());
     }

    if(i==n-2)
    if(minmax==0)
    decs+="->MIN<p>";
    else
    decs+="->MAX<p>";
   }
    if(minmax==0)
    {
    decs+="Меняем знак целевой функции:<p>";
    decs+="S=";
    }
  }

  //обмен знака
  if(minmax==0&&Doubl==TRUE)
  {
   for(int i=0;i<n-1;i++)
   {
   decs+=IntToStr(s.get_obj()[i].get_num());
   if(s.get_obj()[i].get_num()!=0&&s.get_obj()[i].get_den()!=1)
   decs+="/"+IntToStr(s.get_obj()[i].get_den());
   decs+="*Y";
   decs+=IntToStr(i+1);
   if(i!=n-2&&s.get_obj()[i+1].get_num()>=0) decs+='+';

    if(i==n-2)
    {
     if(-s.get_freed()[m-1].get_num()>=0) decs+="+";
     decs+=IntToStr(-s.get_freed()[m-1].get_num());
     if(-s.get_freed()[m-1].get_num()!=0&&s.get_freed()[m-1].get_den()!=1)
     decs+="/"+IntToStr(s.get_freed()[m-1].get_den());
    }

   if(i==n-2)
   decs+="->MAX<p>";
   }
  }

  if(minmax==1&&Line==True)
  {
   for(int i=0;i<n-1;i++)
   {
   decs+=IntToStr(-s.get_obj()[i].get_num());
   if(s.get_obj()[i].get_num()!=0&&s.get_obj()[i].get_den()!=1)
   decs+="/"+IntToStr(s.get_obj()[i].get_den());
   decs+="*X";
   decs+=IntToStr(i+1);
   if(i!=n-2&&-s.get_obj()[i+1].get_num()>=0) decs+='+';

    if(i==n-2)
    {
    if(s.get_freed()[m-1].get_num()>=0)
    decs+="+";
    decs+=IntToStr(s.get_freed()[m-1].get_num());
    if(s.get_freed()[m-1].get_num()!=0&&s.get_freed()[m-1].get_den()!=1)
    decs+="/"+IntToStr(s.get_freed()[m-1].get_den());
    }

   if(i==n-2)
   decs+="->MIN<p>";
   }
   decs+="Меняем знак целевой функции:<p>";
   decs+="Y=";
  }

  //обмен знака
  if(minmax==1&&Line==True)
  for(int i=0;i<n-1;i++)
  {
   decs+=IntToStr(s.get_obj()[i].get_num());
   if(s.get_obj()[i].get_num()!=0&&s.get_obj()[i].get_den()!=1)
   decs+="/"+IntToStr(s.get_obj()[i].get_den());
   decs+="*X";
   decs+=IntToStr(i+1);
   if(i!=n-2&&s.get_obj()[i+1].get_num()>=0) decs+='+';

    if(i==n-2)
    {
    if(-s.get_freed()[m-1].get_num()>=0)
    decs+="+";
    decs+=IntToStr(-s.get_freed()[m-1].get_num());
    if(-s.get_freed()[m-1].get_num()!=0&&s.get_freed()[m-1].get_den()!=1)
    decs+="/"+IntToStr(s.get_freed()[m-1].get_den());
    }

   if(i==n-2)
   decs+="->MAX<p>";
  }

  //вывод ограничений
  decs+="При наличии ограничений:<p>";
  for(int i=0;i<m-1;i++)
  {
   for(int j=0;j<n-1;j++)
   {

    decs+=IntToStr(s.get_res()[i][j].get_num());
    if(s.get_res()[i][j].get_num()!=0&&s.get_res()[i][j].get_den()!=1)
    decs+="/"+IntToStr(s.get_res()[i][j].get_den());
    if(Doubl==FALSE)
    decs+="*X";
    else decs+="*Y";
    decs+=IntToStr(j+1);



    if(j!=n-2&&s.get_res()[i][j+1].get_num()>=0) decs+='+';

    if(j==n-2)
    {
     if(s.get_symb()[i].get_num()==0)
     decs+="<=";
     if(s.get_symb()[i].get_num()==1)
     decs+="=";
     if(s.get_symb()[i].get_num()==2)
     decs+=">=";
     decs+=IntToStr(s.get_freed()[i].get_num());
     if(s.get_freed()[i].get_num()!=0&&s.get_freed()[i].get_den()!=1)
     decs+="/"+IntToStr(s.get_freed()[i].get_den());
     decs+="<p>";
    }
   }
  }


  if(s.get_k()!=0) //если доп переменные вводятся
  {
   decs+="<p><p>Приводим ограничения к канонической форме.<p>";
   decs+="Для этого вводим дополняющие переменные: ";
   for(int i=0;i<s.get_k();i++)
   {
    if(Line==True) decs+="X";  //пр задача
    if(Doubl==True) decs+="Y"; //обр задача
    decs+=IntToStr(n+i);
    if(i!=s.get_k()-1)
    decs+=", ";
   }
   decs+=", чтобы избавиться от неравенств. <p>";


   } //if(s.get_k()!=0)



   if(s.get_r()!=0) //если вводится иск переменная
  {
   decs+="Необходимо ввести искуственные переменные: ";
   for(int i=0;i<s.get_r();i++)
   {
    if(Line==True)  decs+="X";
    if(Doubl==True) decs+="Y";
    decs+=IntToStr(s.get_k()+n+i);
    if(i!=s.get_r()-1)
    decs+=", ";
   }
   

   } //if(s.get_r()!=0)

  if(s.get_l()!=0) //если расщ переменные вводятся
  {
   int e=0;
   decs+="<p><p>Есть переменные, на которые наложено ограничение равенства.<p>";
   for(int i=0;i<n-1;i++)
   if(s.get_dis()[i].get_num()==1)

   {
    decs+="Представляем переменную ";
    if(Line==True)  decs+="X";
    if(Doubl==True) decs+="Y";
    decs+=IntToStr(i+1);
    decs+=" в виде: ";
    if(Line==True)  decs+="X";
    if(Doubl==True) decs+="Y";
    decs+=IntToStr(i+1);
    decs+="-";
    if(Line==True)  decs+="X";
    if(Doubl==True) decs+="Y";
    decs+=IntToStr(e+n+s.get_k());
    decs+=".<p>";
    e++;
    }
   } //if(s.get_l()!=0


   decs+="<p><p>Выразим базис в форме Таккера:<p>";
   for(int i=0;i<m-1;i++)
   for(int j=0;j<n-1+s.get_k()+s.get_r()+s.get_l();j++)   //по числу переменных
   {

     if(
        (s.get_r()==0&&(j<n-1||j>n-2+s.get_k()))||
        (s.get_r()!=0&&(j<n-1+s.get_k()+s.get_l()||j>n-2+s.get_k()+s.get_l()+s.get_r()))
       )
     {

     if(
        (s.get_r()==0&&j==n-1+s.get_k())||
        (s.get_r()!=0&&j==n-1+s.get_k()+s.get_r())
       )
     if(s.get_nres()[i][j].get_num()>=0)
     decs+='+';

     if(j==0)
     {
      if(s.get_r()!=0)
      {
       if(Line==True)  decs+="X";
       if(Doubl==True) decs+="Y";
       decs+=IntToStr(n+i+s.get_k()+s.get_l())+"=";  //"X#="
      }
      else
      {
       if(Line==True)  decs+="X";
       if(Doubl==True) decs+="Y";
       decs+=IntToStr(n+i)+"=";  //"X#="
      }
     }

      if(j==0)
      {
       decs+=IntToStr(s.get_nfreed()[i].get_num());
       if(s.get_nfreed()[i].get_num()!=0&&s.get_nfreed()[i].get_den()!=1)
       decs+="/"+IntToStr(s.get_nfreed()[i].get_den());
       decs+="-(";
      }

      decs+=IntToStr(s.get_nres()[i][j].get_num());
      if(s.get_nres()[i][j].get_num()!=0&&s.get_nres()[i][j].get_den()!=1)
      decs+="/"+IntToStr(s.get_nres()[i][j].get_den());
      if(Line==True)  decs+="*X";
      if(Doubl==True) decs+="*Y";
      decs+=IntToStr(j+1);

      if(
        (s.get_r()==0&&(j+1<n-1||j>n-2+s.get_k()))||
        (s.get_r()!=0&&(j+1<n-1+s.get_k()+s.get_l()||j>n-2+s.get_k()+s.get_r()+s.get_l()))
       )
      if(j!=n-2+s.get_k()+s.get_r()+s.get_l()&&s.get_nres()[i][j+1].get_num()>=0)
      decs+='+';

      }//if

      if(j==n-2+s.get_k()+s.get_r()+s.get_l())
      decs+=")<p>";


     }//for/for


     if(s.get_r()==0)
     {
      decs+="Целевая функция в форме Таккера: <p>";
      if(Line==True)  decs+="Y=";
      if(Doubl==True) decs+="S=";
      decs+=IntToStr(s.get_nfreed()[m-1].get_num());
      if(s.get_nfreed()[m-1].get_num()!=0&&s.get_nfreed()[m-1].get_den()!=1)
      decs+="/"+IntToStr(s.get_nfreed()[m-1].get_den());
      decs+="-(";
     }
     else
     {
      decs+="Искусственная целевая функция F в форме Таккера";
      decs+="<p>сумма искусственных переменых и стремится к MIN <p>";
      decs+="меняем знак целевой функции:<p>";
      decs+="F'="+IntToStr(s.get_nfreed()[m-1].get_num());
      if(s.get_nfreed()[m-1].get_num()!=0&&s.get_nfreed()[m-1].get_den()!=1)
      decs+="/"+IntToStr(s.get_nfreed()[m-1].get_den());
      decs+="-(";
     }

   //целевая функция
   for(int i=0;i<n-1+s.get_k()+s.get_r()+s.get_l();i++)
   {
    decs+=IntToStr(s.get_nobj()[i].get_num());
    if(s.get_nobj()[i].get_num()!=0&&s.get_nobj()[i].get_den()!=1)
    decs+="/"+IntToStr(s.get_nobj()[i].get_den());
    if(Line==True)  decs+="*X";
    if(Doubl==True) decs+="*Y";
    decs+=IntToStr(i+1);

    if(i!=n-2+s.get_k()+s.get_r()+s.get_l()&&(s.get_nobj()[i+1].get_num())>=0)
    decs+='+';
    if(i==n-2+s.get_k()+s.get_r()+s.get_l())
    decs+=")->MАX<p>";
   }
  //------------------------------------------------------------ 
  return decs;
}
//---------------------------------------------------------------------------
AnsiString add_table(simplex s, int m, int n)
{
  AnsiString decs;
  //добавление таблицы
  decs+= "<p><table border=2 cellpadding=4 ><tr bgcolor=\"#FFFFFF\">";
  decs+= "<td align=\"center\">БП</td><td>СЧ</td>";

  //первая строка
  for (int k = 1; k < n+s.get_k()+s.get_r()+s.get_l(); k++)
  {
   decs += "<td align=\"center\">";
   if(Line==True)
   decs = decs + "X" + k;
   if(Doubl==True)
   decs = decs + "Y" + k;
   decs += "</td>";

  }
  decs += "</tr>";
  decs += "<tr>";

   //остальные строки
   for (int i = 0; i < m; i++)
   {
   for (int j = 0; j < n+s.get_k()+s.get_r()+s.get_l()+1; j++)
   {


     decs += "<td align=\"center\">";


     if(j==0&&i!=m-1)
     if(Line==True)
     decs += "X" + IntToStr(s.get_base()[i]);

     if(j==0&&i!=m-1)
     if(Doubl==True)
     decs += "Y" + IntToStr(s.get_base()[i]);

     if(j==0&&i==m-1)
     {
      if(s.get_r()==0)
      {
       if(Line==True)  decs += "Y";
       if(Doubl==True) decs += "S";
      } 
      else decs += "F'";
     }

     if(j==1)
     {
      decs+=IntToStr(s.get_nfreed()[i].get_num());
      if(s.get_nfreed()[i].get_num()!=0&&s.get_nfreed()[i].get_den()!=1)
      decs+="/"+IntToStr(s.get_nfreed()[i].get_den());
     }

     if(j>1&&i!=m-1)
     {
       decs+=IntToStr(s.get_nres()[i][j-2].get_num());
       if(s.get_nres()[i][j-2].get_num()!=0&&s.get_nres()[i][j-2].get_den()!=1)
       decs+="/"+IntToStr(s.get_nres()[i][j-2].get_den());
     }

      if(j>1&&i==m-1)
      {
       decs+=IntToStr(s.get_nobj()[j-2].get_num());
       if(s.get_nobj()[j-2].get_num()!=0&&s.get_nobj()[j-2].get_den()!=1)
       decs+="/"+IntToStr(s.get_nobj()[j-2].get_den());
      }

      decs += "</td>";

   }
   decs += "</tr>";
   if(i!=m-1)
   decs += "<tr>";

  }
  decs+="</table>";
  //таблица добавлена
  return decs;
}
//---------------------------------------------------------------------------
AnsiString add_answer(simplex s)
{
 AnsiString decs;
 fraction answ = s.get_answ();
 decs+="Ответ: ";


 fraction x[50];
 if(s.get_not()==0)
 {
  decs+="Решение оптимально<p>";
  if((Doubl==TRUE&&minmax==0)||(Line==True&&minmax==1))
  decs+="Возвращаем знак целевой функции:<p>";
  if(Line==true)  decs+="Y=";
  if(Doubl==true) decs+="S=";
  if((Doubl==TRUE&&minmax==0)||(Line==True&&minmax==1))
  decs+= IntToStr(-answ.get_num());
  else
  decs+= IntToStr(answ.get_num());
  if(answ.get_num()!=0&&answ.get_den()!=1)
  decs+="/"+IntToStr(answ.get_den());
  decs+="<p>";

  //какие переменные возвращаются
  int l=0;
  for(int j=0;j<n-1;j++)
  {
    if(s.get_dis()[j].get_num()==1)
    {
     decs+="Возвращаем переменную ";
     if(Line==True) decs+="X";
     if(Doubl==True) decs+="Y";
     decs+=IntToStr(j+1);
     decs+="=";
     if(Line==True) decs+="X";
     if(Doubl==True) decs+="Y";
     decs+=IntToStr(j+1);
     decs+="-";
     if(Line==True) decs+="X";
     if(Doubl==True) decs+="Y";
     decs+=IntToStr(n+l+s.get_k());
     decs+=".<p>";
     l++;
    }
   }


  int j=0;
  fraction a;
  fraction zero;

  for(int i=0;i<n-1+s.get_k();i++)
  {
   x[i]=zero;
  }
  //получение ответа
  for(int i=0;i<m-1;i++)
  {
   j=s.get_base()[i];
   x[j-1]=s.get_nfreed()[i];;
   j++;
  }
  
  //возвращение переменной
  l=0;
  fraction fr;
  for(int i=0;i<n-1;i++)
  {
   if(s.get_dis()[i].get_num()==1)
   {
    x[i]=x[i]+x[n+l+s.get_k()-1].men(x[n+l+s.get_k()-1]);
    fr=x[i];
    l++;
   }
  }


  for(int i=0;i<n-1+s.get_k();i++)
  {

   if(Line==True)
   decs+="X"+IntToStr(i+1)+"="+IntToStr(x[i].get_num());

   if(Doubl==True)
   {
    decs+="Y";
    decs+=IntToStr(i+1);
    decs+="=";
    decs+=IntToStr(x[i].get_num());
   }

   if(x[i].get_num()!=0&&x[i].get_den()!=1)
   decs+="/"+IntToStr(x[i].get_den())+"; ";
   else decs+="; ";
  }
  //decs+=" Число итераций: "+IntToStr(n_it);
 }
 else
 {
  decs+="Ответ: Целевая функция неограничена, причина №"+IntToStr(s.get_not());
  if(s.get_not()==1)
  decs+=": В столбце над отриц. элементом нет положительных";
  if(s.get_not()==2)
  decs+=": В строке справа от отриц. элемента нет отрицательных";
  if(s.get_not()==3)
  decs+=": Искусственная целевая функция осталась отрицательна";
 }

 return decs;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ExitClick(TObject *Sender)
{
 Form1->Close();        
}
//---------------------------------------------------------------------------

//добавление переменной при сохр ост
void __fastcall TForm1::N8Click(TObject *Sender)
{
 TabSheet1->Show();
 AnsiString ed[11][11];
 for(int i=0;i<11;i++)
 for(int j=0;j<11;j++)
 ed[i][j]="0";
 for (int i = 0; i < m; i++)
 for (int j = 0; j < n; j++)
 ed[i][j]=Mass_edit[i][j]->Text;
 CSpinEdit1->Value=n;
 Button1->Click();
 for (int i = 0; i < m; i++)
 for (int j = 0; j < n-2; j++)
 Mass_edit[i][j]->Text=ed[i][j];

 for (int i = 0; i < m; i++)
 Mass_edit[i][n-1]->Text=ed[i][n-2];
 if(n==11)
 {N8->Enabled=false;
  SpeedButton3->Enabled=false;}
}
//---------------------------------------------------------------------------
//добавление ограничения при сохр ост
void __fastcall TForm1::N9Click(TObject *Sender)
{
 TabSheet1->Show();
 AnsiString ed[11][11];
 for(int i=0;i<11;i++)
 for(int j=0;j<11;j++)
 ed[i][j]="0";
 for (int i = 0; i < m; i++)
 for (int j = 0; j < n; j++)
 ed[i][j]=Mass_edit[i][j]->Text;
 CSpinEdit2->Value=m;
 Button1->Click();
 for (int i = 0; i < m; i++)
 for (int j = 0; j < n; j++)
 Mass_edit[i][j]->Text=ed[i][j];
 if(m==11)
 {N9->Enabled=false;
 SpeedButton4->Enabled=false;}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N7Click(TObject *Sender)
{
 Form2->Show();        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N5Click(TObject *Sender)
{
 Form3->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N10Click(TObject *Sender)//открытие исх данных
{
 TabSheet1->Show();
 AnsiString FileN;
 char strdet[20];
 int voider, m, n;
 FILE *fp;
 if(OpenDialog1->Execute())
 {
  FileN=OpenDialog1->FileName;
  fp=fopen(FileN.c_str(), "rb");

  fread(&voider, sizeof(int), 1, fp);CSpinEdit2->Value=voider;
  m=voider+1;
  fread(&voider, sizeof(int), 1, fp);CSpinEdit1->Value=voider;
  n=voider+1;
  Button1->Click();
  for(int i=0;i<m;i++)
  for(int j=0;j<n;j++)
  {fread(&strdet, sizeof(strdet), 1, fp);Mass_edit[i][j]->Text=strdet;}

  for(int i=0;i<n-1;i++)
  {fread(&voider, sizeof(int), 1, fp);
   Mass_ch[i]->Checked=voider;}

  for(int i=0;i<m;i++)
  {fread(&voider, sizeof(int), 1, fp);Mass_cb[i]->ItemIndex=voider;}

  fclose(fp);
 } 
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N11Click(TObject *Sender)//сохранение исх данных
{
 TabSheet1->Show();
 AnsiString FileN;
 char strdet[20];
 int voider;
 FILE *fp;
 if(SaveDialog3->Execute())
 {
  FileN=SaveDialog3->FileName+".dat";
  fp=fopen(FileN.c_str(), "wb");
  voider=m-1;fwrite(&voider, sizeof(int), 1, fp);  //кол-во ограничений
  voider=n-1;fwrite(&voider, sizeof(int), 1, fp);  //кол-во переменных
  for(int i=0;i<m;i++)
  for(int j=0;j<n;j++)
  {
   strcpy(strdet,(Mass_edit[i][j]->Text).c_str());
   fwrite(&strdet, sizeof(strdet), 1, fp);
  }
  
  for(int i=0;i<n-1;i++)
  {voider=Mass_ch[i]->Checked;fwrite(&voider, sizeof(int), 1, fp);}
 
  for(int i=0;i<m;i++)
  {voider=Mass_cb[i]->ItemIndex; fwrite(&voider, sizeof(int), 1, fp);}

  fclose(fp);
 } 
}
//---------------------------------------------------------------------------
//пошаговый ввод
void __fastcall TForm1::Button3Click(TObject *Sender)
{


  //Проверка правильного ввода
  char Key;
  int l;
  for (int i = 0; i < m; i++)
  for (int j = 0; j < n; j++)
  {
   if(Mass_edit[i][j]->Text=="") Mass_edit[i][j]->Text="0";
   for (int k = 0; k < Mass_edit[i][j]->Text.Length(); k++)
   {
    Key=Mass_edit[i][j]->Text.c_str()[k];
    if ((Key!='1')&&(Key!='2')&&(Key!='3')&&(Key!='4')&&(Key!='5')
    &&(Key!='6')&&(Key!='7')&&(Key!='8')&&(Key!='9')&&(Key!='0')
    &&(Key!='/')&&(Key!=',')&&(Key!='-'))
    {MessageBox(0,"Ошибка ввода","",MB_ICONINFORMATION);return;}
   }
  }
  
  //проверка combobox
  if(Mass_cb[0]->Text!="MAX"&&Mass_cb[0]->Text!="MIN") Mass_cb[0]->Text="MAX";
  for (int i = 0; i < m-1; i++)
  if(Mass_cb[i+1]->Text!="<="&&Mass_cb[i+1]->Text!="="&&Mass_cb[i+1]->Text!=">=")
  Mass_cb[i+1]->Text="<=";

  //удаление старой таблицы для дв задачи
  if(CheckBox1->Checked==False)
  {
   for(int i=0;i<StringGrid2->ColCount;i++)
   for(int j=0;j<StringGrid2->ColCount;j++)
   StringGrid2->Cells[i][j]="";
   Panel4->Caption="";
  }


 //удаление старой таблицы для пр задачи
 if(CheckBox2->Checked==False)
 {
  for(int i=0;i<StringGrid2->ColCount;i++)
  for(int j=0;j<StringGrid2->ColCount;j++)
  StringGrid1->Cells[i][j]="";
  Panel3->Caption="";
 }

 if(CheckBox1->Checked==False&&CheckBox2->Checked==False)
 {MessageBox(0,"Выберите задачу для решения","",MB_ICONINFORMATION);return;}

 //если "прямая задача"
 if(Line==FALSE&&Doubl==FALSE)
 if(CheckBox2->Checked==True)
 {
  Button4->Visible=false;
  N1->Enabled=false;
  Line=TRUE;
  Button3->Click();
  Line=FALSE;
 }
 else Line=FALSE;

 //если "двойств задача"
 int a;
 if(Doubl==FALSE&&Line==False)
 if(CheckBox1->Checked==True)
 {
  Button5->Visible=false;
  N2->Enabled=false;
  Doubl=TRUE;
  Button3->Click();
  Doubl=FALSE;
  a = n; n = m; m = a;
 }
 else Doubl=FALSE;

 if(Doubl==False&&Line==False)
 return;




 //n - количество переменных +1
 //m - количество ограничений +1

 //минимум или максимум
 if(Line==TRUE||(Line==FALSE&&Doubl==True))//если есть прямая задача
 //или только двойственная
 if(Mass_cb[0]->Text=="MAX")
 minmax=0;
 else minmax=1;

 //число переменных и ограничений меняется при переходе к дв задаче
 if(Doubl==TRUE)
 {a = n; n = m; m = a;}


 //установка элемента класса и удаление старого
 if(Line==TRUE)
 {
  li.del_simplex();
  li.nsimplex(n-1, m-1);
 }
 if(Doubl==TRUE)
 {
  du.del_simplex();
  du.nsimplex(n-1, m-1);
 }



 //установка целевой функции пр задача
 if(Line==True)
 {
  for(int i = 0;i<n-1;i++)
  if(minmax==0)
  li.get_obj()[i]=get_fr(Mass_edit[0][i]->Text);
  else
  li.get_obj()[i]=get_fr("-"+Mass_edit[0][i]->Text);//при мин меняется знак
 }

 //установка целевой функции обр задача
 if(Doubl==True)
 {
   for(int i = 0;i<n-1;i++)
   if(zz[i].get_num()==2) //если был знак >=
   du.get_obj()[i]=get_fr(Mass_edit[i+1][m-1]->Text);
   else
   du.get_obj()[i]=get_fr("-"+Mass_edit[i+1][m-1]->Text);
 }

 //установка свободных членов пр задача
 if(Line==True)
 for(int i = 0;i<m-1;i++)
 li.get_freed()[i]=get_fr(Mass_edit[i+1][n-1]->Text);

 //установка свободных членов дв задача
 if(Doubl==True)
 {
  for(int i = 0;i<m-1;i++)
  if(minmax==0)
  du.get_freed()[i]=get_fr(Mass_edit[0][i]->Text);
  else
  du.get_freed()[i]=get_fr("-"+Mass_edit[0][i]->Text);
 }

 //установка нач значения цел функции пр задача
 if(Line==True)
 {
  if(minmax==0)
  li.get_freed()[m-1]=get_fr("-"+Mass_edit[0][n-1]->Text);
  else
  li.get_freed()[m-1]=get_fr(Mass_edit[0][n-1]->Text);
 }

 //установка нач значения цел функции обр задача
 if(Doubl==True)
 {
  if(minmax==0)
  du.get_freed()[m-1]=get_fr(Mass_edit[0][m-1]->Text);
  else
  du.get_freed()[m-1]=get_fr("-"+Mass_edit[0][m-1]->Text);
 }

 //установка знаков
 if(Line==true)
 for(int i = 0;i<m-1;i++) //цикл по числу ограничений
 {
  if(Mass_cb[i+1]->Text=="<=")
  li.get_symb()[i].set_num(0);
  if(Mass_cb[i+1]->Text=="=")
  li.get_symb()[i].set_num(1);
  if(Mass_cb[i+1]->Text==">=")
  li.get_symb()[i].set_num(2);
  zz[i].set_num(li.get_symb()[i].num);
 }

 fraction frac;
 if(Doubl==true)
 for(int i = 0;i<n-1;i++) //цикл по числу переменных
 {
  if(Mass_cb[i+1]->Text=="<=")
  frac.set_num(0);
  if(Mass_cb[i+1]->Text=="=")
  frac.set_num(1);
  if(Mass_cb[i+1]->Text==">=")
  frac.set_num(2);
  zz[i].set_num(frac.num);
 }

  //установка для дв задачи знаков
 if(Doubl==TRUE)
 for(int i = 0;i<m-1;i++)
 if(Mass_ch[i]->Checked==false)
 du.get_symb()[i].set_num(1);
 else
 du.get_symb()[i].set_num(2);

 //установка коэффициентов пр задача
 if(Line==true)
 for(int i=0;i<m-1;i++)
 for(int j=0;j<n-1;j++)
 li.get_res()[i][j]=get_fr(Mass_edit[i+1][j]->Text);

 //установка коэффициентов обр задача
 if(Doubl==true)
 for(int i=0;i<m-1;i++)
 for(int j=0;j<n-1;j++)
 {
  if(zz[j].get_num()==2)
  du.get_res()[i][j]=get_fr("-"+Mass_edit[j+1][i]->Text);
  else
  du.get_res()[i][j]=get_fr(Mass_edit[j+1][i]->Text);
 }

 
 //установка какие переменные надо расщепить
 if(Doubl==TRUE)
 for(int j=0;j<n-1;j++)
 du.get_dis()[j]=zz[j]; //при дв задаче проверка по предыдущей


 if(Line==TRUE)
 for(int j=0;j<n-1;j++)
 if(Mass_ch[j]->Checked==false)
 li.get_dis()[j].set_num(1);   //в пр задаче проверка по checkbox


  if(Line==TRUE)  li.de_init(n-1, m-1);//удаление ст таблицы
  if(Doubl==TRUE) du.de_init(n-1, m-1);//удаление ст таблицы

  if(Line==TRUE)
  li.init(n-1, m-1);//получение первой таблицы

  if(Doubl==TRUE)
  du.init(n-1, m-1);//получение первой таблицы

  //Memo для HTML
  Memo->Lines->Clear();

  if(Line==True)
  decs1="<HTML>";
  if(Doubl==True)
  decs2="<HTML>";

  if(Line==True)
  decs1+="<HEAD><TITLE>Решение задачи линейного программирования</TITLE></HEAD>";
  if(Doubl==True)
  decs2+="<HEAD><TITLE>Решение двойственной задачи линейного программирования</TITLE></HEAD>";

  if(Line==True)
  decs1+="<BODY>";
  if(Doubl==True)
  decs2+="<BODY>";
  
  //вывод в буфер
  if(Line==TRUE)
  decs1+=add_title(li);
  if(Doubl==TRUE)
  decs2+=add_title(du);


  //добавление таблицы
  if(Line==TRUE)
  decs1+=add_table(li, m, n);
  if(Doubl==TRUE)
  decs2+=add_table(du, m, n);
  //таблица добавлена

  
  //вывод 1 табл в stringgrid
  if(Line==True)
  {
   StringGrid1->Cells[0][0]="БП";
   StringGrid1->Cells[1][0]="СЧ";
   StringGrid1->ColCount=1+n+li.get_k()+li.get_r()+li.get_l();
   for (int k = 1; k < n+li.get_k()+li.get_r()+li.get_l(); k++)
   StringGrid1->Cells[k+1][0]= "X"+IntToStr(k);

  StringGrid1->RowCount=m+1;

  for (int i = 0; i < m; i++)
  for (int j = 0; j < n+li.get_k()+li.get_r()+li.get_l()+1; j++)
   {

     if(j==0&&i!=m-1)
     StringGrid1->Cells[j][i+1] = "X" + IntToStr(li.get_base()[i]);


     if(j==0&&i==m-1)
     {
      if(li.get_r()==0)
      StringGrid1->Cells[j][i+1]= "Y";
      else StringGrid1->Cells[j][i+1]= "F'";
     }

     if(j==1)
     {
      StringGrid1->Cells[j][i+1]=IntToStr(li.get_nfreed()[i].get_num());
      if(li.get_nfreed()[i].get_num()!=0&&li.get_nfreed()[i].get_den()!=1)
      StringGrid1->Cells[j][i+1]=StringGrid1->Cells[j][i+1]+"/"+IntToStr(li.get_nfreed()[i].get_den());
     }

     if(j>1&&i!=m-1)
     {
       StringGrid1->Cells[j][i+1]=IntToStr(li.get_nres()[i][j-2].get_num());
       if(li.get_nres()[i][j-2].get_num()!=0&&li.get_nres()[i][j-2].get_den()!=1)
       StringGrid1->Cells[j][i+1]=StringGrid1->Cells[j][i+1]+"/"+IntToStr(li.get_nres()[i][j-2].get_den());
     }

      if(j>1&&i==m-1)
      {
       StringGrid1->Cells[j][i+1]=IntToStr(li.get_nobj()[j-2].get_num());
       if(li.get_nobj()[j-2].get_num()!=0&&li.get_nobj()[j-2].get_den()!=1)
       StringGrid1->Cells[j][i+1]=StringGrid1->Cells[j][i+1]+"/"+IntToStr(li.get_nobj()[j-2].get_den());
      }
  }
 }

 if(Doubl==True) //нач таблица для дв задачи
 {

  StringGrid2->Cells[0][0]="БП";
  StringGrid2->Cells[1][0]="СЧ";
  StringGrid2->ColCount=1+n+du.get_k()+du.get_r()+du.get_l();
  for (int k = 1; k < n+du.get_k()+du.get_r()+du.get_l()+1; k++)
  StringGrid2->Cells[k+1][0]= "Y"+IntToStr(k);

  StringGrid2->RowCount=m+1;

  for (int i = 0; i < m; i++)
  for (int j = 0; j < n+du.get_k()+du.get_r()+du.get_l()+1; j++)
  {

     if(j==0&&i!=m-1)
     StringGrid2->Cells[j][i+1] = "Y" + IntToStr(du.get_base()[i]);


     if(j==0&&i==m-1)
     {
      if(du.get_r()==0)
      StringGrid2->Cells[j][i+1]= "S";
      else StringGrid2->Cells[j][i+1]= "F'";
     }

     if(j==1)
     {
      StringGrid2->Cells[j][i+1]=IntToStr(du.get_nfreed()[i].get_num());
      if(du.get_nfreed()[i].get_num()!=0&&du.get_nfreed()[i].get_den()!=1)
      StringGrid2->Cells[j][i+1]=StringGrid2->Cells[j][i+1]+"/"+IntToStr(du.get_nfreed()[i].get_den());
     }

     if(j>1&&i!=m-1)
     {
       StringGrid2->Cells[j][i+1]=IntToStr(du.get_nres()[i][j-2].get_num());
       if(du.get_nres()[i][j-2].get_num()!=0&&du.get_nres()[i][j-2].get_den()!=1)
       StringGrid2->Cells[j][i+1]=StringGrid2->Cells[j][i+1]+"/"+IntToStr(du.get_nres()[i][j-2].get_den());
     }

      if(j>1&&i==m-1)
      {
       StringGrid2->Cells[j][i+1]=IntToStr(du.get_nobj()[j-2].get_num());
       if(du.get_nobj()[j-2].get_num()!=0&&du.get_nobj()[j-2].get_den()!=1)
       StringGrid2->Cells[j][i+1]=StringGrid2->Cells[j][i+1]+"/"+IntToStr(du.get_nobj()[j-2].get_den());
      }
  }
  }
 fraction x[50];

 if(Line==True)
 if(li.get_dif()==TRUE)  //если для пр задачи ответ получен сразу
 {
   Button4->Visible=true;
   N1->Enabled=true;
   decs1+=add_answer(li);


   fraction answ = li.get_answ();
   if(li.get_not()==0)
   {

    int j=0;
    fraction mm;
    fraction zero;

    for(int i=0;i<m-1+li.get_k();i++) x[i]=zero;

    //получение ответа
    for(int i=0;i<m-1;i++)
    {
     j=li.get_base()[i];
     x[j-1]=li.get_nfreed()[i];;
     j++;
    }
    
    l=0;
    //возвращение переменной
    for(int i=0;i<n-1;i++)
    if(li.get_dis()[i].get_num()==1)
    {
     x[i]=x[i]+x[n+l+li.get_k()-1].men(x[n+l+li.get_k()-1]);
     l++;
    }
    decs1+=" Число итераций: "+IntToStr(0);
   }



   decs1+="</BODY></HTML>";
   Memo->Lines->Clear();
   Memo->Lines->Add(decs1);
   Memo->Lines->SaveToFile("Simplex.htm");

   decs1="";

   
   Panel3->Caption="Ответ: ";

  //если целевая ф-ция оптимальна
  if(li.get_not()==0)
  {
   if(Line==true)
   for(int i=0;i<n-1+li.get_k();i++)
   {
    Panel3->Caption=Panel3->Caption+"X"+IntToStr(i+1)+"="+IntToStr(x[i].get_num());
    if(x[i].get_num()!=0&&x[i].get_den()!=1)
    Panel3->Caption=Panel3->Caption+"/"+IntToStr(x[i].get_den())+"; ";
    else Panel3->Caption=Panel3->Caption+"; ";
   }
  }
 else 
 if(Line==true)
 {
  if(li.get_not()!=0)
  {
   Panel3->Caption="Ответ: Целевая функция не ограничена, причина №"+IntToStr(li.get_not());
   if(li.get_not()==1)
   Panel3->Caption=Panel3->Caption+": В столбце над отриц. элементом нет положительных";
   if(li.get_not()==2)
   Panel3->Caption=Panel3->Caption+": В строке справа от отриц. элемента нет отрицательных";
   if(li.get_not()==3)
   Panel3->Caption=Panel3->Caption+": Искусственная целевая функция осталась отрицательна";
  }
 }


 if(li.get_not()==0)
 {
  if(Line==true)
  {
   if(minmax==1)
   Panel3->Caption = Panel3->Caption+" Y="+IntToStr(-answ.get_num());
   else
   Panel3->Caption = Panel3->Caption+" Y="+IntToStr(answ.get_num());
   if(answ.get_num()!=0&&answ.get_den()!=1)
   Panel3->Caption = Panel3->Caption+"/"+IntToStr(answ.get_den());
   Panel3->Caption = Panel3->Caption+" Число итераций: "+IntToStr(0);
  }

 }

}
else
{
 Panel3->Caption="Исходная таблица";
 Button6->Visible=true;
}

if(Doubl==True)
if(du.get_dif()==TRUE) //если ответ дв задачи получен сразу
{
   Button5->Visible=true;
   N2->Enabled=true;
   decs2+=add_answer(du);


   fraction answ = du.get_answ();
   if(du.get_not()==0)
   {

    int j=0;
    fraction mm;
    fraction zero;

    for(int i=0;i<m-1+du.get_k();i++)
    x[i]=zero;

    //получение ответа
    for(int i=0;i<m-1;i++)
    {
     j=du.get_base()[i];
     x[j-1]=du.get_nfreed()[i];;
     j++;
    }

    //возвращение переменной
    l=0;
    for(int i=0;i<n-1;i++)
    if(du.get_dis()[i].get_num()==1)
    {
     x[i]=x[i]+x[n+l+du.get_k()-1].men(x[n+l+du.get_k()-1]);
     l++;
    } 
    decs2+=" Число итераций: "+IntToStr(0);
   }



  decs2+="</BODY></HTML>";
  Memo->Lines->Clear();
  Memo->Lines->Add(decs2);
  Memo->Lines->SaveToFile("Double_simplex.htm");
  decs2="";
  Panel4->Caption="Ответ: ";

 //если целевая ф-ция оптимальна
 if(du.get_not()==0)
 {


  if(Doubl==true)
  for(int i=0;i<m-1+du.get_k();i++)
  {
   Panel4->Caption=Panel4->Caption+"Y"+IntToStr(i+1)+"="+IntToStr(x[i].get_num());
   if(x[i].get_num()!=0&&x[i].get_den()!=1)
   Panel4->Caption=Panel4->Caption+"/"+IntToStr(x[i].get_den())+"; ";
   else Panel4->Caption=Panel4->Caption+"; ";
  }
 }
 else
 if(Doubl==true)
 {
  if(du.get_not()!=0)
  Panel4->Caption="Ответ: Целевая функция не ограничена, причина №"+IntToStr(du.get_not());
  if(du.get_not()==1)
  Panel4->Caption=Panel4->Caption+": В столбце над отриц. элементом нет положительных";
  if(du.get_not()==2)
  Panel4->Caption=Panel4->Caption+": В строке справа от отриц. элемента нет отрицательных";
  if(du.get_not()==3)
  Panel4->Caption=Panel4->Caption+": Искусственная целевая функция осталась отрицательна";
 }


 if(du.get_not()==0)
 {
  if(Doubl==true)
   {
    if(minmax==0)
    Panel4->Caption  = Panel4->Caption + " S="+IntToStr(-answ.get_num());
    else
    Panel4->Caption  = Panel4->Caption + " S="+IntToStr(answ.get_num());
    if(answ.get_num()!=0&&answ.get_den()!=1)
    Panel4->Caption = Panel4->Caption + "/"+IntToStr(answ.get_den());
    Panel4->Caption = Panel4->Caption+" Число итераций: "+IntToStr(0);
   }
  }

 }
 else
 {
  Panel4->Caption="Исходная таблица";
  Button7->Visible=true;
 }

 if(CheckBox2->Checked==TRUE||(CheckBox1->Checked==TRUE&&CheckBox2->Checked==FALSE))
 TabSheet2->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
  int n_it;
  n_it=li.calculation(n-1, m-1); //получение очеред итерации


  if(li.get_not()==0)   //если не получена ошибка
  {
   if(li.get_isk()==TRUE)
   decs1+="<p>Переходим к исходной целевой функции.<p> ";

   if(li.get_isk()==FALSE)
   if(li.get_doub()==TRUE)
   decs1+="Используем двойственный симплекс-метод. ";
   else decs1+="Используем обычный симплекс-метод. ";

   if(li.get_next()!=0)
   if(li.get_isk()==FALSE)
   {
    decs1+="Вводим в базис X"+IntToStr(li.get_next())+", ";
    decs1+="выводим из базиса X"+IntToStr(li.get_prev())+"<p>";
   }


  //добавление таблицы
  Line=TRUE;
  decs1+=add_table(li, m ,n);
  Line=FALSE;
 }

  
  //новая таблица
  StringGrid1->Cells[0][0]="БП";
  StringGrid1->Cells[1][0]="СЧ";
  StringGrid1->ColCount=1+n+li.get_k()+li.get_r()+li.get_l();
  for (int k = 1; k < n+li.get_k()+li.get_r()+li.get_l(); k++)
  StringGrid1->Cells[k+1][0]= "X"+IntToStr(k);

  StringGrid1->RowCount=m+1;

  for (int i = 0; i < m; i++)
  for (int j = 0; j < n+li.get_k()+li.get_r()+li.get_l()+1; j++)
  {

   if(j==0&&i!=m-1) StringGrid1->Cells[j][i+1] = "X" + IntToStr(li.get_base()[i]);


   if(j==0&&i==m-1)
   {
    if(li.get_r()==0) StringGrid1->Cells[j][i+1]= "Y";
    else StringGrid1->Cells[j][i+1]= "F'";
   }

   if(j==1)
   {
    StringGrid1->Cells[j][i+1]=IntToStr(li.get_nfreed()[i].get_num());
    if(li.get_nfreed()[i].get_num()!=0&&li.get_nfreed()[i].get_den()!=1)
    StringGrid1->Cells[j][i+1]=StringGrid1->Cells[j][i+1]+"/"+IntToStr(li.get_nfreed()[i].get_den());
   }

   if(j>1&&i!=m-1)
   {
    StringGrid1->Cells[j][i+1]=IntToStr(li.get_nres()[i][j-2].get_num());
    if(li.get_nres()[i][j-2].get_num()!=0&&li.get_nres()[i][j-2].get_den()!=1)
    StringGrid1->Cells[j][i+1]=StringGrid1->Cells[j][i+1]+"/"+IntToStr(li.get_nres()[i][j-2].get_den());
   }

   if(j>1&&i==m-1)
   {
    StringGrid1->Cells[j][i+1]=IntToStr(li.get_nobj()[j-2].get_num());
    if(li.get_nobj()[j-2].get_num()!=0&&li.get_nobj()[j-2].get_den()!=1)
    StringGrid1->Cells[j][i+1]=StringGrid1->Cells[j][i+1]+"/"+IntToStr(li.get_nobj()[j-2].get_den());
   }
  }

  fraction x[50];

  if(li.get_dif()==TRUE)  //если для пр задачи получен ответ
  {
   Button4->Visible=true;
   N1->Enabled=true;
   Panel3->Caption="";
   Line=TRUE;
   decs1+=add_answer(li);
   Line=FALSE;

   fraction answ = li.get_answ();
   if(li.get_not()==0)
   {

    int j=0;
    fraction mm;
    fraction zero;

    for(int i=0;i<n-1+li.get_k();i++)
    x[i]=zero;

    //получение ответа
    for(int i=0;i<m-1;i++)
    {
     j=li.get_base()[i];
     x[j-1]=li.get_nfreed()[i];;
     j++;
    }

    int l=0;
    //возвращение переменной
    for(int i=0;i<n-1;i++)
    if(li.get_dis()[i].get_num()==1)
    {
     x[i]=x[i]+x[n+l+li.get_k()-1].men(x[n+l+li.get_k()-1]);
     l++;
    }

    decs1+=" Число итераций: "+IntToStr(n_it);
   }

   decs1+="</BODY></HTML>";
   Memo->Lines->Clear();
   Memo->Lines->Add(decs1);
   Memo->Lines->SaveToFile("Simplex.htm");
   decs1="";



  Panel3->Caption="Ответ: ";


  //если целевая ф-ция оптимальна
  if(li.get_not()==0)
  {
   for(int i=0;i<n-1+li.get_k();i++)
   {
    Panel3->Caption=Panel3->Caption+"X"+IntToStr(i+1)+"="+IntToStr(x[i].get_num());
    if(x[i].get_num()!=0&&x[i].get_den()!=1)
    Panel3->Caption=Panel3->Caption+"/"+IntToStr(x[i].get_den())+"; ";
    else Panel3->Caption=Panel3->Caption+"; ";
   }
  }
  else
  {
    Panel3->Caption="Ответ: Целевая функция не ограничена, причина №"+IntToStr(li.get_not());
    if(li.get_not()==1)
    Panel3->Caption=Panel3->Caption+": В столбце над отриц. элементом нет положительных";
    if(li.get_not()==2)
    Panel3->Caption=Panel3->Caption+": В строке справа от отриц. элемента нет отрицательных";
    if(li.get_not()==3)
    Panel3->Caption=Panel3->Caption+": Искусственная целевая функция осталась отрицательна";
  }


  if(li.get_not()==0)
  {
   {
    if(minmax==1)
    Panel3->Caption = Panel3->Caption+" Y="+IntToStr(-answ.get_num());
    else
    Panel3->Caption = Panel3->Caption+" Y="+IntToStr(answ.get_num());
    if(answ.get_num()!=0&&answ.get_den()!=1)
    Panel3->Caption = Panel3->Caption+"/"+IntToStr(answ.get_den());
    Panel3->Caption = Panel3->Caption+" Число итераций: "+IntToStr(n_it);
   }

  }
  Button6->Visible=false;
 }
 else
 {
  Panel3->Caption="Итерация №"+IntToStr(n_it);
  if(li.get_next()!=0)
  if(li.get_isk()==FALSE)
  Panel3->Caption=Panel3->Caption+" Вводим в базис X"+
  IntToStr(li.get_next())+", выводим из базиса X"+IntToStr(li.get_prev());
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
  int n_it;
  n_it=du.calculation(m-1, n-1); //получение оч итерации для дв задачи


  if(du.get_not()==0)  //если не получена ошибка
  {
   if(du.get_isk()==TRUE)
   decs2+="<p>Переходим к исходной целевой функции.<p> ";

   if(du.get_isk()==FALSE)
   if(du.get_doub()==TRUE)
   decs2+="Используем двойственный симплекс-метод. ";
   else decs2+="Используем обычный симплекс-метод. ";


   if(du.get_next()!=0)
   if(du.get_isk()==FALSE)
   {
    decs2+="Вводим в базис Y"+IntToStr(du.get_next())+", ";
    decs2+="выводим из базиса Y"+IntToStr(du.get_prev())+"<p>";
   }

  //добавление таблицы
  Doubl=TRUE;
  decs2+=add_table(du, n, m);
  Doubl=FALSE;

 }

  StringGrid2->Cells[0][0]="БП";
  StringGrid2->Cells[1][0]="СЧ";
  StringGrid2->ColCount=1+m+du.get_k()+du.get_r()+du.get_l();
  for (int k = 1; k < m+du.get_k()+du.get_r()+du.get_l()+1; k++)
  StringGrid2->Cells[k+1][0]= "Y"+IntToStr(k);

  StringGrid2->RowCount=n+1;

  for (int i = 0; i < n; i++)
  for (int j = 0; j < m+du.get_k()+du.get_r()+du.get_l()+1; j++)
  {

     if(j==0&&i!=n-1)
     StringGrid2->Cells[j][i+1] = "Y" + IntToStr(du.get_base()[i]);


     if(j==0&&i==n-1)
     {
      if(du.get_r()==0)
      StringGrid2->Cells[j][i+1]= "S";
      else StringGrid2->Cells[j][i+1]= "F'";
     }

     if(j==1)
     {
      StringGrid2->Cells[j][i+1]=IntToStr(du.get_nfreed()[i].get_num());
      if(du.get_nfreed()[i].get_num()!=0&&du.get_nfreed()[i].get_den()!=1)
      StringGrid2->Cells[j][i+1]=StringGrid2->Cells[j][i+1]+"/"+IntToStr(du.get_nfreed()[i].get_den());
     }

     if(j>1&&i!=n-1)
     {
       StringGrid2->Cells[j][i+1]=IntToStr(du.get_nres()[i][j-2].get_num());
       if(du.get_nres()[i][j-2].get_num()!=0&&du.get_nres()[i][j-2].get_den()!=1)
       StringGrid2->Cells[j][i+1]=StringGrid2->Cells[j][i+1]+"/"+IntToStr(du.get_nres()[i][j-2].get_den());
     }

      if(j>1&&i==n-1)
      {
       StringGrid2->Cells[j][i+1]=IntToStr(du.get_nobj()[j-2].get_num());
       if(du.get_nobj()[j-2].get_num()!=0&&du.get_nobj()[j-2].get_den()!=1)
       StringGrid2->Cells[j][i+1]=StringGrid2->Cells[j][i+1]+"/"+IntToStr(du.get_nobj()[j-2].get_den());
      }
  }

  fraction x[50];

  if(du.get_dif()==TRUE) //если для дв задачи получен ответ
  {
    Button5->Visible=true;
    N2->Enabled=true;
    Doubl=TRUE;
    decs2+=add_answer(du);
    Doubl=FALSE;

   fraction answ = du.get_answ();
   if(du.get_not()==0)
   {

    int j=0;
    fraction mm;
    fraction zero;

    for(int i=0;i<m-1+du.get_k();i++)
    x[i]=zero;

    //получение ответа
    for(int i=0;i<n-1;i++)
    {
     j=du.get_base()[i];
     x[j-1]=du.get_nfreed()[i];;
     j++;
    }

    int l=0;
    //возвращение переменной
    for(int i=0;i<m-1;i++)
    if(du.get_dis()[i].get_num()==1)
    {
     x[i]=x[i]+x[m+l+du.get_k()-1].men(x[m+l+du.get_k()-1]);
     l++;
    }
    decs2+=" Число итераций: "+IntToStr(n_it);
   }


  decs2+="</BODY></HTML>";
  Memo->Lines->Clear();
  Memo->Lines->Add(decs2);
  Memo->Lines->SaveToFile("Double_simplex.htm");
  decs2="";

 Panel4->Caption="Ответ: ";

 //если целевая ф-ция оптимальна
 if(du.get_not()==0)
 {

  for(int i=0;i<m-1+du.get_k();i++)
  {
   Panel4->Caption=Panel4->Caption+"Y"+IntToStr(i+1)+"="+IntToStr(x[i].get_num());
   if(x[i].get_num()!=0&&x[i].get_den()!=1)
   Panel4->Caption=Panel4->Caption+"/"+IntToStr(x[i].get_den())+"; ";
   else Panel4->Caption=Panel4->Caption+"; ";
  }
 }
 else

 {
  if(du.get_not()!=0)
  Panel4->Caption="Ответ: Целевая функция не ограничена, причина №"+IntToStr(du.get_not());
  if(du.get_not()==1)
  Panel4->Caption=Panel4->Caption+": В столбце над отриц. элементом нет положительных";
  if(du.get_not()==2)
  Panel4->Caption=Panel4->Caption+": В строке справа от отриц. элемента нет отрицательных";
  if(du.get_not()==3)
  Panel4->Caption=Panel4->Caption+": Искусственная целевая функция осталась отрицательна";
 }

                          
 if(du.get_not()==0)
 {
   {
    if(minmax==0)
    Panel4->Caption  = Panel4->Caption + " S="+IntToStr(-answ.get_num());
    else
    Panel4->Caption  = Panel4->Caption + " S="+IntToStr(answ.get_num());
    if(answ.get_num()!=0&&answ.get_den()!=1)
    Panel4->Caption = Panel4->Caption + "/"+IntToStr(answ.get_den());
    Panel4->Caption = Panel4->Caption+" Число итераций: "+IntToStr(n_it);
   }
  }
  Button7->Visible=false;
 }
 else
 {
  Panel4->Caption="Итерация №"+IntToStr(n_it);
  if(du.get_next()!=0)
  if(du.get_isk()==FALSE)
  Panel4->Caption=Panel4->Caption+" Вводим в базис Y"+
  IntToStr(du.get_next())+", выводим из базиса Y"+IntToStr(du.get_prev());
 }
}
//---------------------------------------------------------------------------




