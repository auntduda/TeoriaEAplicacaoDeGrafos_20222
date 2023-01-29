"""
Projeto 1 da discplina CIC0199 - Teoria e Aplicacao de Grafos
Nome: Maria Eduarda Carvalho Santos
Matricula: 190092556
UnB, Darcy Ribeiro - 2022.2
-------------------------------------------------------------------------------------------------------------------------
Para analisar o resultado do emprelhamento, é necessário utilizar

$ python3 190092556_MariaEduardaCarvalhoSantos-projeto2_TAG.py > output.txt

como linha de comando no terminal, e então, abrir o arquivo "output.txt" para visualizar os resultados.
"""

def monta_grafo():
    
    professores = {}        # montando um dicionario de professores disponiveis a partir do input
    escolas = {}            # montando um dicionario de escolas disponiveis a partir do input

    # fazendo parse do arquivo cabuloso

    with open('entradaProj2TAG.txt') as file:
        for line in file:
            if not line.startswith('//'):
                line = line.replace('(', '').replace(')','').strip().split(':')
                
                if line[0].startswith('P'):
                    professor = line[0].split()
                    # criando um dicionario de professores cujas chaves são a identificacao do professor (o numero do input), quantas habilitacoes ele possui, as escolas que ele prefere, e o local onde ele ira trabalhar - a ser calculado posteriormente 
                    professores[int(professor[0].replace(',', '').replace('P', ''))] = {'id': int(professor[0].replace(',', '').replace('P', '')), 'habilitacao': int(professor[1]), 'escolas_pref': list(map(int, line[1].strip().replace('E', '').split(', '))), 'local_trabalho': -1, 'livre': True}
                elif line[0].startswith('E'):
                    vagas = [int(i) for i in range(1, len(line))]   # criando uma lista de vagas disponiveis em cada escola a partir do input
                    
                    if len(vagas) < 2:
                        vagas.append(-1)
                    # criando um dicionario de escolas cujas chaves sao a identificacao da escola, as habilitacoes esperadas dos professores e quantas vagas ja foram preenchidas
                    escolas[int(line[0].replace('E', ''))] = {'id': int(line[0].replace('E', '')),'habilitacao_pret': vagas, 'vagas_preenchidas': [False] * len(vagas), 'professores': [-1, -1], 'margem': [False, False]}

    return professores, escolas


def aloca(id_professor, professores_livres, escolas_livres):
    
    for i in range(len(professores_livres[id_professor]['escolas_pref'])):
        id_escola = professores_livres[id_professor]['escolas_pref'][i]                                                         # analisando todas as escolas que o professor tem preferencia em lecionar
        
        # se o professor estiver livre, e a escola aceita a habilitacao dele,
        if (professores_livres[id_professor]['livre'] == True) and (professores_livres[id_professor]['habilitacao'] >= escolas_livres[id_escola]['habilitacao_pret'][0]) or ((professores_livres[id_professor]['habilitacao'] >= escolas_livres[id_escola]['habilitacao_pret'][1]) and (escolas_livres[id_escola]['habilitacao_pret'][1] != -1)):
            # AND a primeira vaga ainda nao foi preenchida,
            if (escolas_livres[id_escola]['vagas_preenchidas'][0] == False) and (professores_livres[id_professor]['habilitacao'] >= escolas_livres[id_escola]['habilitacao_pret'][0]):
                # aloca-se o professor para aquela vaga
                professores_livres[id_professor]['livre'] = False
                professores_livres[id_professor]['local_trabalho'] = escolas_livres[id_escola]['id']
                escolas_livres[id_escola]['vagas_preenchidas'][0] = True
                escolas_livres[id_escola]['professores'][0] = professores_livres[id_professor]['id']
                
                # se a habilitacao do professor alocado for maior que a habilitacao solicitada pela escola,
                if professores_livres[id_professor]['habilitacao'] > escolas_livres[id_escola]['habilitacao_pret'][0]:
                    escolas_livres[id_escola]['margem'][0] = True                                                               # nao ha margem para troca de professores,
                elif professores_livres[id_professor]['habilitacao'] == escolas_livres[id_escola]['habilitacao_pret'][0]:       # se nao,
                    escolas_livres[id_escola]['margem'][0] = False                                                              # aquele professor pode ser trocado em algum momento - para construir um emparelhamento mais estavel
                
                return True
            
            # AND a segunda vaga ainda nao foi preenchida, aloca-se o professor para aquela vaga
            if (escolas_livres[id_escola]['vagas_preenchidas'][1] == False) and ((professores_livres[id_professor]['habilitacao'] >= escolas_livres[id_escola]['habilitacao_pret'][1]) and (escolas_livres[id_escola]['habilitacao_pret'][1] != -1)):
                professores_livres[id_professor]['livre'] = False
                professores_livres[id_professor]['local_trabalho'] = escolas_livres[id_escola]['id']
                escolas_livres[id_escola]['vagas_preenchidas'][1] = True
                escolas_livres[id_escola]['professores'][1] = professores_livres[id_professor]['id']
                
                # se a habilitacao do professor alocado for maior que a habilitacao solicitada pela escola,
                if professores_livres[id_professor]['habilitacao'] > escolas_livres[id_escola]['habilitacao_pret'][0]:
                    escolas_livres[id_escola]['margem'][0] = True                                                               # nao ha margem para troca de professores,
                elif professores_livres[id_professor]['habilitacao'] == escolas_livres[id_escola]['habilitacao_pret'][0]:       # se nao,
                    escolas_livres[id_escola]['margem'][0] = False                                                              # aquele professor pode ser trocado em algum momento - para construir um emparelhamento mais estavel
                
                return True                                                                                                     # e entao, foi possivel alocar o professor
    return False                                                                                                                # caso as condicoes acima nao possam ser satisfeitas em algum momento, o professor nao pôde ser alocado em nenhuma escola


