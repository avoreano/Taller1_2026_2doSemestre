#include "DecisionTree.hpp"
#include <iostream>

using namespace std;

DecisionTree::DecisionTree() {
    this->root = nullptr;
    build();
}

DecisionTree::~DecisionTree() {
    destroy(root);
}

DecisionNode* DecisionTree::leaf(const string& service) {
    return new DecisionNode(service);
}

DecisionNode* DecisionTree::question(const string& attribute, const string& op, double threshold,
                                     const string& yesLabel, const string& noLabel,
                                     DecisionNode* yes, DecisionNode* no) {
    DecisionNode* node = new DecisionNode(attribute, op, threshold, "", yesLabel, noLabel);
    node->yes = yes;
    node->no = no;
    return node;
}

DecisionNode* DecisionTree::painTypeQuestion(const string& painType,
                                             const string& yesLabel, const string& noLabel,
                                             DecisionNode* yes, DecisionNode* no) {
    DecisionNode* node = new DecisionNode("TipoDolor", "==", 0, painType, yesLabel, noLabel);
    node->yes = yes;
    node->no = no;
    return node;
}

void DecisionTree::build() {
    destroy(root);

    // las preguntas se evaluan en este orden, asi que el orden decide los empates:
    // 1 edad, 2 trauma, 3 neurologico, 4 dolor de pecho, 5 signos vitales, 6 dias de hospitalizacion
    root =
        question("Edad", "<", 14, "Edad < 14", "Edad >= 14",
            leaf("Pediatria"),
            question("Trauma", "==", 1, "Trauma == 1", "Trauma == 0",
                question("RequiereCirugia", "==", 1,
                         "RequiereCirugia == 1", "RequiereCirugia == 0",
                    leaf("Cirugia"),
                    leaf("Traumatologia")),
                question("SintomasNeurologicos", "==", 1,
                         "SintomasNeurologicos == 1", "SintomasNeurologicos == 0",
                    leaf("Neurologia"),
                    painTypeQuestion("pecho", "TipoDolor == pecho", "TipoDolor != pecho",
                        // dolor de pecho: si la presion es critica va a urgencias, si no a cardiologia
                        question("Presion", ">=", 180,
                                 "Presion >= 180", "Presion < 180",
                            leaf("Urgencias"),
                            question("Presion", "<", 90,
                                     "Presion < 90", "Presion >= 90",
                                leaf("Urgencias"),
                                leaf("Cardiologia"))),
                        // sin dolor de pecho: fiebre, dolor fuerte o hipotension van a urgencias
                        question("Temperatura", ">=", 38.5,
                                 "Temperatura >= 38.5", "Temperatura < 38.5",
                            leaf("Urgencias"),
                            question("Dolor", ">=", 8,
                                     "Dolor >= 8", "Dolor < 8",
                                leaf("Urgencias"),
                                question("Presion", "<", 90,
                                         "Presion < 90", "Presion >= 90",
                                    leaf("Urgencias"),
                                    question("DiasHospitalizacion", ">=", 2,
                                             "DiasHospitalizacion >= 2", "DiasHospitalizacion < 2",
                                        leaf("Hospitalizacion"),
                                        leaf("Medicina General")))))))));
}

// entrega el valor numerico del atributo (los si/no se leen como 1 o 0)
double DecisionTree::getValue(const DecisionNode* node, const Patient* patient) const {
    if (node->attribute == "Edad") return patient->getAge();
    if (node->attribute == "Temperatura") return patient->getTemperature();
    if (node->attribute == "Presion") return patient->getBloodPressure();
    if (node->attribute == "Dolor") return patient->getPain();
    if (node->attribute == "Trauma") return patient->hasTrauma() ? 1 : 0;
    if (node->attribute == "SintomasNeurologicos") return patient->hasNeuroSymptoms() ? 1 : 0;
    if (node->attribute == "RequiereCirugia") return patient->getRequiresSurgery() ? 1 : 0;
    if (node->attribute == "DiasHospitalizacion") return patient->getHospitalDays();
    return 0;
}

// responde la pregunta del nodo: true = camino si, false = camino no
bool DecisionTree::check(const DecisionNode* node, const Patient* patient) const {
    if (node->attribute == "TipoDolor") {
        return patient->getPainType() == node->textValue;
    }

    double value = getValue(node, patient);

    if (node->op == "<") return value < node->threshold;
    if (node->op == ">=") return value >= node->threshold;
    if (node->op == "==") return value == node->threshold;
    return false;
}

string DecisionTree::evaluate(const Patient* patient, string& route) const {
    route = "";

    if (patient == nullptr || root == nullptr) {
        return "";
    }

    const DecisionNode* current = root;

    while (!current->isLeaf) {
        if (check(current, patient)) {
            route += current->yesLabel;
            current = current->yes;
        } else {
            route += current->noLabel;
            current = current->no;
        }
        route += " -> ";
    }

    // se saca el " -> " que sobra al final
    route = route.substr(0, route.size() - 4);
    return current->service;
}

string DecisionTree::evaluate(const Patient* patient) const {
    string route;
    return evaluate(patient, route);
}

void DecisionTree::showNode(const DecisionNode* node) const {
    if (node->isLeaf) {
        cout << "[Servicio: " << node->service << "]" << endl;
    } else {
        cout << node->yesLabel << "?" << endl;
    }
}

void DecisionTree::preOrder(const DecisionNode* node) const {
    if (node == nullptr) {
        return;
    }
    showNode(node);
    preOrder(node->yes);
    preOrder(node->no);
}

void DecisionTree::inOrder(const DecisionNode* node) const {
    if (node == nullptr) {
        return;
    }
    inOrder(node->yes);
    showNode(node);
    inOrder(node->no);
}

void DecisionTree::postOrder(const DecisionNode* node) const {
    if (node == nullptr) {
        return;
    }
    postOrder(node->yes);
    postOrder(node->no);
    showNode(node);
}

void DecisionTree::showPreOrder() const {
    cout << "=== ARBOL DE DECISION (PREORDEN) ===" << endl;
    preOrder(root);
}

void DecisionTree::showInOrder() const {
    cout << "=== ARBOL DE DECISION (INORDEN) ===" << endl;
    inOrder(root);
}

void DecisionTree::showPostOrder() const {
    cout << "=== ARBOL DE DECISION (POSTORDEN) ===" << endl;
    postOrder(root);
}

void DecisionTree::destroy(DecisionNode* node) {
    if (node == nullptr) {
        return;
    }
    destroy(node->yes);
    destroy(node->no);
    delete node;
}