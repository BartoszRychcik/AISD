#include <iostream>
using namespace std;
//wczytywanie FREAD
size_t read_count;
size_t read_pos;
static char buffer[10001];
bool eof;
inline int getCharFromStream()
        {
    if(read_pos>=read_count)
        {
        read_count=fread(buffer,1,1001,stdin);
        if(!read_count)
                {
            eof = true;
            return -1;
                }
        read_pos=0;
    }
    char ch=buffer[read_pos++];
    return ch;
        }
inline int read()
        {
   int value=0;
   bool foundValue=false;
   while(true)
        {
      char ch=getCharFromStream();
      if (ch<'0'||ch>'9')
        {
         if (foundValue)
                {
             read_pos--;
             break;
                }
         continue;
        }
      value=10*value+(ch-'0');
      foundValue = true;
   }
   return value;
} 
//-----------------------------
struct graf
    {
    bool zuzycie; 
    bool odwiedzony;
    int it;
    int *polaczenia;
    int dopasowanie;
    int odleglosc;
    }; 
 
inline void bfs(graf *G,int ludzie,int *Q)
    {
    int it=0;
    for(int i=0;i<ludzie;i++)
        G[i].odleglosc=-1;  
    for(int i=0;i<ludzie;i++)
        {
        if(!G[i].zuzycie)
            {
            Q[it++]=i;
            G[i].odleglosc=0;
            }
        }
    for(int i=0;i<it;i++)
        {
        int V=Q[i];
        for(int j=G[V].it-1;j>=0;j--)
            {
            int E=G[G[V].polaczenia[j]].dopasowanie;
            if(E>=0&&G[E].odleglosc<0)
                {
                G[E].odleglosc=G[V].odleglosc+1;
                Q[it++]=E;
                }
            }
        }
    }
 
inline bool dfs(graf *G,int V) 
    {
    G[V].odwiedzony = true;
    for(int i=G[V].it-1;i>=0;i--)
        {
        int E=G[V].polaczenia[i];
        int S=G[E].dopasowanie;
        if(S<0||!G[S].odwiedzony&&G[S].odleglosc==G[V].odleglosc+1&&dfs(G,S))
            {
            G[E].dopasowanie=V;
            G[V].zuzycie=true;
            return true;
            }
        }
    return false;
    }
 
int main() 
    {
    register int ludzie,rowery,a,b;
    ludzie=read();
    rowery=read();
    
    int *Q=new int[ludzie];
    graf *G=new graf[ludzie+rowery+1];
 
    for(int i=0;i<ludzie;i++)
        {
        a=read();
        G[i].polaczenia=new int[a];
        G[i].it=0;
        G[i].zuzycie=false;
        for(int j=0;j<a;j++)
            {
            b=read();
            G[i].polaczenia[G[i].it++]=b+ludzie-1;
            }
        }
        
    for(int i=0;i<ludzie+rowery+1;i++)
        G[i].dopasowanie=-1;
    
    for(int wynik=0,sciezka_max=0;;wynik+=sciezka_max,sciezka_max=0) 
        {
        bfs(G,ludzie,Q);
        for(int i=0;i<ludzie;i++)G[i].odwiedzony=false;
        for(int i=0;i<ludzie;i++)if(!G[i].zuzycie && dfs(G,i))sciezka_max++;
        if(!sciezka_max){cout<<wynik;break;};
        }
        
    delete [] G;
    delete [] Q;
    }
