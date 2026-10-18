#ifndef CATEGORIA_HPP
#define CATEGORIA_HPP

#include <string>

class Categoria {
private:
    int id;
    std::string nome;
    double limite;

public:
    Categoria();
    Categoria(int id, const std::string &nome, double limite);

    int getId() const;
    const std::string &getNome() const;
    double getLimite() const;

    void setId(int id);
    void setNome(const std::string &nome);
    void setLimite(double limite);
};

#endif
