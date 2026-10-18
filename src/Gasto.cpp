#include "Gasto.hpp"

Gasto::Gasto() {
    id = 0;
    valor = 0.0;
    descricao = "";
    dia = 1;
    mes = 1;
    ano = 2000;
    idCategoria = 0;
}

Gasto::Gasto(int id, double valor, const std::string &descricao,
             int dia, int mes, int ano, int idCategoria) {
    this->id = id;
    this->valor = valor;
    this->descricao = descricao;
    this->dia = dia;
    this->mes = mes;
    this->ano = ano;
    this->idCategoria = idCategoria;
}

int Gasto::getId() const {
    return id;
}

double Gasto::getValor() const {
    return valor;
}

const std::string &Gasto::getDescricao() const {
    return descricao;
}

int Gasto::getDia() const {
    return dia;
}

int Gasto::getMes() const {
    return mes;
}

int Gasto::getAno() const {
    return ano;
}

int Gasto::getIdCategoria() const {
    return idCategoria;
}

void Gasto::setId(int id) {
    this->id = id;
}

void Gasto::setValor(double valor) {
    this->valor = valor;
}

void Gasto::setDescricao(const std::string &descricao) {
    this->descricao = descricao;
}

void Gasto::setDia(int dia) {
    this->dia = dia;
}

void Gasto::setMes(int mes) {
    this->mes = mes;
}

void Gasto::setAno(int ano) {
    this->ano = ano;
}

void Gasto::setIdCategoria(int idCategoria) {
    this->idCategoria = idCategoria;
}

bool Gasto::operator<(const Gasto &outro) const {
    if (ano != outro.ano) {
        return ano < outro.ano;
    }
    if (mes != outro.mes) {
        return mes < outro.mes;
    }
    if (dia != outro.dia) {
        return dia < outro.dia;
    }
    return id < outro.id;
}

bool Gasto::operator==(const Gasto &outro) const {
    return id == outro.id;
}
