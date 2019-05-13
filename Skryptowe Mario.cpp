#include <iostream>
#define MAX_DL 1001 //max dlugosc napisu na wejsciu
#define MAX_DL_ZM 11 //max dlugosc zmiennej z testow, dla zaoszczedzenia pamieci
enum tryb{liczba,zmienna,liczba_ujemna};
using namespace std;
//******************************STRUKTURY*****************************//
struct stack{int count; int size; char **tab;};
struct list{char *val;list *next;list *prev;};
struct zmienne{char *nazwa; int wartosc; bool init; zmienne* left;zmienne* right;};
//****************************DEKLARACJE**************************//
inline void my_clear(char *a);
inline void my_add_string_b_to_a(char *a, const char *b);
char *my_stoi(int val);
int my_atoi(char *val);
int my_strcmp(char *a, char*b);
int my_strlen(const char *a);
void init_stack(stack *stos);
void reallocate_stack(stack *stos);
void push_stack(stack *stos,char *val);
void pop_stack(stack *stos);
char* top_stack(const stack *stos);
bool empty_stack(const stack *stos);
void list_init(list *head);
void list_push_back(list *lista, char *val);
char wczytaj_wartosc(char znak,list *ONP, stack *operatory, bool *koniec_instrukcji,int tryb);
char wczytaj_operator(char znak,list *ONP, stack *operatory, bool *koniec_instrukcji);
int priorytet(char *operat);
void interpretuj_polecenia(zmienne *tab,list *s_ONP,stack *stos,int ile_zmiennych,int *wykonane,int operacje);
void dodaj_zmienna(zmienne *tab, char *val,int *i,bool *czy_puste_zmienne);
zmienne* wyszukaj_zmienna(zmienne *tab,char *szukana);
zmienne* poznaj_wartosc(zmienne *tab,stack *stos,int *all_zmienne,int *zmienna,bool *liczba);
void wykonaj_przypisz(zmienne *tab,stack *stos,zmienne *wsk,int zmienna2,bool liczba2);
bool czyj_to_nawias(const list *lista);
//***************************MAIN*********************************//
int main()
    {
    list *ONP = new list; list_init(ONP);
    list *s_ONP = ONP;
    stack *operatory = new stack; init_stack(operatory);
    stack *stos_do_ONP = new stack; init_stack(stos_do_ONP);
    zmienne *tab = new zmienne;
    zmienne *wsk=NULL;
    char linia[MAX_DL], zmienna[MAX_DL_ZM] = {},znak='\0';
    int operacje, wykonane = 0, ile_zmiennych=0,w = 0; 
    bool koniec_instrukcji=false,czy_puste_zmienne=true;
 
    cin >> operacje;
    cin.ignore();
    cin.getline(linia, MAX_DL);
 
    for (int i = 0; i < my_strlen(linia); i++)
        {
        if (isalnum(linia[i])) zmienna[w++] = linia[i];
        if (!isalnum(linia[i]) || i == my_strlen(linia) - 1)
            {
            dodaj_zmienna(tab,zmienna,&ile_zmiennych,&czy_puste_zmienne);
            my_clear(zmienna);
            w = 0;
            }
        }   
 
    while (1)
        {
        if(isdigit(znak))
            znak=wczytaj_wartosc(znak,ONP,operatory,&koniec_instrukcji,::liczba);
        else
        if(isalpha(znak))
            znak=wczytaj_wartosc(znak,ONP,operatory,&koniec_instrukcji,::zmienna);
        else
        if(isprint(znak)&&!isspace(znak))
            znak=wczytaj_operator(znak,ONP,operatory,&koniec_instrukcji);
        else
            {
            if(~scanf("%c",&znak));
            else break;
            }
        while(ONP->next!=NULL)ONP = ONP->next; //przesuniecie wskaŸnika
        }
 
    while(!empty_stack(operatory))
        {
        list_push_back(ONP,top_stack(operatory));
        pop_stack(operatory);
        ONP=ONP->next;
        }
 
    delete operatory;
    interpretuj_polecenia(tab,s_ONP->next,stos_do_ONP,ile_zmiennych,&wykonane,operacje);    
 
    printf("%d\n", wykonane);
    for (int i = 0; i < my_strlen(linia); i++)
        {
        if (isalnum(linia[i])) zmienna[w++] = linia[i];
        if (!isalnum(linia[i]) || i == my_strlen(linia) - 1)
            {
            wsk=wyszukaj_zmienna(tab,zmienna);
            printf("%s ", wsk->nazwa);
            if (wsk->init)
                printf("%d\n", wsk->wartosc);
            else
                printf("Nul\n");
            my_clear(zmienna);
            w = 0;
            }
        }   
 
    delete ONP; 
    delete stos_do_ONP; 
    delete s_ONP; 
    delete tab;
    }
 
