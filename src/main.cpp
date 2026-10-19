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

        // Comando: listar gastos
        if (linha == "listar gastos") {
            carteira.listarGastos(std::cout);
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

        // Comando: criar gasto <valor> <descrição> <dia/mes/ano> <categoria>
        // A data é o primeiro token que contém '/', então descrição e
        // categoria podem ter espaços.
        if (cmd1 == "criar" && cmd2 == "gasto") {
            std::string prefixo = "criar gasto ";
            if (linha.size() < prefixo.size()) {
                std::cout << "Falha ao criar gasto: dados incompletos" << std::endl;
                continue;
            }
            std::string resto = linha.substr(prefixo.size());
            trim(resto);

            size_t esp1 = resto.find(' ');
            if (esp1 == std::string::npos) {
                std::cout << "Falha ao criar gasto: dados incompletos" << std::endl;
                continue;
            }
            std::string valorStr = resto.substr(0, esp1);
            std::string depoisValor = resto.substr(esp1 + 1);
            trim(depoisValor);

            size_t posBarra = depoisValor.find('/');
            if (posBarra == std::string::npos) {
                std::cout << "Falha ao criar gasto: data ausente (use dia/mes/ano)" << std::endl;
                continue;
            }
            size_t iniData = depoisValor.rfind(' ', posBarra);
            if (iniData == std::string::npos) {
                iniData = 0;
            } else {
                iniData = iniData + 1;
            }
            size_t fimData = depoisValor.find(' ', posBarra);
            std::string descricao = depoisValor.substr(0, iniData);
            std::string dataStr;
            std::string categoria;
            if (fimData == std::string::npos) {
                dataStr = depoisValor.substr(iniData);
            } else {
                dataStr = depoisValor.substr(iniData, fimData - iniData);
                categoria = depoisValor.substr(fimData + 1);
            }
            trim(descricao);
            trim(dataStr);
            trim(categoria);
            if (categoria.empty()) {
                std::cout << "Falha ao criar gasto: categoria ausente" << std::endl;
                continue;
            }

            double valor = 0.0;
            std::stringstream ssValor(valorStr);
            if (!(ssValor >> valor)) {
                std::cout << "Falha ao criar gasto: valor invalido" << std::endl;
                continue;
            }

            int dia = 0, mes = 0, ano = 0;
            char barra1 = 0, barra2 = 0;
            std::stringstream ssData(dataStr);
            if (!(ssData >> dia >> barra1 >> mes >> barra2 >> ano)
                || barra1 != '/' || barra2 != '/') {
                std::cout << "Falha ao criar gasto: data invalida (use dia/mes/ano)" << std::endl;
                continue;
            }

            if (carteira.adicionarGasto(valor, descricao, dia, mes, ano, categoria)) {
                std::cout << "Gasto registrado com sucesso!" << std::endl;
            } else {
                std::cout << "Falha ao registrar gasto" << std::endl;
            }
            continue;
        }

        // Comando: deletar gasto <id>
        if (cmd1 == "deletar" && cmd2 == "gasto") {
            std::string idStr;
            ss >> idStr;
            int id = 0;
            std::stringstream ssi(idStr);
            if (idStr.empty() || !(ssi >> id)) {
                std::cout << "Falha ao deletar gasto: id invalido" << std::endl;
                continue;
            }

            Gasto removido;
            if (carteira.deletarGasto(id, removido)) {
                std::cout << "Gasto deletado com sucesso" << std::endl;
                std::cout << "  id=" << removido.getId()
                          << " " << removido.getDia() << "/" << removido.getMes()
                          << "/" << removido.getAno()
                          << " " << removido.getDescricao()
                          << " R$ " << removido.getValor()
                          << " categoria: "
                          << carteira.nomeDaCategoria(removido.getIdCategoria())
                          << std::endl;
            } else {
                std::cout << "Falha ao deletar gasto: id nao encontrado" << std::endl;
            }
            continue;
        }

        std::cout << "Comando desconhecido" << std::endl;
    }

    return 0;
}
