#ifndef GASTO_HPP
#define GASTO_HPP

#include <string>

class Gasto {
private:
    int id;
    double valor;
    std::string descricao;
    int dia;
    int mes;
    int ano;
    int idCategoria;

public:
    Gasto();
    Gasto(int id, double valor, const std::string &descricao,
          int dia, int mes, int ano, int idCategoria);

    int getId() const;
    double getValor() const;
    const std::string &getDescricao() const;
    int getDia() const;
    int getMes() const;
    int getAno() const;
    int getIdCategoria() const;

    void setId(int id);
    void setValor(double valor);
    void setDescricao(const std::string &descricao);
    void setDia(int dia);
    void setMes(int mes);
    void setAno(int ano);
    void setIdCategoria(int idCategoria);

    bool operator<(const Gasto &outro) const;
    bool operator==(const Gasto &outro) const;
};

#endif
