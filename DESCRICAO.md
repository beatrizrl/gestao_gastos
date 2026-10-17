# Descrição do sistema

Escreva este arquivo sem IA, antes de qualquer código. Apague as instruções
em itálico quando preencher.

## Nome e ideia
    Um sistema de gerenciamento de gastos para ajudar o usuario a acompanhar e controlar os gastos mensais.
    Permintindo o usuario possa registrar os gastos por categoria e delimitando limites de valor para gastar em cada categoria.
    Alem de permitir que possa vizualizar os gastos mensais individualmente ou por categoria.


## O que o sistema guarda
    O sistema guarda os seguintes itens:
    - Gastos
        - valor
        - descriçao
        - data (DD/MM)
        - categoria
        - codigo do gasto(id numerico)
    - Categoria
        - nome
        - limite
        - codigo da categoria(id numerico)
    - Carteira
        - limite total

## O que dá para fazer
    As funcionalidade serão:
    - Adicionar novo gasto:,
        - pede: valor do gasto, descriçao, data (DD/MM) e categoria.
        - mostrar: resposta de sucesso ou falha no registro
    - Adicionar nova categoria:
        - pede: nome da nova categoria e limite*
        - mostra: resposta de sucesso ou falha na operaçao no registro
    - Consultar gastos por categoria no mes: 
        - pede: o mes(numero)
        - mostra: a lista de todas as categorias e o valor gasto em cada e o limite da categoria
    - Consultar gastos individuais de uma categoria em determinado mes 
        - pede: a categoria(numero) e mes 
        - mostra: a lista de todos os gastos com a descriçao ao lado, o valor daquela categoria naquele mes e indica caso ultrapase o valor limite qual gasto que fez ultrapassar. E a lista sera ordenada por data
    - Deletar categoria:
        - pede: a categoria(numero)
        - mostra: mensagem de sucesso ou de falha na operaçao
    - Atualizar gasto:
        - pede: o id do gasto, valor*, descriçao*, categoria*, data(DD/MM)*
        - mostra: se for sucesso mostra gasto atualizado ou mensagem de falha na operaçao
    - Deletar Gasto:
        - pede: o id do gasto
        - mostra: retorna o gasto e a mensagem que foi deletado ou que deu falha 
    - Definir Limite da carteira:
        - pede: valor do limite
        - mostra: retorna o valor do limite e mensagem de sucesso ou de falha
    - Atualizar categoria:
        - pede: o id da categoria, limite*, nome*
        - mostra: retorna a categoria atualizada ou a mensagem de falha na operaçao

    Atributos opcionais estao marcados com "*" 

## Regras
    - Não pode ter duas categoria com nomes iguais
    - os limites nao podem ser negativos 
    - o sistema começa com uma categoria chamada sem categoria e que nao pode ser apagada
    - Quando uma categoria for deletada os gastos que estao nela serao alocados a categoria chamada "sem categoria"
    - A soma dos limites de todas as categorias pode ultrapassar ao limite total da carteira
    - todo gasto e categoria criado tem seu id criado pelo sistema automaticamente e unico para cada, o id é um valor numerico incremental 

## Quando algo dá errado
   - Nas operaçoes de criaçao de gasto ou categoria se algum valor ou dado estiver fora do formato adequado o sistema deve avisar e nao prosseguir. 
   - Se ao tentar criar ou atualizar a categoria para um nome que ja extiste o sistema deve avisar e nao prosseguir
   - Se tentar apagar um gasto ou categoria que nao existe o sistema deve avisar e nao prosseguir


## Classes que eu imagino
O sistema deve ter as seguintes classes:
    - Gasto
        - responsabilidade: armazenas as informaçoes de cada gasto

    - Categoria
        - responsabilidade: armazenar o limite definido dela e o nome
    - gasto mensal
        - responsabilidade: armazenar todos os gastos de determinado mes 
    - Carteira
        - responsabilidade: armazenar os gastos mensais de todos os meses adicionados, as categorias criadas, e as funcionalidades do sistema 

_Uma linha para cada classe: o nome e a responsabilidade dela. Não precisa
ser a versão final; o plano com a IA pode mudar isso._

## Exemplo de uso
U - o usuario entra no sistema
U - digita o comando de criar uma categoria com os dados: aliementaçao de limite 500
S - Devolve uma mensagem que a categoria foi criada com sucesso
U - digita o comando de criar uma categoria com os dados: lazer de limite 200
S - Devolve uma mensagem que a categoria foi criada com sucesso
U - digita o comando de criar uma categoria com os dados: transporte de limite 300
S - Devolve uma mensagem que a categoria foi criada com sucesso
U - Digita o comando de criar um gasto com os dados: Categoria: Alimentação, Descrição: Sorvete, Valor: R$ 13,00, Data: 05/10
S - Exibe: "Gasto registrado com sucesso!"
U - Digita o comando de criar um gasto com os dados: Categoria: Lazer, Descrição: Cinema, Valor: R$ 60,00, Data: 10/10
S - Exibe: "Gasto registrado com sucesso!"
U - Digita o comando de criar um gasto com os dados: Categoria: Lazer, Descrição: Show, Valor: R$ 160,00, Data: 15/10
S - Exibe: "Gasto registrado com sucesso!"
U - Digita o comando de consultar gastos por categoria no mês: Mês: 10
S - Exibe a lista consolidada do mês 10:
Alimentação: R$ 13,00 / Limite: R$ 500,00
Lazer: R$ 220,00 / Limite: R$ 200,00
Transporte: R$ 0,00 / Limite: R$ 300,00
U - Digita o comando de consultar gastos individuais de uma categoria no mês com os dados: Categoria: Lazer (ID: 2), Mês: 10
S - Exibe a listagem ordenada por data:
10/10 - Cinema: R$ 60,00 [ID: 2]
15/10 - Show: R$ 160,00 [ID: 3] (Ultrapassou o limite da categoria de R$ 200,00)
Total gasto em Lazer no mês: R$ 220,00