//***********************FUNKCJE_NAPISOW****************************//
void my_clear(char *a)
    {
    int i = 0;
    while (a[i] != '\0')
        {
        a[i] = '\0';
        i++;
        }
    }
 
void my_add_string_b_to_a(char *a, const char *b)
    {
    int i = 0, k = 0;
    while (a[i] != '\0')i++;
    while (b[k] != '\0')
        {
        a[i] = b[k];
        i++;
        k++;
        }
    a[i] = '\0';
    }
 
char *my_stoi(int val)
    {
    bool ujemna=false;
    if(val<0){ujemna=true;val*=-1;}
    char *liczba=new char[25]; //int nie jest dluzszy ni¿ 25 znakow
    my_clear(liczba);
    int w=0;
    if(val==0)liczba[w++]='0';
    while(val)
        {
        liczba[w]=val%10+'0';
        val/=10;
        w++;
        }
    if(ujemna)liczba[w++]='-';
 
    for(int i=0;i<my_strlen(liczba)/2;i++)
        {
        char pom=liczba[i];
        liczba[i]=liczba[my_strlen(liczba)-1-i];
        liczba[my_strlen(liczba)-1-i]=pom;
        }
    liczba[w]='\0';
    return liczba;
    }
 
int my_atoi(char *val)
    {
    bool ujemna=false;
    if(val[0]=='-')ujemna=true;
    int liczba=0,i=0;
    if(ujemna)i=1;
    for(;i<my_strlen(val);i++)
        liczba=liczba*10+int(val[i]-'0');
    if(ujemna)liczba*=-1;
    return liczba;
    }
 
int my_strcmp(char *a, char*b)
    {
    while (*a && (*a == *b))
        a++, b++;
    return *(char*)a - *(char*)b;
    }
 
int my_strlen(const char *a)
    {
    int dl = 0;
    while (a[dl] != '\0')dl++;
    return dl;
    }   
 
//*******************************STOS*******************************// 
void init_stack(stack *stos)
    {
    stos->count=0;
    stos->size=10;
    stos->tab=new char*[10];
    }
 
 void reallocate_stack(stack *stos)
    {
    char **pom=new char*[stos->size*2];
    for(int i=0;i<stos->count;i++)
        {
        pom[i]=new char[my_strlen(stos->tab[i])+1];
        my_clear(pom[i]);
        my_add_string_b_to_a(pom[i],stos->tab[i]);
        }
 
    delete[] stos->tab;
    stos->tab=pom;
    stos->size*=2;
    }
 
void push_stack(stack *stos,char *val)
    {
    if(stos->count==stos->size)
        reallocate_stack(stos);
 
    stos->tab[stos->count]=new char[my_strlen(val)+1];
    my_clear(stos->tab[stos->count]);
    my_add_string_b_to_a(stos->tab[stos->count],val);
    stos->count++;
    }
 
void pop_stack(stack *stos)
    {
    if(stos->count!=0)
        {
        stos->count--;
        }
    }
 
char* top_stack(const stack *stos)
    {
    if(stos->count!=0)
        return stos->tab[stos->count-1];
    return "EMPTY";
    }
 
