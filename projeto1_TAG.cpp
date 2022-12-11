/*
  Projeto 1 da discplina CIC0199 - Teoria e Aplicacao de Grafos
  Nome: Maria Eduarda Carvalho Santos
  Matricula: 190092556
  UnB, Darcy Ribeiro - 2022.2
*/

#include<bits/stdc++.h>

using namespace std;

vector<vector<string>> grafo;
vector<vector<string>> cliques;

void print(vector<string> v)
{  
  for(int i=0; i<v.size(); i++)
  {
    cout << v[i] << " ";
  }

  cout << endl;
}

void print()
{
    for(int i=0; i<grafo.size(); i++)
    {
        //cout << grafo[i][0] << ": \n";

        for(int j=0; j<grafo[i].size(); j++)
        {
            cout << grafo[i][j] << " -> ";
        }

        cout << endl;
    }
}

double coeficienteDeAglomeracao()
{
  double ans = 0;

  for(int v=1; v<=grafo.size(); v++)    //talvez tenha um erro nesse grafo.size() aqui
  {
    double coeficienteLocal = 0;

    for(auto vizinho : grafo[v])
    {
      for(auto outroVizinho : grafo[find(grafo.begin(), grafo.end(), vizinho)-grafo.begin()+1])
      {
        for(auto u : grafo[v])
        {
          if(outroVizinho == u)
          {
            coeficienteLocal++;
          }
        }
      }
    }

    if(grafo[v].size()>1)
    {
      ans += (double)coeficienteLocal/(grafo[v].size()*(grafo[v].size()-1));
    }
  }

  return (double)ans/grafo.size();
}

vector<string> uniao(vector<string> A, vector<string> B)
{
  vector<string> U(A.size()+B.size());

  auto it = set_union(A.begin(), A.end(), B.begin(), B.end(), U.begin());

  U.resize(it- U.begin());

  return U;
}

vector<string> interseccao(vector<string> A, vector<string> B)
{
  vector<string> I(A.size()+B.size());

  auto it = set_union(A.begin(), A.end(), B.begin(), B.end(), I.begin());

  I.resize(it- I.begin());

  return I;
}

void bronKerboschSemPivot(vector<string> R, vector<string> P, vector<string> X)
{
    vector<string> PCopy(P);

    if(P.empty() && X.empty())
    {
      cliques.push_back(R);
    }

    for(auto v: P)
    {
      bronKerboschSemPivot(uniao(R, {v}), interseccao(PCopy, grafo[find(grafo.begin(), grafo.end(), v)-grafo.begin()+1]), interseccao(X, grafo[find(grafo.begin(), grafo.end(), v)-grafo.begin()+1]));

      PCopy.erase(find(PCopy.begin(), PCopy.end(), v));
      X = uniao(X, {v});
    }
}

int main()
{
    //freopen("entrada.txt", "r", stdin);
    
    string s;
    vector<string> aux;                                                                       //vector auxiliar na leitura da entrada
    vector<string> P; vector<string> R; vector<string> X;

    while(cin >> s)
    {
        string edge = s;                                                                      //variavel auxiliar para insercao de dados limpos no grafo
        edge.pop_back();                                                                      //limpeza do dado
        aux.push_back(edge);                                                                  //insercao do dado no vector auxiliar

        if(s.find(";")!=-1)                              
        {
            P.push_back(aux[0]);
            grafo.push_back(aux);                                                            //insercao das grafoacencias na lista dinamica que uso como grafo
            aux.clear();                                                                     //limpando o vetor auxiliar para usar na proxima lista de grafoacencia
        }
    }

    bronKerboschSemPivot(R, P, X);                                                           //implementacao do algortimo de Bron-Kerbosch

    int maior = -1;
    for(int i=0; i<cliques.size(); i++)                                                     //apresentando os cliques maximais acima de 3 vertices
    {
      if(cliques[i].size()>3)
      {
        cout << "o clique maximal eh\n";
        print(cliques[i]);

        if(cliques[i].size()>maior)
        {
          maior = i;                                                                       //encontrando o clique maximo
        }
      }
    }

    vector<string> maximo = cliques[maior];

    cout << "o clique maximo eh\n"; print(maximo);                                                                       //apresentando o clique maximo

    printf("O coeficiente medio de aglomeracao eh %.2lf\n", coeficienteDeAglomeracao());         //apresentando o coeficiente medio de aglomeracao

    return 0;
}