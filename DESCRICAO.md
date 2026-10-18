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
        - data (DD/MM/AAAA)
        - categoria(nome)
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
        - pede: valor do gasto, descriçao, data (DD/MM/AAAA) e categoria.
        - mostrar: resposta de sucesso ou falha no registro
    - Adicionar nova categoria:
        - pede: nome da nova categoria e limite*
        - mostra: resposta de sucesso ou falha na operaçao no registro
    - Consultar gastos por categoria no mes: 
        - pede: o mes(numero) e o ano(numero)
        - mostra: a lista de todas as categorias e o valor gasto em cada e o limite da categoria
    - Consultar gastos individuais de uma categoria em determinado mes 
        - pede: a categoria(nome) e mes(numero) e ano(numero)
        - mostra: a lista de todos os gastos com a descriçao ao lado, o valor daquela categoria naquele mes/ano ordenado por data(dia/mes/ano) e indica qual gasto fez com que o total acumulado ultrapassasse o limite da categoria. 
    - Deletar categoria:
        - pede: nome da categoria
        - mostra: mensagem de sucesso ou de falha na operaçao
    - Atualizar gasto:
        - pede: o id do gasto, valor*, descriçao*, categoria*(nome)*, data(DD/MM/AAAA)*
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
    - o valor dos gastos deve ser maior que O
    - o sistema começa com uma categoria chamada "sem categoria" e que nao pode ser apagada. Essa categoria não pode ser renomeada. Seu limite pode ser mantido/atualizado conforme necessário, mas ela continua existindo.
    - Quando uma categoria for deletada os gastos que estao nela serao alocados a categoria chamada "sem categoria". Os objetos Gasto permanecem os mesmos, apenas o nome da categoria associada a eles é alterado.
    - A soma dos limites de todas as categorias pode ultrapassar ao limite total da carteira. o limite total da carteira tem caráter informativo, não gerando alertas automáticos.
    - Todo gasto e categoria criado tem seu id criado pelo sistema automaticamente e unico para cada tipo. São mantidos dois contadores separados (um para gastos e um para categorias).
    - As operações com categoria devem ser feitas pelo nome. O id da categoria é usado apenas internamente pelo sistema. ao criar um gasto, e se for informada uma categoria que não existe, o gasto deve ser alocado à categoria "sem categoria".
    - A categoria "sem categoria" já existe desde o início e tem um id próprio (fixo). Não pode ser deletada.

## Quando algo dá errado
    - Nas operaçoes de criaçao de gasto ou categoria se algum valor ou dado estiver fora do formato adequado o sistema deve avisar e nao prosseguir. (ex.: valor <= 0, limites negativos, data inválida no formato DD/MM/AAAA)
    - Se ao tentar criar ou atualizar a categoria para um nome que ja extiste o sistema deve avisar e nao prosseguir
    - Se tentar apagar um gasto ou categoria que nao existe o sistema deve avisar e nao prosseguir
    - Não é permitido apagar ou renomear a categoria "sem categoria"


## Classes que eu imagino
    o sistema deve ter as seguintes classes:
    - Gasto
        - responsabilidade: armazenar as informações de cada gasto: id, valor, descrição, dia, mes, ano, nome da categoria
    - Categoria
        - responsabilidade: armazenar o id, o nome e o limite definido para cada categoria
    -  Carteira
        - responsabilidade: armazenar todos os gastos (em um único vetor), todas as categorias criadas (em um único vetor), o limite total da carteira, gerenciar os IDs automáticos separados e conter todas as funcionalidades do sistema   



## Exemplo de uso
    U - o usuario entra no sistema
    U - digita o comando de criar uma categoria com os dados: alimentação de limite 500
    S - Devolve uma mensagem que a categoria foi criada com sucesso
    U - digita o comando de criar uma categoria com os dados: lazer de limite 200
    S - Devolve uma mensagem que a categoria foi criada com sucesso
    U - digita o comando de criar uma categoria com os dados: transporte de limite 300
    S - Devolve uma mensagem que a categoria foi criada com sucesso
    U - Digita o comando de criar um gasto com os dados: Categoria: alimentação, Descrição: Sorvete, Valor: R$ 13,00, Data: 05/10/2026
    S - Exibe: "Gasto registrado com sucesso!"
    U - Digita o comando de criar um gasto com os dados: Categoria: lazer, Descrição: Cinema, Valor: R$ 60,00, Data: 10/10/2026
    S - Exibe: "Gasto registrado com sucesso!"
    U - Digita o comando de criar um gasto com os dados: Categoria: lazer, Descrição: Show, Valor: R$ 160,00, Data: 15/10/2026
    S - Exibe: "Gasto registrado com sucesso!"
    U - Digita o comando de consultar gastos por categoria no mês: Mês: 10, Ano: 2026
    S - Exibe a lista consolidada do mês 10/2026:
    Alimentação: R$ 13,00 / Limite: R$ 500,00
    Lazer: R$ 220,00 / Limite: R$ 200,00
    Transporte: R$ 0,00 / Limite: R$ 300,00
    U - Digita o comando de consultar gastos individuais de uma categoria no mês com os dados: Categoria: lazer, Mês: 10,
    Ano: 2026
    S - Exibe a listagem ordenada por data: 
    10/10/2026 - Cinema: R$ 60,00 ID: 21
    15/10/2026 - Show: R$ 160,00 [ID: 3] (Ultrapassou o limite da categoria de R$ 200,00)
    Total gasto em Lazer no mês 10/2026: R$ 220,00