bool empty_stack(const stack *stos)
    {
    if(stos->count>0)return false;
    return true;
    }
 
//*******************************LISTA*******************************//     
void list_init(list *head)
    {
    head->next = NULL;
    head->prev = NULL;
    }
 
void list_push_back(list *lista, char *val)
    {
    list *x = new list;
    x->val=new char[my_strlen(val)+1];
    my_clear(x->val);
    my_add_string_b_to_a(x->val,val);
    x->next = lista->next;
    lista->next = x;
    x->prev = lista;
    }
 
//*************************INNE_FUNCKJE********************************//
char wczytaj_wartosc(char znak, list *ONP, stack *operatory, bool *koniec_instrukcji,int tryb)
    {
    int w=0;
    char liczba[MAX_DL_ZM]={}; //dluzszych zmiennych w testach niema
    if(tryb==::liczba||tryb==::liczba_ujemna)
        {
        if(tryb==::liczba_ujemna)liczba[w++]='-';
        while(isdigit(znak))
            {
            liczba[w++]=znak;
            znak='\0';
            scanf("%c",&znak);
            }
        }
    else
    if(tryb==::zmienna)
        {
        while(isalnum(znak))
            {
            liczba[w++]=znak;
            znak='\0';
            scanf("%c",&znak);
            }
        }
 
    if(*koniec_instrukcji)
        {
        while(!empty_stack(operatory))
            {
            list_push_back(ONP,top_stack(operatory));
            pop_stack(operatory);
            ONP=ONP->next;
            }
        }
    *koniec_instrukcji=true;
    list_push_back(ONP,liczba);
    ONP = ONP->next;
    return znak;
    }
 
char wczytaj_operator(char znak, list *ONP, stack *operatory, bool *koniec_instrukcji)
    {
    char nastepny;
    char operat[2]={};
    operat[2]='\0';
    static bool unarny;
    if (cin >> nastepny);
    else nastepny = '\0';
    operat[0] = znak;
    if (nastepny == '=')operat[1] = nastepny;
 
    if(znak=='?'||znak=='@')
        {
        *koniec_instrukcji=true;
        while(!empty_stack(operatory))
            {
            list_push_back(ONP,top_stack(operatory));
            pop_stack(operatory);
            ONP=ONP->next;
            }   
        list_push_back(ONP,"$"); //oznaczenie poczatku warunku, zeby wiedzialo gdzie wracac przy petlach
        ONP=ONP->next;
        }
 
    if(unarny||(operat[0]=='-'&&!*koniec_instrukcji))
        {
        unarny=false;
        if(isdigit(nastepny))
            return wczytaj_wartosc(nastepny,ONP,operatory,koniec_instrukcji,liczba_ujemna);
        else 
            operat[1] = 'u';
        }
 
    if(priorytet(operat)==-1&&*koniec_instrukcji) //jak otwarcie nawiasu a wczesniej byla val bez oper
        {
        while(!empty_stack(operatory))
            {
            list_push_back(ONP,top_stack(operatory));
            pop_stack(operatory);
            ONP=ONP->next;
            }
        }
 
    if (empty_stack(operatory)||priorytet(operat)==-1||priorytet(operat)==5)
        {
        push_stack(operatory, operat); //gdy pusty || nawias || k-operatory !!! ---
        if(operat[0]=='{')
            {
            list_push_back(ONP,top_stack(operatory));
            pop_stack(operatory);
            ONP = ONP->next;
            }
        }
    else
    if(priorytet(operat)==-2)
        {
        while (priorytet(top_stack(operatory))!=-1&&!empty_stack(operatory))
            {
            list_push_back(ONP,top_stack(operatory));
            pop_stack(operatory);
            ONP = ONP->next;
            }
        if(operat[0]=='}')
            {
            list_push_back(ONP,operat);
            ONP = ONP->next;
            }
        pop_stack(operatory);
        }
    else
        {
        while(priorytet(top_stack(operatory))>=priorytet(operat)&&!empty_stack(operatory))
            {
            //= jest lewostronny wiec & i | nie zdejmuja przypisania
            if(priorytet(top_stack(operatory))==priorytet(operat)&&top_stack(operatory)[0]=='=')break;
            list_push_back(ONP, top_stack(operatory));
            pop_stack(operatory);
            ONP = ONP->next;
            }
        push_stack(operatory, operat);
        }
 
    if(priorytet(operat)==-2)*koniec_instrukcji=true;
    else *koniec_instrukcji=false;
 
    if(operat[0]!=')'&&nastepny=='-')unarny=true;
    if(nastepny!='=')return nastepny;
    return ' ';
    }   
 
