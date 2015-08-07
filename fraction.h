#ifndef fractionH
#define fractionH

class fraction//класс дробь

{

public:
long num; //числитель
long den; //знаменатель


fraction(int=0, int=1);
fraction operator+(fraction const & a);
fraction operator-(fraction const & a);
fraction operator*(fraction const & a);
fraction operator/(fraction const & a);

fraction men(fraction const & a);
bool more(fraction const & a, fraction const & b);
bool less(fraction const & a, fraction const & b);
bool more_or_eq(fraction const & a, fraction const & b);
bool less_or_eq(fraction const & a, fraction const & b);




fraction & operator = (fraction const & a);
int nod (int, int) const;
int get_num();
int get_den();
void set_num(int);
void set_den(int);
};


//-------------------------------------------------------------------------------
#endif 