def substitui(id_professor, professores_livres, escolas_livres):

    antigo = None
    for i in range(len(professores_livres[id_professor]['escolas_pref'])):
        id_escola = professores_livres[id_professor]['escolas_pref'][i]
        
        # se o professor possui a primeira habilitacao melhor ou igual à que a escola pede AND a escola tem uma habilitacao especifica,
        if (professores_livres[id_professor]['habilitacao'] >= escolas_livres[id_escola]['habilitacao_pret'][0]) or ((professores_livres[id_professor]['habilitacao'] >= escolas_livres[id_escola]['habilitacao_pret'][1]) and (escolas_livres[id_escola]['habilitacao_pret'][1] != -1)):
            
            # AND o professor analisado nao esta alocado na escola analisada,
            if(professores_livres[id_professor]['habilitacao'] >= escolas_livres[id_escola]['habilitacao_pret'][0]) and (escolas_livres[id_escola]['margem'][0] == True) and (escolas_livres[id_escola]['professores'][0] != professores_livres[id_professor]['id']) and (escolas_livres[id_escola]['professores'][1] != professores_livres[id_professor]['id']):
                professores_livres[id_professor]['livre'] = False                                       # o professor nao esta mais livre para ser alocado
                professores_livres[id_professor]['local_trabalho'] = escolas_livres[id_escola]['id']    # o novo local de trabalho dele é a escola atualmente analisada
                escolas_livres[id_escola]['vagas_preenchidas'][0] = True                                # a vaga que pede aquela habilitacao especifica esta preenchida
                
                if professores_livres[id_professor]['habilitacao'] > escolas_livres[id_escola]['habilitacao_pret'][0]:      # se o professor alocado tem uma habilitacao maior que a solicitada pela escola,
                    escolas_livres[id_escola]['margem'][0] = True                                                           # ainda existe margem para troca
                elif professores_livres[id_professor]['habilitacao'] == escolas_livres[id_escola]['habilitacao_pret'][0]:   # se nao,
                    escolas_livres[id_escola]['margem'][0] = False                                                          # este sera o professor definitivo
                
                # alocando definitivamente o novo professor na escola, e retornando o professor desalocado para coloca-lo de volta na fila de professores que ainda precisam de escola
                antigo = escolas_livres[id_escola]['professores'][0]
                professores_livres[antigo]['livre'] = True
                escolas_livres[id_escola]['professores'][0] = professores_livres[id_professor]['id']
                
                return antigo

            # verificando se o professor a substituir outro se encaixa na segunda habilitacao - tal como feito acima
            if (professores_livres[id_professor]['habilitacao'] >= escolas_livres[id_escola]['habilitacao_pret'][1] and (escolas_livres[id_escola]['habilitacao_pret'][1] != -1)) and (escolas_livres[id_escola]['margem'][1] == True) and (escolas_livres[id_escola]['professores'][0] != professores_livres[id_professor]['id']) and (escolas_livres[id_escola]['professores'][1] != professores_livres[id_professor]['id']):
                professores_livres[id_professor]['livre'] = False
                professores_livres[id_professor]['local_trabalho'] = escolas_livres[id_escola]['id']
                escolas_livres[id_escola]['vagas_preenchidas'][1] = True
                
                if professores_livres[id_professor]['habilitacao'] > escolas_livres[id_escola]['habilitacao_pret'][1]:
                    escolas_livres[id_escola]['margem'][1] = True
                elif professores_livres[id_professor]['habilitacao'] == escolas_livres[id_escola]['habilitacao_pret'][1]:
                    escolas_livres[id_escola]['margem'][1] = False
                
                antigo = escolas_livres[id_escola]['professores'][1]
                professores_livres[antigo]['livre'] = True
                escolas_livres[id_escola]['professores'][1] = professores_livres[id_professor]['id']
                
                return antigo
    return -1