int priorytet(char *operat)
    {
    if(operat[0]=='=')
        {
        if(operat[1]=='=')return 1;
        else return 0;
        }
    if(operat[0]=='|'||operat[0]=='&')return 0;
    if(operat[0]=='!')
        {
        if(operat[1]=='=')return 1;
        else return 5;
        }
    if(operat[0]=='<'||operat[0]=='>')return 2;
    if(operat[0]=='+')return 3;
    if(operat[0]=='-')
        {
        if(operat[1]=='u')return 5;
        else return 3;
        }
    if(operat[0]=='/'||operat[0]=='*'||operat[0]=='%')return 4;
    if(operat[0]=='('||operat[0]=='{')return -1;
    if(operat[0]==')'||operat[0]=='}')return -2;
    return -99;
    }
 
void dodaj_zmienna(zmienne *tab, char *val,int *i,bool *czy_puste)
    {
    *i=*i+1;
    if(*czy_puste)
        {
        tab->nazwa=new char[my_strlen(val)+1];
        my_clear(tab->nazwa);
        my_add_string_b_to_a(tab->nazwa,val);
        tab->left=NULL;
        tab->right=NULL;
        tab->init=false;
        tab->wartosc=-9090909; //dla ulatwienia podgladu
        *czy_puste=false;
        }
    else
        {
        zmienne *nowy=new zmienne;
        nowy->nazwa=new char[my_strlen(val)+1];
        my_clear(nowy->nazwa);
        my_add_string_b_to_a(nowy->nazwa,val);
        nowy->left=NULL;
        nowy->right=NULL;
        nowy->init=false;
        nowy->wartosc=-9090909; //dla ulatwienia podgladu
        while(tab!=NULL)
            {
            if(my_strcmp(tab->nazwa,val)>0)
                {
                if(tab->left!=NULL)
                    tab=tab->left;
                else
                    {tab->left=nowy;break;}
                }
            else
                {
                if(tab->right!=NULL)
                    tab=tab->right;
                else
                    {tab->right=nowy;break;}
                }
            }
        }
    }   
 
zmienne* wyszukaj_zmienna(zmienne *tab,char *szukana)
    {
    while(tab!=NULL)
            {
            if(my_strcmp(tab->nazwa,szukana)>0)
                {
                if(tab->left!=NULL)
                    tab=tab->left;
                else
                    return NULL;
                }
            else
            if(my_strcmp(tab->nazwa,szukana)<0)
                {
                if(tab->right!=NULL)
                    tab=tab->right;
                else
                    return NULL;
                }
            else
                return tab;
            }
        return NULL;
    }
 
zmienne* poznaj_wartosc(zmienne *tab,stack *stos,int *all_zmienne,int *zmienna,bool *liczba)
    {
    *liczba=true;
    zmienne *wsk=NULL;
    if(isdigit(top_stack(stos)[0])||top_stack(stos)[0]=='-') //jak liczba to jej wartosc
        {
        *zmienna=my_atoi(top_stack(stos));
        pop_stack(stos);
        }
    else 
        { 
        wsk=wyszukaj_zmienna(tab,top_stack(stos)); 
        bool test=false;
        if(wsk==NULL) //jak nie ma takiej zmiennej to ja dodaj
            {
            dodaj_zmienna(tab,top_stack(stos),all_zmienne,&test);
            wsk=wyszukaj_zmienna(tab,top_stack(stos)); 
            }
        if(wsk->init==true)//jak zmienna ma wartosc to jej wartosc
            {
            *zmienna=wsk->wartosc;
            pop_stack(stos);
            }
        else
            { //a jak zmienna to nul to zwraca wskaznik na ta zmienna
            *liczba=false;
            pop_stack(stos);
            }
        }
    return wsk;
    }
 
