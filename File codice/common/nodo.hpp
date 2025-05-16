#include <string>

//INCLUDE INFORMAZIONI ASSOCIATE A UN SINGOLO CONTATTO
struct Info{
    public:
        std::string nome;
        std::string città;
        std::string numero;
        std::string email;
        std::string età;
        Info(std::string nome, std::string città, std::string numero, std::string email, std::string età) : nome(nome), città(città), numero(numero), email(email), età(età) {}
};

struct Nodo {
    public:
        Info info;
        Nodo* prossimo;

        Nodo(Info info) : info(info), prossimo(nullptr) {}
};
