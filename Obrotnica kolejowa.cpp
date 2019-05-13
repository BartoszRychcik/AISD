#include <iostream>
#define Ob_Size 12
#define SIZE liczba_torow+1 
using namespace std;
 
struct stack
        {
        int count;
        int *tab;
        };
 
struct list
        {
        int val;
        list *next;
        };
 
int stack_size(stack *stos)
        {
        return stos->count;
        }
 
void stack_init(stack *stos)
        {
        stos->count = 0;
        stos->tab = new int[Ob_Size];
        }
 
void list_init(list *head)
        {
        head->next = NULL;
        head->val = -1;
        }
 
void stack_push_back(stack *stos, int val)
        {
        stos->tab[stos->count] = val;
        stos->count++;
        }
 
int stack_pop_back(stack *stos)
        {
        if (stos->count != 0)
                {
                stos->count--;
                return stos->tab[stos->count];
                }
        return -1;
        }
 
void list_push_back(list *lista, int val)
        {
        list *x = new list;
 
        x->val = val;
        x->next = lista->next;
        lista->next = x;
        }
 
void list_pop_back(list *lista)
        {
        if (lista->next != NULL)
                {
                list *x = lista->next->next;
                delete lista->next;
                lista->next = x;
                }
        }
 
void print_list(const list *lista)
        {
        lista = lista->next;
        while (lista != NULL)
                {
                printf("%d ", lista->val);
                lista = lista->next;
                }
        printf("\n");
        }
 
int back_list(const list *lista)
        {
        lista = lista->next;
        if (lista != NULL)
                return lista->val;
        return -1;
        }
 
void przestaw(int  *obrotnica,int val,int liczba_torow)
        {
        if(val>0)
                {
                *obrotnica = (*obrotnica + val) % liczba_torow;
                }
        else
        if (val < 0)
                {
                *obrotnica += val;
                if (*obrotnica < 0)
                        *obrotnica = (0+(liczba_torow-abs(*obrotnica) % liczba_torow))%liczba_torow;
                }
        }
 
bool jest_elem_listy(list *lista)
        {       
        if (lista->next != NULL)
                return true;
        return false;
        }
 
int main()
        {
        ios_base::sync_with_stdio(0);
        int liczba_torow, id_wag, obrotnica = 0, ile_wag, zdarzenia,val;
        stack *stos = new stack;
        stack_init(stos);
        char znak;
        cin >> liczba_torow;
        int *list_size = new int[liczba_torow+1];
        list **tory=new list *[liczba_torow+1];
        list **s_list=new list *[liczba_torow+1];
        for (int i = 0; i < liczba_torow; i++)
                {
                tory[i] = new list;
                s_list[i] = tory[i];
                list_init(tory[i]);
                cin >> ile_wag;
                list_size[i] = ile_wag;
                for (int j = 0; j < ile_wag; j++)
                        {       
                        cin >> id_wag;
                        list_push_back(tory[i],id_wag);
                        tory[i] = tory[i]->next;
                        }
                tory[i] = s_list[i];
                }
        cin >> zdarzenia;
        while (zdarzenia--)
                {
                cin >> znak >> val;
                switch (znak)
                        {
                        case'z':
                                {
                                przestaw(&obrotnica, val,liczba_torow);
                                break;
                                }
                        case'o':
                                {
                                przestaw(&obrotnica, -val,liczba_torow);
                                break;
                                }
                        case'w':
                                {       
                                while (val > 0 && jest_elem_listy(s_list[obrotnica]) && stack_size(stos) < 10)
                                        {
                                        int elem = back_list(s_list[obrotnica]);
                                        list_size[obrotnica]--;
                                        stack_push_back(stos,elem);
                                        list_pop_back(s_list[obrotnica]);
                                        val--;
                                        }
                                break;
                                }
                        case'j':
                                {
                                while (val > 0 && stack_size(stos) > 0)
                                        {
                                        int elem = stack_pop_back(stos);
                                        list_push_back(s_list[obrotnica], elem);
                                        list_size[obrotnica]++;
                                        val--;
                                        }
                                break;
                                }
                        }
                }
        cout << stack_size(stos)<<" ";
        while (stack_size(stos))
                cout << stack_pop_back(stos) << " ";
        cout << endl;
 
        for (int i = 0; i < liczba_torow; i++)
                {
                cout << list_size[obrotnica]<<" ";
                print_list(tory[obrotnica]);
                obrotnica = (obrotnica + 1) % liczba_torow;
                }
        delete stos;
        delete[] list_size;
        for (int i = 0; i < liczba_torow; i++)
                {
                delete tory[i];
                }
        delete[] tory;
        delete[] s_list;
        return 0;
        }
