#ifndef DECISION_NODE_HPP
#define DECISION_NODE_HPP

#include <string>

struct DecisionNode {
    bool isLeaf;

    // solo para preguntas
    std::string attribute;      // Edad, Temperatura, Presion, Dolor, TipoDolor, Trauma,
                                // SintomasNeurologicos, RequiereCirugia, DiasHospitalizacion
    std::string op;             // operaciones "<", ">=" o "=="
    double threshold;
    std::string textValue;      // se usa solo con TipoDolor (pecho, cabeza, etc.)
    std::string yesLabel;       // texto de la ruta si la respuesta es si, ej: "Edad < 14"
    std::string noLabel;        // texto de la ruta si la respuesta es no, ej: "Edad >= 14"
    DecisionNode* yes;
    DecisionNode* no;

    // solo para hojas
    std::string service;

    // hoja: termina en un servicio
    DecisionNode(const std::string& service) {
        this->isLeaf = true;
        this->service = service;
        this->attribute = "";
        this->op = "";
        this->threshold = 0;
        this->textValue = "";
        this->yesLabel = "";
        this->noLabel = "";
        this->yes = nullptr;
        this->no = nullptr;
    }

    // pregunta: compara un atributo y se divide en si / no
    DecisionNode(const std::string& attribute, const std::string& op, double threshold,
                 const std::string& textValue,
                 const std::string& yesLabel, const std::string& noLabel) {
        this->isLeaf = false;
        this->service = "";
        this->attribute = attribute;
        this->op = op;
        this->threshold = threshold;
        this->textValue = textValue;
        this->yesLabel = yesLabel;
        this->noLabel = noLabel;
        this->yes = nullptr;
        this->no = nullptr;
    }
};

#endif