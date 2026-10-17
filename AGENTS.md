# Instruções para a IA neste projeto

- Este é o projeto de um estudante de C++ que está aprendendo STL. Responda
  em português, de forma curta e didática. Quando explicar um conceito, use
  um exemplo pequeno.
- O sistema está descrito em `DESCRICAO.md`, escrito pelo estudante. O plano
  de implementação fica em `PLANO.md`. Leia os dois antes de propor ou
  alterar código. Se a descrição for ambígua, pergunte em vez de supor.
- Use C++11 e somente a biblioteca padrão. Não instale nada e não acesse a
  rede.
- Prefira guardar objetos diretamente nos containers da STL. Não use `new`
  nem `delete`, a não ser que o estudante peça herança com método virtual;
  nesse caso, explique quem libera a memória.
- Evite recursos que o estudante ainda não viu, como lambda e
  `unordered_map`, a não ser que ele peça. Se achar que um deles ajuda,
  sugira e explique antes de usar.
- Uma classe por par de arquivos: `include/Nome.hpp` e `src/Nome.cpp`.
- As classes do sistema não usam `cin` nem `cout`. Elas recebem dados por
  parâmetro e devolvem resultados. Toda leitura e escrita na tela fica na
  interface, em arquivos próprios.
- Faça somente o que foi pedido nesta mensagem, ou a etapa do `PLANO.md` que
  o estudante indicou. Não acrescente funcionalidades que não foram pedidas.
- Quando o estudante pedir um plano, não edite: diga quais arquivos e quais
  métodos você criaria ou alteraria, e por quê.
- Depois de editar, compile com o comando que está no `README.md` e rode os
  arquivos de `exemplos/`, se existirem. Mostre o resultado real. Se criar um
  arquivo novo, atualize o comando de compilação no `README.md`.
- Não altere `DESCRICAO.md`, nem as seções "Onde uso a STL" e "Como usei a
  IA" do `README.md`. Essas partes são escritas pelo estudante. Se achar que
  a descrição precisa mudar, sugira a mudança.
- Não faça commit e não mexa no histórico do Git.
