# Plano do sistema de gerenciamento de gastos

Plano feito a partir de `DESCRICAO.md` e `AGENTS.md`. Nenhum código foi
escrito ainda.

## Respostas às dúvidas (já decididas)

1. **Categoria: id interno, nome externo.** Por dentro tudo trabalha pelo
   id; o usuário digita e vê sempre o nome.
2. **O `Gasto` referencia a categoria pelo id**, não pelo nome. Renomear
   categoria não mexe em nenhum gasto — o id continua o mesmo.
3. **Data (revisada):** dia de 1 a 31, mês de 1 a 12, e o dia é conferido
   contra o tamanho real do mês: 31 em abril falha, fevereiro aceita 1–28
   e 1–29 em ano bissexto (regra simples: divisível por 4).
4. **Deletar gasto:** devolve o gasto (para a interface mostrar) **ou** a
   mensagem de falha — nunca as duas coisas ao mesmo tempo.
5. **"Sem categoria" não tem limite** e não pode receber limite. A
   consulta consolidada mostra as outras categorias com limite; para ela,
   a interface mostra "sem limite".
6. **Atualizar gasto com categoria inexistente** manda para
   "sem categoria" (mesma regra da criação).

### Situação do `DESCRICAO.md`

Todas as decisões acima já estão refletidas no `DESCRICAO.md` — as
ambiguidades foram sanadas. Único ponto de atenção: a seção "Classes que
eu imagino" do `DESCRICAO.md` fala em "vetor" e "nome da categoria" no
`Gasto`; este plano refina isso para `list`/`map` e id interno (regras das
linhas 63–67). A regra vale mais que a imaginação da seção de classes.

## As classes

Três classes, uma por par de arquivos (`include/Nome.hpp` + `src/Nome.cpp`).
Nenhuma delas usa `cin`/`cout`: tudo entra por parâmetro e sai como retorno.
Quem lê e escreve na tela é a interface (`src/main.cpp`).

### Gasto — `include/Gasto.hpp`, `src/Gasto.cpp`

Guarda um gasto:

| campo | tipo | observe |
| --- | --- | --- |
| `id` | `int` | gerado pela `Carteira`, único |
| `valor` | `double` | sempre > 0 |
| `descricao` | `std::string` | |
| `dia`, `mes`, `ano` | `int` | guardo separado para comparar data |
| `idCategoria` | `int` | guardo o **id**, não o nome (decisão 2); o nome é resolvido na hora de mostrar |

Métodos: construtor, getters/setters e dois operadores:

- `operator<` — compara por (ano, mes, dia) e desempata pelo id. Serve para
  `std::sort` ordenar os gastos por data na consulta detalhada.
- `operator==` — compara **só o id** (dois gastos são o mesmo gasto se o id
  bate). Serve para `std::find` localizar um gasto pelo id.

### Categoria — `include/Categoria.hpp`, `src/Categoria.cpp`

Guarda uma categoria:

| campo | tipo | observe |
| --- | --- | --- |
| `id` | `int` | gerado pela `Carteira`, único |
| `nome` | `std::string` | único no sistema (regra) |
| `limite` | `double` | >= 0; a "sem categoria" não tem limite (decisão 5) |

Só getters/setters + uma validação de limite. Não precisa de operador de
comparação: as buscas são pelo nome (é a chave do `map`).

### Carteira — `include/Carteira.hpp`, `src/Carteira.cpp`

Dono de tudo. Guarda:

| campo | tipo | container da STL |
| --- | --- | --- |
| `gastos` | todos os gastos | `std::list<Gasto>` |
| `categorias` | todas as categorias | `std::map<std::string, Categoria>` |
| `limiteTotal` | informacional | `double` |
| `proximoIdGasto`, `proximoIdCategoria` | contadores | `int` |

**Por que `map<string, Categoria>` nas categorias:**

- toda operação com categoria é pelo nome — `map` acha na hora, sem varrer;
- a chave `string` já garante que não existem duas categorias com o mesmo
  nome (uma chave só aparece uma vez);
- `map` sai ordenado de fábrica: a consulta consolidada percorre as
  categorias em ordem alfabética sem precisar ordenar nada;
- a regra "nome único" vira quase grátis: `insert` não sobrescreve.

**Por que `list<Gasto>` nos gastos:**

- gastos são criados, consultados e **deletados no meio** da coleção o
  tempo todo; em `list` o `erase` de um elemento só mexe nos ponteiros dos
  vizinhos, em `vector` todo mundo depois dele desloca;