def gale_shapley(professores_livres, escolas_livres, maximo, it):

    professor_f = []                                                                # inicializacao da fila de professores que serao alocados em alguma escola
    
    for i in professores_livres.keys():                                             # identificando os professores que ainda nao estao alocados em uma vaga
        if professores_livres[i]['livre'] == True:                                  # inicialmente indicando que todos os professores estao livres para serem alocados a uma escola
            professor_f.append(i)                                                   # criando uma fila de professores que sao elegiveis a uma escola

    while len(professor_f) > 0:                                                     # se a fila de professores nao estiver vazia,
        atual = professor_f[0]                                                      # analise a situacao deste professor
        professor_f.pop(0)                                                          # tire este professor da fila

        if type(atual) == int:
            if (aloca(atual, professores_livres, escolas_livres) == False):         # se nao foi possivel alocar o professor em alguma escola,
                saiu = substitui(atual, professores_livres, escolas_livres)         # é verificado se é melhor substituir algum dos professores ja alocados na escola
                
                if saiu != -1:                                                      # caso nao seja possivel alocar o professor em nenhuma escola,
                    atual = professores_livres[saiu]
                    professor_f.append(atual)                                       # ele volta para a fila de alocacao

    if maximo == False or it == 10:                                                 # se a primeira analise de alocacao foi feita,
        professores_alocados = 0                                                    # comeca a contagem de professores que foram alocados
        
        for i in escolas_livres:                                                    # para cada escola livre,
            x = None
            
            if escolas_livres[i]['professores'][1] != -1:                           # se a escola ja tem professor alocado,
                professores_alocados += 1                                           # acrescenta-se +1 unidade na lista de professores ja alocados
                x = escolas_livres[i]['professores'][1]
            else:                                                                   # e caso contrario, sinalizar a falta de professor alocado com o caractere 'X'
                x = 'X'

            print(f"A escola {i} ficou com os professores {escolas_livres[i]['professores'][0]} e {x}")

            if escolas_livres[i]['professores'][0] != -1:
                professores_alocados += 1

        print(f'\nProfessores alocados {professores_alocados}')


def main():
    professores_livres, escolas_livres = monta_grafo()                                              # realiza a leitura do arquivo e relaciona-os em uma estrutura de dados dicionario

    print('Emparelhamento estavel: (X significa que nao tem nenhum professor alocado na vaga)\n')
    
    gale_shapley(professores_livres, escolas_livres, False, 0)                                      # aplica o algoritmo de Gale Shapley, descrito no artigo repassado na descricao do projeto
    
    print('\nEmparelhamento maximo: (X significa que nao tem nenhum professor alocado na vaga)\n')
    
    alocados = 0
    for i in range(11):
        gale_shapley(professores_livres, escolas_livres, True, i)                                   # realizando uma segunda aplicacao do algoritmo de Gale-Shapley, para encontrar um emparelhamento melhor que o anteriormente calculado


if __name__ == "__main__":
    main()