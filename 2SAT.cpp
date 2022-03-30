struct sat2{
  // vertices impares = !
  // x e -x (-x = anti(x))
 
  vector<int> grafo[MAXN],inv[MAXN],fim;
 
  int q;
  int comp[MAXN];
  bool vis[MAXN];
 
  sat2(){
    for(int i = 0;i < MAXN;i++) comp[i] = i;
    q = 0;
  }
    
  int anti(int x){
    return x ^ 1;
  }
 
  int node(int x){
    if(x >= 0) return 2 * (x - 1);
    else return anti(node(abs(x)));
  }
  
  // adiciona implicação ao grafo (a implica b)
  void implica(int a,int b){
    a = node(a) , b = node(b);
    grafo[a].push_back(b);
    inv[b].push_back(a);
  }
  
  // calcula as componentes fortemente conexas
  void calc_comp(int n){
    for(int i = 1;i <= 2*n;i++)
      if(vis[i] == false)
        dfs1(i);
 
    memset(vis,false,sizeof vis);
    
    for(int i = (int) fim.size() - 1;i >= 0;i--)
      if(vis[fim[i]] == false)
        dfs2(fim[i]);      
  }
 
  int find(int x){
    if(comp[x] == x) return x;
    return comp[x] = find(comp[x]);
  }
 
  void join(int a,int b){
    comp[find(a)] = find(b);
  }
  
  // retorna a componente na qual x pertence
  int cmp(int x){
    x = node(x);
    return find(comp[x]);
  }
 
  void dfs1(int x){
    vis[x] = true;
    for(auto i : grafo[x])
      if(vis[i] == false)
        dfs1(i);
    fim.push_back(x);
  }
 
  void dfs2(int x){
    vis[x] = true;
    for(auto i : inv[x])
      if(vis[i] == false){
        join(x,i);
        dfs2(i);
      }
  }
 
  // limpa toda a struct
  void limpa(int n){
    for(int i = 0;i <= 10*n;i++){
      comp[i] = i;
      vis[i] = false;
      grafo[i].clear();
      inv[i].clear();
    }
 
    fim.clear();
    q = 0;
  }
};

sat2 s;