- a ordem de armazenamento não importa: toda consulta que precisa de ordem
  filtra para um `vector<Gasto>` temporário e ordena com `std::sort`
  (`std::sort` não funciona em `list`, que não tem acesso aleatório — por
  isso a cópia);
- os objetos são guardados **dentro** do container (nada de `new`/`delete`).

**Por que um `vector<Gasto>` temporário nas consultas:**

- `vector` + `std::sort` é a combinação pedida na atividade (exemplo 11:
  `sort` com `operator<` da minha classe).

Observação sobre achar gasto por id: como o `AGENTS.md` evita lambda, o
`std::find` recebe um `Gasto` "chave" só com o id preenchido e compara com
`operator==` — do jeito do exemplo 11.

### Structs de retorno — dentro de `Carteira.hpp`

A lógica devolve dados prontos; a interface só imprime:

- `ResumoCategoria { std::string categoria; double total; double limite; }`
  — resultado da consulta consolidada do mês. Para a "sem categoria" o
  `limite` vem preenchido com `-1` e a interface imprime "sem limite"
  (decisão 5).
- `DetalheMes { std::vector<Gasto> gastos; double total; int idEstouro; }`
  — resultado da consulta detalhada: gastos já ordenados por data, o total
  e o id do gasto que estourou o limite (`-1` se não estourou).

São só agrupamentos de dados (sem comportamento), não uma quarta classe.

### Métodos da `Carteira`

| método | devolve | nota |
| --- | --- | --- |
| `adicionarGasto(valor, descricao, dia, mes, ano, nomeCategoria)` | `bool`/`std::string` | valida tudo; o nome é convertido para id aqui; nome inexistente → "sem categoria" |
| `adicionarCategoria(nome, limite)` | `bool` | falha se nome repetido ou limite < 0 |
| `atualizarGasto(id, ...)` | `bool` | campos opcionais chegam já resolvidos pela interface; categoria inexistente → "sem categoria" (decisão 6) |
| `deletarGasto(id, gasto& copia)` | `bool` | acha com `std::find`; se achou, copia o gasto para a interface mostrar e só então apaga (decisão 4) |
| `atualizarCategoria(nome, novoNome, novoLimite)` | `bool` | sempre falha para "sem categoria" (não renomeia e não recebe limite — decisão 5); falha se o novo nome já existir; no `map`, renomear é apagar a chave antiga e inserir a nova com o **mesmo** objeto `Categoria` (o id não muda — decisão 2, e os gastos não são tocados) |
| `deletarCategoria(nome)` | `bool` | muda o `idCategoria` dos gastos dela para o de "sem categoria" |
| `consultarMes(mes, ano)` | `vector<ResumoCategoria>` | percorre o `map` inteiro |
| `consultarCategoria(nome, mes, ano)` | `DetalheMes` | filtra por `idCategoria`, copia para `vector`, `std::sort`, acumula e acha o estouro; "sem categoria" não tem limite, então `idEstouro` fica `-1` |
| `definirLimiteTotal(valor)` | `bool` | rejeita negativo |
| `gastoPorId(id)` | `bool` + cópia | apoio da interface |
| `nomeDaCategoria(id)` | `std::string` | a interface usa para mostrar o nome ao lado do gasto |

A `Carteira` começa no construtor com a categoria "sem categoria" (id fixo
0, **sem limite**) e contadores começando em 1. Onde o sistema precisar do
número de dias de um mês, a validação de data usa uma tabelinha simples:
31 em jan/mar/mai/jul/ago/out/dez, 30 em abr/jun/set/nov, fevereiro 28 —
29 se o ano for bissexto (`ano % 4 == 0`).

### Interface — `src/main.cpp`

- loop lendo comandos em texto da entrada padrão;
- interpreta os comandos conforme os nomes usados na `DESCRICAO.md`;
- toda mensagem de sucesso/falha é escrita aqui, nunca nas classes.

O comando de compilação do `README.md` (`src/*.cpp -Iinclude`) já pega
todos os arquivos planejados; não precisa mudar.

## Etapas

Cada etapa termina com o programa compilando (`g++ -std=c++11 -Wall -Wextra
src/*.cpp -Iinclude -o sistema`), rodando e um commit.

### Etapa 1 — Classe `Gasto` e teste solto

**Arquivos:** `include/Gasto.hpp`, `src/Gasto.cpp`, `src/main.cpp`.

- `Gasto` completa: campos, construtor, getters/setters, `operator<` e
  `operator==`.
- `main.cpp` só de teste: cria uns 4 gastos fora de ordem, ordena com
  `std::sort`, acha um por id com `std::find`, imprime.

