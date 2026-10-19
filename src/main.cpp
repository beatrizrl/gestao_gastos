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

        // Comando: atualizar gasto <id> [valor <v>] [descricao <texto>] [data <d/m/a>] [categoria <nome>]
        if (cmd1 == "atualizar" && cmd2 == "gasto") {
            std::string idStr;
            ss >> idStr;
            int id = 0;
            std::stringstream ssi(idStr);
            if (idStr.empty() || !(ssi >> id)) {
                std::cout << "Falha ao atualizar gasto: id invalido" << std::endl;
                continue;
            }

            Gasto atual;
            if (!carteira.gastoPorId(id, atual)) {
                std::cout << "Falha ao atualizar gasto: id nao encontrado" << std::endl;
                continue;
            }

            double valor = atual.getValor();
            std::string descricao = atual.getDescricao();
            int dia = atual.getDia();
            int mes = atual.getMes();
            int ano = atual.getAno();
            std::string nomeCat = carteira.nomeDaCategoria(atual.getIdCategoria());

            std::vector<std::string> tokens;
            std::string palavra;
            while (ss >> palavra) {
                tokens.push_back(palavra);
            }

            bool erro = false;
            for (size_t i = 0; i < tokens.size(); ++i) {
                if (tokens[i] == "valor") {
                    if (i + 1 >= tokens.size()) {
                        std::cout << "Falha ao atualizar gasto: valor ausente" << std::endl;
                        erro = true;
                        break;
                    }
                    std::stringstream ssValor(tokens[i + 1]);
                    if (!(ssValor >> valor)) {
                        std::cout << "Falha ao atualizar gasto: valor invalido" << std::endl;
                        erro = true;
                        break;
                    }
                    ++i;
                } else if (tokens[i] == "data") {
                    if (i + 1 >= tokens.size()) {
                        std::cout << "Falha ao atualizar gasto: data ausente" << std::endl;
                        erro = true;
                        break;
                    }
                    int d = 0, m = 0, a = 0;
                    char barra1 = 0, barra2 = 0;
                    std::stringstream ssData(tokens[i + 1]);
                    if (!(ssData >> d >> barra1 >> m >> barra2 >> a)
                        || barra1 != '/' || barra2 != '/') {
                        std::cout << "Falha ao atualizar gasto: data invalida (use dia/mes/ano)" << std::endl;
                        erro = true;
                        break;
                    }
                    dia = d;
                    mes = m;
                    ano = a;
                    ++i;
                } else if (tokens[i] == "descricao" || tokens[i] == "categoria") {
                    std::string valorCampo;
                    size_t j = i + 1;
                    while (j < tokens.size() && tokens[j] != "valor"
                           && tokens[j] != "descricao" && tokens[j] != "data"
                           && tokens[j] != "categoria") {
                        if (!valorCampo.empty()) {
                            valorCampo += " ";
                        }
                        valorCampo += tokens[j];
                        ++j;
                    }
                    if (valorCampo.empty()) {
                        std::cout << "Falha ao atualizar gasto: " << tokens[i] << " ausente" << std::endl;
                        erro = true;
                        break;
                    }
                    if (tokens[i] == "descricao") {
                        descricao = valorCampo;
                    } else {
                        nomeCat = valorCampo;
                    }
                    i = j - 1;
                } else {
                    std::cout << "Falha ao atualizar gasto: campo '" << tokens[i] << "' invalido" << std::endl;
                    erro = true;
                    break;
                }
            }
            if (erro) {
                continue;
            }

            if (carteira.atualizarGasto(id, valor, descricao, dia, mes, ano, nomeCat)) {
                std::cout << "Gasto atualizado com sucesso" << std::endl;
            } else {
                std::cout << "Falha ao atualizar gasto: dados invalidos" << std::endl;
            }
            continue;
        }

        // Comando: atualizar categoria <nome...> [limite <v>] [nome <novoNome>]
        if (cmd1 == "atualizar" && cmd2 == "categoria") {
            std::string prefixoCat = "atualizar categoria ";
            if (linha.size() < prefixoCat.size()) {
                std::cout << "Falha ao atualizar categoria: nome ausente" << std::endl;
                continue;
            }
            std::string restoCat = linha.substr(prefixoCat.size());
            trim(restoCat);

            std::vector<std::string> tokensCat;
            std::stringstream ssCat(restoCat);
            std::string palavraCat;
            while (ssCat >> palavraCat) {
                tokensCat.push_back(palavraCat);
            }

            std::string nomeCat, novoNomeCat;
            size_t iCat = 0;
            for (; iCat < tokensCat.size(); ++iCat) {
                if (tokensCat[iCat] == "limite" || tokensCat[iCat] == "nome") {
                    break;
                }
                if (!nomeCat.empty()) {
                    nomeCat += " ";
                }
                nomeCat += tokensCat[iCat];
            }
            if (nomeCat.empty()) {
                std::cout << "Falha ao atualizar categoria: nome ausente" << std::endl;
                continue;
            }
            if (nomeCat == "sem categoria") {
                std::cout << "Falha ao atualizar categoria: 'sem categoria' nao pode ser alterada" << std::endl;
                continue;
            }

            Categoria catAtual;
            if (!carteira.categoriaPorNome(nomeCat, catAtual)) {
                std::cout << "Falha ao atualizar categoria: nome nao encontrado" << std::endl;
                continue;
            }
            novoNomeCat = catAtual.getNome();
            double novoLimiteCat = catAtual.getLimite();

            bool erroCat = false;
            for (; iCat < tokensCat.size(); ++iCat) {
                if (tokensCat[iCat] == "limite") {
                    if (iCat + 1 >= tokensCat.size()) {
                        std::cout << "Falha ao atualizar categoria: limite ausente" << std::endl;
                        erroCat = true;
                        break;
                    }
                    std::stringstream ssLimite(tokensCat[iCat + 1]);
                    if (!(ssLimite >> novoLimiteCat)) {
                        std::cout << "Falha ao atualizar categoria: limite invalido" << std::endl;
                        erroCat = true;
                        break;
                    }
                    ++iCat;
                } else if (tokensCat[iCat] == "nome") {
                    novoNomeCat.clear();
                    for (size_t j = iCat + 1; j < tokensCat.size(); ++j) {
                        if (!novoNomeCat.empty()) {
                            novoNomeCat += " ";
                        }
                        novoNomeCat += tokensCat[j];
                    }
                    if (novoNomeCat.empty()) {
                        std::cout << "Falha ao atualizar categoria: novo nome ausente" << std::endl;
                        erroCat = true;
                        break;
                    }
                    iCat = tokensCat.size();
                } else {
                    std::cout << "Falha ao atualizar categoria: campo '" << tokensCat[iCat] << "' invalido" << std::endl;
                    erroCat = true;
                    break;
                }
            }
            if (erroCat) {
                continue;
            }
            if (novoLimiteCat < 0.0) {
                std::cout << "Falha ao atualizar categoria: limite invalido" << std::endl;
                continue;
            }
            if (novoNomeCat != nomeCat) {
                Categoria outra;
                if (carteira.categoriaPorNome(novoNomeCat, outra)) {
                    std::cout << "Falha ao atualizar categoria: nome ja existente" << std::endl;
                    continue;
                }
            }

            if (carteira.atualizarCategoria(nomeCat, novoNomeCat, novoLimiteCat)) {
                std::cout << "Categoria atualizada com sucesso" << std::endl;
            } else {
                std::cout << "Falha ao atualizar categoria" << std::endl;
            }
            continue;
        }

        // Comando: deletar categoria <nome...>
        if (cmd1 == "deletar" && cmd2 == "categoria") {
            std::string prefixoDel = "deletar categoria ";
            if (linha.size() < prefixoDel.size()) {
                std::cout << "Falha ao deletar categoria: nome ausente" << std::endl;
                continue;
            }
            std::string nomeDel = linha.substr(prefixoDel.size());
            trim(nomeDel);
            if (nomeDel.empty()) {
                std::cout << "Falha ao deletar categoria: nome ausente" << std::endl;
                continue;
            }
            if (nomeDel == "sem categoria") {
                std::cout << "Falha ao deletar categoria: 'sem categoria' nao pode ser deletada" << std::endl;
                continue;
            }
            if (carteira.deletarCategoria(nomeDel)) {
                std::cout << "Categoria deletada com sucesso" << std::endl;
            } else {
                std::cout << "Falha ao deletar categoria: nome nao encontrado" << std::endl;
            }
            continue;
        }

        // Comando: consultar gastos por categoria no mes <mes> <ano>
        // ou: consultar gastos individuais de uma categoria no mes <categoria> <mes> <ano>
        if (cmd1 == "consultar" && cmd2 == "gastos") {
            std::string restoConsulta = linha;
            std::string prefixo1 = "consultar gastos por categoria no mes ";
            if (restoConsulta.find(prefixo1) == 0) {
                std::string dados = restoConsulta.substr(prefixo1.size());
                trim(dados);
                int mes = 0, ano = 0;
                std::stringstream ssd(dados);
                if (!(ssd >> mes >> ano)) {
                    std::cout << "Falha ao consultar gastos: dados invalidos" << std::endl;
                    continue;
                }
                std::vector<ResumoCategoria> res = carteira.consultarMes(mes, ano);
                for (std::vector<ResumoCategoria>::size_type i = 0; i < res.size(); ++i) {
                    std::cout << "Categoria: " << res[i].categoria
                              << " | Total: R$ " << res[i].total;
                    if (res[i].limite < 0.0) {
                        std::cout << " | Limite: sem limite" << std::endl;
                    } else {
                        std::cout << " | Limite: R$ " << res[i].limite << std::endl;
                    }
                }
                continue;
            }

            std::string prefixo2 = "consultar gastos individuais de uma categoria no mes ";
            if (restoConsulta.find(prefixo2) == 0) {
                std::string dados = restoConsulta.substr(prefixo2.size());
                trim(dados);
                if (dados.empty()) {
                    std::cout << "Falha ao consultar gastos: dados invalidos" << std::endl;
                    continue;
                }
                size_t espUlt = dados.rfind(' ');
                if (espUlt == std::string::npos) {
                    std::cout << "Falha ao consultar gastos: dados invalidos" << std::endl;
                    continue;
                }
                std::string anoStr = dados.substr(espUlt + 1);
                std::string meio = dados.substr(0, espUlt);
                trim(meio);
                trim(anoStr);
                size_t espUlt2 = meio.rfind(' ');
                if (espUlt2 == std::string::npos) {
                    std::cout << "Falha ao consultar gastos: dados invalidos" << std::endl;
                    continue;
                }
                std::string mesStr = meio.substr(espUlt2 + 1);
                std::string nomeCat = meio.substr(0, espUlt2);
                trim(nomeCat);
                trim(mesStr);

                int mes = 0, ano = 0;
                std::stringstream ssm(mesStr);
                std::stringstream ssa(anoStr);
                if (!(ssm >> mes) || !(ssa >> ano)) {
                    std::cout << "Falha ao consultar gastos: dados invalidos" << std::endl;
                    continue;
                }

                DetalheMes dm = carteira.consultarCategoria(nomeCat, mes, ano);
                for (std::vector<Gasto>::size_type i = 0; i < dm.gastos.size(); ++i) {
                    const Gasto &g = dm.gastos[i];
                    std::cout << "id=" << g.getId()
                              << " " << g.getDia() << "/" << g.getMes() << "/" << g.getAno()
                              << " " << g.getDescricao()
                              << " R$ " << g.getValor() << std::endl;
                }
                std::cout << "Total: R$ " << dm.total << std::endl;
                if (dm.idEstouro != -1) {
                    Categoria cCat;
                    if (carteira.categoriaPorNome(nomeCat, cCat)) {
                        std::cout << "(Ultrapassou o limite da categoria de R$ "
                                  << cCat.getLimite() << ",00)" << std::endl;
                    }
                }
                continue;
            }

            std::cout << "Comando desconhecido" << std::endl;
            continue;
        }

        std::cout << "Comando desconhecido" << std::endl;
    }

    return 0;
}
