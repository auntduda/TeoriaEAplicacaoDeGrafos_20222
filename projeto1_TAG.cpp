/*
  Projeto 1 da discplina CIC0199 - Teoria e Aplicacao de Grafos
  Nome: Maria Eduarda Carvalho Santos
  Matricula: 190092556
  UnB, Darcy Ribeiro - 2022.2

  #####################################################################################################

  Este programa nao consegue ser executado porque existe algum enderecamento de memoria feito de maneira inccorreta,
  que me impede de executar o algoritmo de maneira correta. Isso, obviamente, tem a ver com o fato de que deixei para
  realizar o trabalho de ultima hora, alem de n fatores que me impediram de comecar essa atividade mais cedo. Este recado
  eh mais um aviso ao leitor (professor Dibio) e um lembrete para a Maria Eduarda do futuro.

*/

#include<bits/stdc++.h>

using namespace std;

vector<vector<string>> grafo;                                               //lista de adjacencia que caracteriza o grafo
vector<vector<string>> cliques;                                             //lista de cliques contidos no grafo

//funcao utilizada para mostrar a lista de cliques e listas auxiliares - funcao de debug
void print(vector<string> v)
{  
  for(int i=0; i<v.size(); i++)
  {
    cout << v[i] << " ";
  }

  cout << endl;
}

//funcao utilizada para apresentar o grafo na tela
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

//funcao para calcular o coeficiente medio de aglomeracao
double coeficienteDeAglomeracao()
{
  double ans = 0;

  //percorrendo todos os vertices do grafo
  for(int v=1; v<=grafo.size(); v++)    //talvez tenha um erro nesse grafo.size() aqui
  {
    double coeficienteLocal = 0;

    //percorrendo todos os vizinhos do vertice
    for(auto vizinho : grafo[v])
    {
      //percorrendo todos os vizinhos dos vizinhos do vertice da primiera iteracao
      int elem;
      for(int i=0; i<grafo.size(); i++)
      {
        //buscando pelo vertice do grafo porque tentei fazer com chave-valor antes mas nao deu muito certo, entao apelei pra fazer com lista de lista e me lasquei
        if(grafo[i][0].compare(vizinho))
        {
          elem = i;
          break;
        }
      }

      for(auto outroVizinho : grafo[elem])
      {
        for(auto u : grafo[v])
        {
          if(outroVizinho == u)                                                     //se o vizinho do vizinho for vizinho do vertice analisado na iteracao, adicione +1 a resposta
          {
            coeficienteLocal++;
          }
        }
      }
    }

    if(grafo[v].size()>1)                                                           // se o vertice tiver mais de um vizinho, eh necessario recalcular o coeficiente de aglomeracao
    {
      ans += (double)coeficienteLocal/(grafo[v].size()*(grafo[v].size()-1));
    }
  }

  return (double)ans/grafo.size();                                                //retorna a soma total dos coeficientes de cada vertice e divide pela quantidade de vertices
}

//funcao que une dois conjuntos
vector<string> uniao(vector<string> A, vector<string> B)
{
  vector<string> U(A.size()+B.size());                                            // armazena a uniao entre os conjuntos - as listas

  auto it = set_union(A.begin(), A.end(), B.begin(), B.end(), U.begin());         //une os dois conjuntos

  U.resize(it- U.begin());

  return U;                                                                       //retorna a resposta como uma lista de nos que sao a uniao
}

//funcao que calcula a interseccao entre dois conjuntos
vector<string> interseccao(vector<string> A, vector<string> B)
{
  vector<string> I(A.size()+B.size());                                                    //armazena a interseccao entre os conjuntos - as listas

  auto it = set_intersection(A.begin(), A.end(), B.begin(), B.end(), I.begin());          //calcula a interseccao entre os dois conjuntos

  I.resize(it- I.begin());

  return I;                                                                               //retorna a resposta como uma lista de nos que sao a interseccao
}

//algoritmo de identificacao de cliques em um grafo nao direcionado e sem pesos
void bronKerboschSemPivot(vector<string> R, vector<string> P, vector<string> X)
{
    vector<string> PCopy(P);                                                                  //vetor copia de P utilizado para evitar perder o conteudo do vetor P ao longo da recursividade

    if(P.empty() && X.empty())
    {
      cliques.push_back(R);                                                                   // se nao existem mais nos a serem analisados, e nao ha como expandir o clique, adiciona o clique na resposta
      return;
    }

    for(auto v: P)                                                                            // analisando cada no do grafo,
    {
      //buscando pelo vertice do grafo porque tentei fazer com chave-valor antes mas nao deu muito certo, entao apelei pra fazer com lista de lista e me lasquei
      int elem;
      for(int i=0; i<grafo.size(); i++)
      {
        if(grafo[i][0].compare(v))
        {
          elem = i;
          break;
        }
      }

      //busque recursivamente as ligacoes entre os nos do grafo, a fim de averiguar se todos os nos possuem adjacencia entre si
      bronKerboschSemPivot(uniao(R, {v}), interseccao(PCopy, grafo[elem]), interseccao(X, grafo[elem]));

      PCopy.erase(find(PCopy.begin(), PCopy.end(), v));                                       //apague o no analisado na iteracao da lista de nos a serem analisados
      X = uniao(X, {v});                                                                      // caracterize o conjunto de vertices analisados que nao extendem o clique como a uniao entre o conjunto e o vertice analisado na iteracao
    }

    return; //talvez esse return aqui quebre meu codigo, mas talvez...
}

int main()
{
    freopen("entrada.txt", "r", stdin);                                                       //leitura do arquivo
    
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

    int maior = -1; int ans=-1;
    for(int i=0; i<cliques.size(); i++)                                                     //apresentando os cliques maximais acima de 3 vertices
    {
      if(cliques[i].size()>3)
      {
        cout << "o clique maximal eh\n";
        print(cliques[i]);                                                                   //imprimindo os cliques com tamanho maior que 3

        if(cliques[i].size()>maior)
        {
          ans = i;
          maior = cliques[i].size();                                                            //encontrando o clique maximo
        }
      }
    }

    vector<string> maximo = cliques[ans];                                                     //buscando na lista de cliques a posicao do maior clique

    cout << "o clique maximo eh\n"; print(maximo);                                              //apresentando o clique maximo
    
    cout << "O coeficiente medio de aglomeracao eh " << coeficienteDeAglomeracao() << endl;     //apresentando o coeficiente medio de aglomeracao 

    return 0;
}