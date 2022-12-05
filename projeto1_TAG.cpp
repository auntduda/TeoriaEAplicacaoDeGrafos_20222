#include<bits/stdc++.h>

using namespace std;

vector<pair<string, vector<string>>> adj;

void print()
{
    for(int i=0; i<adj.size(); i++)
    {
        cout << adj[i].first << ": \n";

        for(int j=0; j<adj[i].second.size(); j++)
        {
            cout << *(adj[i].second.begin()+j) << " ";
        }

        cout << endl;
    }
}

int main()
{
    string s; string dotcomma = ";";
    vector<string> aux;                                 //vector auxiliar na leitura da entrada

    while(cin >> s)
    {
        // cout << s << endl;

        string edge = s;                                //variavel auxiliar para insercao de dados limpos no grafo
        edge.pop_back();                                //limpeza do dado
        aux.push_back(edge);                            //insercao do dado no vector auxiliar

        if(s.find(";")!=-1)                             //como o metodo find retorna -1 caso o caractere nao seja encontrado, a condicional abaixo sera executada quando ";" estiver em alguma posicao da string s 
        {
            string node = aux[0];                       //caracterizacao da chave do vetor de adjacencias
            aux.erase(aux.begin());                     //remocao da chave no vector auxiliar
            adj.push_back(make_pair(node, aux));        //criacao das conexoes na lista de adjacencia
            aux.clear();                                //limpeza do vector auxiliar para iniciar a nova conexao
        }
    }

    

    return 0;
}