#include "Service.hpp"
#include <iostream>
#include <cctype>

using namespace std;

namespace {
    string toLower(string text) {
        for (size_t i = 0; i < text.size(); i++) {
            text[i] = static_cast<char>(tolower(static_cast<unsigned char>(text[i])));
        }
        return text;
    }
}

Service::Service(const string& name) {
    this->serviceName = name;
}

Service::~Service() {
    // el servicio es el dueño de los pacientes: se borran antes de que el avl libere sus nodos
    patientsTree.inOrder([](AVLNode<Patient*>* node) {
        delete node->data;
    });
}

string Service::getName() const {
    return serviceName;
}

bool Service::receivePatient(Patient* patient) {
    return patientsTree.insert(patient->getId(), patient);
}

void Service::showStatus() const {
    cout << serviceName << endl;

    if (patientsTree.isEmpty()) {
        cout << "Sin pacientes" << endl;
        return;
    }

    cout << "Pacientes: ";
    bool first = true;
    patientsTree.inOrder([&first](AVLNode<Patient*>* node) {
        if (!first) {
            cout << ", ";
        }
        cout << node->data->getName() << " (" << node->key << ")";
        first = false;
    });
    cout << endl;
}

void Service::showDetails() const {
    cout << "=== AVL DE " << serviceName << " ===" << endl;

    if (patientsTree.isEmpty()) {
        cout << "Sin pacientes" << endl;
        return;
    }

    cout << "Pacientes: " << patientsTree.getSize() << endl;
    cout << "Altura del arbol: " << patientsTree.getHeight() << endl;
    cout << "Factor de balance de la raiz: "
         << patientsTree.getBalanceFactor(patientsTree.getRoot()) << endl;

    // cada nodo se muestra como: id nombre [factor de balance]
    auto showNode = [this](AVLNode<Patient*>* node) {
        cout << node->key << " " << node->data->getName()
             << " [fb=" << patientsTree.getBalanceFactor(node) << "]  ";
    };

    cout << "Inorden:   ";
    patientsTree.inOrder(showNode);
    cout << endl;

    cout << "Preorden:  ";
    patientsTree.preOrder(showNode);
    cout << endl;

    cout << "Postorden: ";
    patientsTree.postOrder(showNode);
    cout << endl;
}

int Service::getPatientCount() const {
    return patientsTree.getSize();
}

bool Service::hasPatientId(int id) const {
    return patientsTree.search(id) != nullptr;
}

Patient* Service::searchPatientById(int id, string& route) const {
    route = "";
    AVLNode<Patient*>* node = patientsTree.search(id, [&route](int key) {
        if (!route.empty()) {
            route += " -> ";
        }
        route += to_string(key);
    });

    if (node == nullptr) {
        return nullptr;
    }
    return node->data;
}

Patient* Service::searchPatientByName(const string& name) const {
    string objective = toLower(name);
    AVLNode<Patient*>* node = patientsTree.findIf([&objective](Patient* patient) {
        return toLower(patient->getName()) == objective;
    });

    if (node == nullptr) {
        return nullptr;
    }
    return node->data;
}