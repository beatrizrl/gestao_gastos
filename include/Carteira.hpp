#ifndef CARTEIRA_HPP
#define CARTEIRA_HPP

#include <list>
#include <map>
#include <string>
#include <vector>
#include "Gasto.hpp"
#include "Categoria.hpp"

class Carteira {
private:
    std::list<Gasto> gastos;
    std::map<std::string, Categoria> categorias;
    double limiteTotal;
    int proximoIdGasto;
    int proximoIdCategoria;

public:
    Carteira();

    bool adicionarCategoria(const std::string &nome, double limite);
    void listarCategorias(std::ostream &out) const;
};

#endif