**Como testar:** rodar o `./sistema` e conferir a ordem das datas e o id
encontrado. É o passo que ensina os dois operadores antes de virar sistema.

### Etapa 2 — `Categoria`, esqueleto da `Carteira` e comandos básicos

**Arquivos:** `include/Categoria.hpp`, `src/Categoria.cpp`,
`include/Carteira.hpp`, `src/Carteira.cpp`, `src/main.cpp`.

- `Categoria` completa.
- `Carteira` com os containers, contadores, construtor que cria "sem
  categoria" (sem limite), e só dois métodos: `adicionarCategoria` (falha
  se nome repetido ou limite < 0) e `listarCategorias` só para conferir.
- Interface com comandos em texto: `criar categoria <nome> <limite>`,
  `listar categorias`, `sair`.

**Como testar:** `exemplos/cenario1.txt` — cria "alimentação" 500, tenta
criar de novo (deve falhar), cria "lazer" 200, cria com limite -10 (deve
falhar), lista. A "sem categoria" já aparece na lista.

### Etapa 3 — Registrar e deletar gasto

**Arquivos:** altera `Carteira.*` e `main.cpp`.

- `adicionarGasto` com validação (valor > 0, descrição não vazia, e data:
  mês 1–12 e dia de 1 ao tamanho real do mês — fevereiro 28, ou 29 em ano
  bissexto) e a regra da categoria inexistente → "sem categoria" (o nome é
  convertido para id aqui dentro).
- `deletarGasto` com `std::find` + `operator==`.
- Interface ganha: `criar gasto ...`, `deletar gasto <id>`,
  `listar gastos` (mostrando o nome da categoria resolvido pelo id).

**Como testar:** `exemplos/cenario2.txt` — registra com categoria que não
existe (cai em "sem categoria"), valor 0 (falha), 31/04/2026 (falha, abril
tem 30), 30/02/2026 (falha, fevereiro tem 28 em 2026), 29/02/2028 (ok,
2028 é bissexto), deleta um id que não existe (falha), lista.

### Etapa 4 — Atualizar gasto, atualizar e deletar categoria

**Arquivos:** altera `Carteira.*` e `main.cpp`.

- `atualizarGasto` (troca valor/descrição/data/categoria; id não muda;
  categoria inexistente → "sem categoria").
- `atualizarCategoria` (nome e/ou limite; bloqueia "sem categoria" e nome
  repetido; os gastos não são tocados — o id da categoria não muda,
  decisão 2).
- `deletarCategoria` (muda o `idCategoria` dos gastos dela para o da
  "sem categoria"; os objetos `Gasto` permanecem os mesmos).

**Como testar:** `exemplos/cenario3.txt` — deleta "lazer" e vê os gastos
irem para "sem categoria"; tenta deletar/renomear "sem categoria" (falha);
atualiza um gasto de categoria.

### Etapa 5 — As duas consultas (aqui entra o `std::sort`)

**Arquivos:** altera `Carteira.*` e `main.cpp`.

- `consultarMes(mes, ano)` → varre o `map`, soma por categoria, devolve o
  `vector<ResumoCategoria>` (categorias sem gasto aparecem com total 0, como
  no exemplo da descrição).
- `consultarCategoria(nome, mes, ano)` → filtra os gastos da categoria no
  mês para um `vector<Gasto>`, `std::sort` por `operator<`, acumula e marca
  o id do gasto que ultrapassou o limite, devolve `DetalheMes`.
- Interface ganha: `consultar gastos por categoria no mes <mes> <ano>`
  e `consultar gastos individuais de uma categoria no mes <categoria> <mes> <ano>`.

**Como testar:** `exemplos/cenario4.txt` reproduzindo o "Exemplo de uso"
do `DESCRICAO.md` e conferindo linha a linha a saída esperada (inclusive o
"(Ultrapassou o limite da categoria de R$ 200,00)").

### Etapa 6 — Limite da carteira e acabamento

**Arquivos:** altera `Carteira.*` e `main.cpp`.

- `definirLimiteTotal` (rejeita negativo) e comando `definir limite total <valor>`.
- Tratar comandos inválidos (mostrar mensagem de falha, sem quebrar).
- Rodar **todos** os `exemplos/` de novo, do primeiro ao último, para
  garantir que a etapa nova não quebrou a antiga.

**Como testar:** `exemplos/cenario5.txt` com o cenário completo.

## Fora do plano (por enquanto)

- Salvar em arquivo (desafio 1 da seção 6 do enunciado) — só depois das 6
  etapas.
- Nada de lambda, `unordered_map`, `new`/`delete` ou herança, conforme
  `AGENTS.md`.
