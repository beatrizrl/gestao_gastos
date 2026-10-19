#include "Carteira.hpp"
#include <algorithm>
#include <ostream>
#include <string>

Carteira::Carteira() {
    gastos.clear();
    categorias.clear();
    limiteTotal = 0.0;
    proximoIdGasto = 1;
    proximoIdCategoria = 1;

    // Cria a categoria "sem categoria" com id fixo 0 e sem limite (regra)
    Categoria semCategoria(0, "sem categoria", 0.0);
    categorias["sem categoria"] = semCategoria;
}

bool Carteira::adicionarCategoria(const std::string &nome, double limite) {
    if (limite < 0.0) {
        return false;
    }

    // Verifica se já existe categoria com esse nome
    std::map<std::string, Categoria>::iterator it = categorias.find(nome);
    if (it != categorias.end()) {
        return false;
    }

    Categoria novaCategoria(proximoIdCategoria, nome, limite);
    categorias[nome] = novaCategoria;
    ++proximoIdCategoria;
    return true;
}

void Carteira::listarCategorias(std::ostream &out) const {
    out << "Categorias:" << std::endl;
    for (std::map<std::string, Categoria>::const_iterator it = categorias.begin();
         it != categorias.end(); ++it) {
        const std::string &nomeCat = it->first;
        const Categoria &cat = it->second;
        if (nomeCat == "sem categoria") {
            out << nomeCat << " (id=" << cat.getId() << ") - sem limite" << std::endl;
        } else {
            out << nomeCat << " (id=" << cat.getId() << ") - limite R$ "
                << cat.getLimite() << std::endl;
        }
    }
}

int Carteira::diasDoMes(int mes, int ano) const {
    if (mes == 2) {
        if (ano % 4 == 0) {
            return 29;
        }
        return 28;
    }
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        return 30;
    }
    return 31;
}

bool Carteira::adicionarGasto(double valor, const std::string &descricao,
                              int dia, int mes, int ano,
                              const std::string &nomeCategoria) {
    if (valor <= 0.0) {
        return false;
    }
    if (descricao.empty()) {
        return false;
    }
    if (mes < 1 || mes > 12) {
        return false;
    }
    if (dia < 1 || dia > diasDoMes(mes, ano)) {
        return false;
    }

    int idCategoria = 0;
    std::map<std::string, Categoria>::const_iterator it =
        categorias.find(nomeCategoria);
    if (it != categorias.end()) {
        idCategoria = it->second.getId();
    }

    Gasto novo(proximoIdGasto, valor, descricao, dia, mes, ano, idCategoria);
    gastos.push_back(novo);
    ++proximoIdGasto;
    return true;
}

bool Carteira::deletarGasto(int id, Gasto &copia) {
    Gasto chave;
    chave.setId(id);

    std::list<Gasto>::iterator it = std::find(gastos.begin(), gastos.end(), chave);
    if (it == gastos.end()) {
        return false;
    }

    copia = *it;
    gastos.erase(it);
    return true;
}

bool Carteira::atualizarGasto(int id, double valor, const std::string &descricao,
                              int dia, int mes, int ano,
                              const std::string &nomeCategoria) {
    Gasto chave;
    chave.setId(id);

    std::list<Gasto>::iterator it = std::find(gastos.begin(), gastos.end(), chave);
    if (it == gastos.end()) {
        return false;
    }
    if (valor <= 0.0) {
        return false;
    }
    if (descricao.empty()) {
        return false;
    }
    if (mes < 1 || mes > 12) {
        return false;
    }
    if (dia < 1 || dia > diasDoMes(mes, ano)) {
        return false;
    }

    int idCategoria = 0;
    std::map<std::string, Categoria>::const_iterator itCat =
        categorias.find(nomeCategoria);
    if (itCat != categorias.end()) {
        idCategoria = itCat->second.getId();
    }

    it->setValor(valor);
    it->setDescricao(descricao);
    it->setDia(dia);
    it->setMes(mes);
    it->setAno(ano);
    it->setIdCategoria(idCategoria);
    return true;
}

bool Carteira::atualizarCategoria(const std::string &nome,
                                  const std::string &novoNome, double novoLimite) {
    if (nome == "sem categoria") {
        return false;
    }
    if (novoLimite < 0.0) {
        return false;
    }

    std::map<std::string, Categoria>::iterator it = categorias.find(nome);
    if (it == categorias.end()) {
        return false;
    }

    if (novoNome == nome) {
        it->second.setLimite(novoLimite);
        return true;
    }

    if (categorias.find(novoNome) != categorias.end()) {
        return false;
    }

    Categoria atualizada = it->second;
    categorias.erase(it);
    categorias[novoNome] = atualizada;
    categorias[novoNome].setNome(novoNome);
    categorias[novoNome].setLimite(novoLimite);
    return true;
}

bool Carteira::deletarCategoria(const std::string &nome) {
    if (nome == "sem categoria") {
        return false;
    }

    std::map<std::string, Categoria>::iterator it = categorias.find(nome);
    if (it == categorias.end()) {
        return false;
    }

    int idApagada = it->second.getId();
    categorias.erase(it);

    for (std::list<Gasto>::iterator g = gastos.begin(); g != gastos.end(); ++g) {
        if (g->getIdCategoria() == idApagada) {
            g->setIdCategoria(0);
        }
    }
    return true;
}

void Carteira::listarGastos(std::ostream &out) const {
    if (gastos.empty()) {
        out << "Nenhum gasto registrado" << std::endl;
        return;
    }
    out << "Gastos:" << std::endl;
    for (std::list<Gasto>::const_iterator it = gastos.begin();
         it != gastos.end(); ++it) {
        out << "id=" << it->getId()
            << " " << it->getDia() << "/" << it->getMes() << "/" << it->getAno()
            << " " << it->getDescricao()
            << " R$ " << it->getValor()
            << " categoria: " << nomeDaCategoria(it->getIdCategoria())
            << std::endl;
    }
}

std::string Carteira::nomeDaCategoria(int id) const {
    for (std::map<std::string, Categoria>::const_iterator it = categorias.begin();
         it != categorias.end(); ++it) {
        if (it->second.getId() == id) {
            return it->first;
        }
    }
    return "desconhecida";
}

bool Carteira::gastoPorId(int id, Gasto &copia) const {
    Gasto chave;
    chave.setId(id);

    std::list<Gasto>::const_iterator it =
        std::find(gastos.begin(), gastos.end(), chave);
    if (it == gastos.end()) {
        return false;
    }

    copia = *it;
    return true;
}

bool Carteira::categoriaPorNome(const std::string &nome, Categoria &copia) const {
    std::map<std::string, Categoria>::const_iterator it = categorias.find(nome);
    if (it == categorias.end()) {
        return false;
    }

    copia = it->second;
    return true;
}
