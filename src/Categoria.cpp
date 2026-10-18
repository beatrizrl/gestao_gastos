#include "Categoria.hpp"

Categoria::Categoria() {
    id = 0;
    nome = "sem categoria";
    limite = 0.0;
}

Categoria::Categoria(int id, const std::string &nome, double limite) {
    this->id = id;
    this->nome = nome;
    this->limite = limite;
}

int Categoria::getId() const {
    return id;
}

const std::string &Categoria::getNome() const {
    return nome;
}

double Categoria::getLimite() const {
    return limite;
}

void Categoria::setId(int id) {
    this->id = id;
}

void Categoria::setNome(const std::string &nome) {
    this->nome = nome;
}

void Categoria::setLimite(double limite) {
    this->limite = limite;
}
