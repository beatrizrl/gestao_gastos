#ifndef CARTEIRA_HPP
#define CARTEIRA_HPP

#include <list>
#include <map>
#include <string>
#include <vector>
#include "Gasto.hpp"
#include "Categoria.hpp"

struct ResumoCategoria {
    std::string categoria;
    double total;
    double limite;
};

struct DetalheMes {
    std::vector<Gasto> gastos;
    double total;
    int idEstouro;
};

class Carteira {
private:
    std::list<Gasto> gastos;
    std::map<std::string, Categoria> categorias;
    double limiteTotal;
    int proximoIdGasto;
    int proximoIdCategoria;

    int diasDoMes(int mes, int ano) const;

public:
    Carteira();

    bool adicionarCategoria(const std::string &nome, double limite);
    void listarCategorias(std::ostream &out) const;

    bool adicionarGasto(double valor, const std::string &descricao,
                        int dia, int mes, int ano,
                        const std::string &nomeCategoria);
    bool deletarGasto(int id, Gasto &copia);
    bool atualizarGasto(int id, double valor, const std::string &descricao,
                        int dia, int mes, int ano,
                        const std::string &nomeCategoria);
    bool atualizarCategoria(const std::string &nome,
                            const std::string &novoNome, double novoLimite);
    bool deletarCategoria(const std::string &nome);
    void listarGastos(std::ostream &out) const;
    std::string nomeDaCategoria(int id) const;
    bool gastoPorId(int id, Gasto &copia) const;
    bool categoriaPorNome(const std::string &nome, Categoria &copia) const;

    std::vector<ResumoCategoria> consultarMes(int mes, int ano) const;
    DetalheMes consultarCategoria(const std::string& nome, int mes, int ano) const;
};

#endif