void wykonaj_przypisz(zmienne *tab,stack *stos,zmienne *wsk,int zmienna2,bool liczba2)
    {
    if(!liczba2) //jak przypisywana to NULL
        {
        wsk->wartosc=0;
        wsk->init=false;
        }
    else //jak przypisywana to cokolwiek innego to w zmienna2 jest od razu wartosc liczbowa
        {
        wsk->wartosc=zmienna2;
        wsk->init=true;
        }
    push_stack(stos,wsk->nazwa);
    }
 
void interpretuj_polecenia(zmienne *tab,list *s_ONP,stack *stos,int ile_zmiennych,int *wykonane,int operacje)
    {
    bool liczba1,liczba2;
    int all_zmienne=ile_zmiennych;
    int zmienna1,zmienna2,zaglebienie=0,petla=0;
    zmienne *wsk1=NULL,*wsk2=NULL;
    while(s_ONP!=NULL&&*wykonane<operacje)
        {//jesli zmienna, liczba lub ujemna liczba to wrzucamy na stos
        if(isalnum(s_ONP->val[0])||(s_ONP->val[0]=='-'&&isdigit(s_ONP->val[1])))push_stack(stos,s_ONP->val);
        else 
            {
            *wykonane=*wykonane+1;
            if(my_strcmp(s_ONP->val,"=")==0)
                {
                wsk2=poznaj_wartosc(tab,stos,&all_zmienne,&zmienna2,&liczba2);
                //zakladam,ze nikt do liczby nie bedzie nic przepisywal.
                wsk1=poznaj_wartosc(tab,stos,&all_zmienne,&zmienna1,&liczba1);
                wykonaj_przypisz(tab,stos,wsk1,zmienna2,liczba2);
                }
            else
            if(priorytet(s_ONP->val)==3||priorytet(s_ONP->val)==4) //arytmetyczne
                {
                wsk2=poznaj_wartosc(tab,stos,&all_zmienne,&zmienna2,&liczba2);
                wsk1=poznaj_wartosc(tab,stos,&all_zmienne,&zmienna1,&liczba1);
                if(liczba1&&liczba2)
                    {
                    if(my_strcmp(s_ONP->val,"+")==0)push_stack(stos,my_stoi(zmienna1+zmienna2));
                    else if(my_strcmp(s_ONP->val,"-")==0)push_stack(stos,my_stoi(zmienna1-zmienna2));
                    else if(my_strcmp(s_ONP->val,"*")==0)push_stack(stos,my_stoi(zmienna1*zmienna2));
                    else if(my_strcmp(s_ONP->val,"/")==0)
                            { //dzielenie przez zero daje Nul
                            if(zmienna2!=0)push_stack(stos,my_stoi(zmienna1/zmienna2));
                            else push_stack(stos,"Nul");
                            }
                    else if(my_strcmp(s_ONP->val,"%")==0)
                            { //modulowanie prez zero daje Nul
                            if(zmienna2!=0)push_stack(stos,my_stoi(zmienna1%zmienna2));
                            else push_stack(stos,"Nul");
                            }
                    }
                else
                    {
                    if(!liczba1) push_stack(stos,wsk1->nazwa);
                    else push_stack(stos,wsk2->nazwa);
                    }
                }
            else
            if(priorytet(s_ONP->val)==1||priorytet(s_ONP->val)==2) //logiczne
                {
                wsk2=poznaj_wartosc(tab,stos,&all_zmienne,&zmienna2,&liczba2);
                wsk1=poznaj_wartosc(tab,stos,&all_zmienne,&zmienna1,&liczba1);
                if(liczba1&&liczba2)
                    {
                    if(my_strcmp(s_ONP->val,"==")==0&&zmienna1==zmienna2)push_stack(stos,my_stoi(0));
                    else if(my_strcmp(s_ONP->val,"!=")==0&&zmienna1!=zmienna2)push_stack(stos,my_stoi(0));
                    else if(my_strcmp(s_ONP->val,"<")==0&&zmienna1<zmienna2)push_stack(stos,my_stoi(0));
                    else if(my_strcmp(s_ONP->val,">")==0&&zmienna1>zmienna2)push_stack(stos,my_stoi(0));
                    else if(my_strcmp(s_ONP->val,"<=")==0&&zmienna1<=zmienna2)push_stack(stos,my_stoi(0));
                    else if(my_strcmp(s_ONP->val,">=")==0&&zmienna1>=zmienna2)push_stack(stos,my_stoi(0));
                    else push_stack(stos,"Nul");
                    }
                else
                    {
                    if(!liczba1) push_stack(stos,wsk1->nazwa);
                    else push_stack(stos,wsk2->nazwa);
                    }
                }
            else
            if(my_strcmp(s_ONP->val,"|")==0||my_strcmp(s_ONP->val,"&")==0) //logiczne_SP
                {
                wsk2=poznaj_wartosc(tab,stos,&all_zmienne,&zmienna2,&liczba2);
                wsk1=poznaj_wartosc(tab,stos,&all_zmienne,&zmienna1,&liczba1);
                if(my_strcmp(s_ONP->val,"|")==0&&(liczba1||liczba2))push_stack(stos,my_stoi(0));
                else if(my_strcmp(s_ONP->val,"&")==0&&(liczba1&&liczba2))push_stack(stos,my_stoi(0));
                else push_stack(stos,"Nul");
                }
            else
            if(priorytet(s_ONP->val)==5) //jednoargumentowe
                {
                wsk1=poznaj_wartosc(tab,stos,&all_zmienne,&zmienna1,&liczba1);
                if(my_strcmp(s_ONP->val,"!")==0&&!liczba1)push_stack(stos,my_stoi(0));
                else if(my_strcmp(s_ONP->val,"-u")==0&&liczba1)push_stack(stos,my_stoi(zmienna1*-1));
                else push_stack(stos,"Nul");
                }
            else
            if(my_strcmp(s_ONP->val,"?")==0||my_strcmp(s_ONP->val,"@")==0)
                {
                wsk1=poznaj_wartosc(tab,stos,&all_zmienne,&zmienna1,&liczba1);
                if(!liczba1)
                    {
                    int nawiasy=1;
                    while(my_strcmp(s_ONP->val,"}")!=0||nawiasy!=0)
                        {
                        if(s_ONP->val[0]=='}')nawiasy++;
                        if(s_ONP->val[0]=='{')nawiasy--;
                        s_ONP=s_ONP->next;
                        }
                    }
                else        
                    if(my_strcmp(s_ONP->val,"@")==0)petla++;
                }
            else
                {
                *wykonane=*wykonane-1;
                if(my_strcmp(s_ONP->val,"}")==0&&petla>0&&czyj_to_nawias(s_ONP->prev))
                    {
                    petla--;
                    s_ONP=s_ONP->prev;
                    int nawiasy=1;
                    while(s_ONP->val[0]!='$'||nawiasy!=0)
                        {
                        if(s_ONP->val[0]=='}')nawiasy++;
                        if(s_ONP->val[0]=='{')nawiasy--;
                        s_ONP=s_ONP->prev;
                        }
                    }
                }
            }
        s_ONP=s_ONP->next;
        }
    }   
 
    bool czyj_to_nawias(const list *wsk)
        {
        int nawiasy=1;
        while((wsk->val[0]!='@'&&wsk->val[0]!='?')||nawiasy!=0)
            {
            if(wsk->val[0]=='}')nawiasy++;
            if(wsk->val[0]=='{')nawiasy--;
            wsk=wsk->prev;
            }
        if(wsk->val[0]=='@')return true;
        return false;
        }
