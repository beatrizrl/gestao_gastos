#include "Carteira.hpp"
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
