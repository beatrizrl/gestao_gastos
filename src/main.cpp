#include <iostream>
#include <algorithm>
#include <vector>
#include "Gasto.hpp"

int main() {
    std::vector<Gasto> gastos;

    gastos.push_back(Gasto(3, 50.0, "Almoco", 15, 10, 2026, 1));
    gastos.push_back(Gasto(1, 10.0, "Cafe", 10, 9, 2026, 0));
    gastos.push_back(Gasto(4, 100.0, "Jantar", 20, 10, 2026, 1));
    gastos.push_back(Gasto(2, 30.0, "Lanche", 5, 10, 2026, 2));

    std::cout << "Antes da ordenacao:" << std::endl;
    for (std::size_t i = 0; i < gastos.size(); ++i) {
        const Gasto &g = gastos[i];
        std::cout << "id=" << g.getId() << " | "
                  << g.getDia() << "/" << g.getMes() << "/" << g.getAno()
                  << " | valor=" << g.getValor()
                  << " | " << g.getDescricao() << std::endl;
    }

    std::sort(gastos.begin(), gastos.end());

    std::cout << "\nDepois da ordenacao (por data, desempate por id):" << std::endl;
    for (std::size_t i = 0; i < gastos.size(); ++i) {
        const Gasto &g = gastos[i];
        std::cout << "id=" << g.getId() << " | "
                  << g.getDia() << "/" << g.getMes() << "/" << g.getAno()
                  << " | valor=" << g.getValor()
                  << " | " << g.getDescricao() << std::endl;
    }

    Gasto chave;
    chave.setId(3);
    std::vector<Gasto>::iterator it = std::find(gastos.begin(), gastos.end(), chave);
    if (it != gastos.end()) {
        std::cout << "\nGasto encontrado por id=3:" << std::endl;
        std::cout << "id=" << it->getId() << " | "
                  << it->getDia() << "/" << it->getMes() << "/" << it->getAno()
                  << " | valor=" << it->getValor()
                  << " | " << it->getDescricao() << std::endl;
    } else {
        std::cout << "\nGasto com id=3 nao encontrado" << std::endl;
    }

    return 0;
}
