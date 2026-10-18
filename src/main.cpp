#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include "Carteira.hpp"
#include "Categoria.hpp"
#include "Gasto.hpp"

void trim(std::string &s) {
    const char *whitespace = " \t\r\n";
    size_t ini = s.find_first_not_of(whitespace);
    if (ini == std::string::npos) {
        s.clear();
        return;
    }
    size_t fim = s.find_last_not_of(whitespace);
    s = s.substr(ini, fim - ini + 1);
}

int main() {
    Carteira carteira;
    std::string linha;

    while (std::getline(std::cin, linha)) {
        trim(linha);
        if (linha.empty()) {
            continue;
        }

        if (linha == "sair") {
            break;
        }

        // Comando: listar categorias
        if (linha == "listar categorias") {
            carteira.listarCategorias(std::cout);
            continue;
        }

        // Comando: criar categoria <nome> <limite>
        // Ex.: "criar categoria alimentação 500" ou com nome sem espaços? DESCRICAO mostra nomes simples
        // Mas vamos parsear: primeiro token "criar", segundo "categoria", resto nome+limite
        std::stringstream ss(linha);
        std::string cmd1, cmd2;
        ss >> cmd1 >> cmd2;
        if (cmd1 == "criar" && cmd2 == "categoria") {
            std::string nome;
            std::string limiteStr;
            // Le nome até o último token? ou pegar tudo entre categoria e limite
            // Mais simples: lê tudo depois de "criar categoria "
            std::string resto = linha;
            // remove prefixo "criar categoria "
            size_t prefixo = resto.find("criar categoria ");
            if (prefixo == 0) {
                resto = resto.substr(std::string("criar categoria ").size());
            }
            trim(resto);
            // último espaço separa limite
            size_t ultEspaco = resto.rfind(' ');
            if (ultEspaco == std::string::npos) {
                std::cout << "Falha ao criar categoria: limite ausente" << std::endl;
                continue;
            }
            nome = resto.substr(0, ultEspaco);
            limiteStr = resto.substr(ultEspaco + 1);
            trim(nome);
            trim(limiteStr);
            if (nome.empty()) {
                std::cout << "Falha ao criar categoria: nome vazio" << std::endl;
                continue;
            }
            // converte limite
            double limite = 0.0;
            // tenta converter
            std::stringstream ssl(limiteStr);
            if (!(ssl >> limite)) {
                std::cout << "Falha ao criar categoria: limite invalido" << std::endl;
                continue;
            }
            if (carteira.adicionarCategoria(nome, limite)) {
                std::cout << "Categoria criada com sucesso" << std::endl;
            } else {
                std::cout << "Falha ao criar categoria" << std::endl;
            }
            continue;
        }

        std::cout << "Comando desconhecido" << std::endl;
    }

    return 0;
}
