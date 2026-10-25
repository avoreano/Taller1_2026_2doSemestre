#ifndef DECISION_TREE_HPP
#define DECISION_TREE_HPP

#include <string>
#include "DecisionNode.hpp"
#include "Patient.hpp"

class DecisionTree {
private:
    DecisionNode* root;

    DecisionNode* leaf(const std::string& service);
    DecisionNode* question(const std::string& attribute, const std::string& op, double threshold,
                           const std::string& yesLabel, const std::string& noLabel,
                           DecisionNode* yes, DecisionNode* no);
    DecisionNode* painTypeQuestion(const std::string& painType,
                                   const std::string& yesLabel, const std::string& noLabel,
                                   DecisionNode* yes, DecisionNode* no);

    double getValue(const DecisionNode* node, const Patient* patient) const;
    bool check(const DecisionNode* node, const Patient* patient) const;

    void showNode(const DecisionNode* node) const;
    void preOrder(const DecisionNode* node) const;
    void inOrder(const DecisionNode* node) const;
    void postOrder(const DecisionNode* node) const;
    void destroy(DecisionNode* node);

public:
    DecisionTree();
    ~DecisionTree();

    // arma el arbol completo (se llama en el constructor)
    void build();

    // devuelve el servicio y deja en route el camino recorrido.
    // si el paciente es nullptr o el arbol esta vacio devuelve "" y route queda vacio.
    std::string evaluate(const Patient* patient, std::string& route) const;
    std::string evaluate(const Patient* patient) const;

    void showPreOrder() const;
    void showInOrder() const;
    void showPostOrder() const;
};

#endif