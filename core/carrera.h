#ifndef CARRERA_H
#define CARRERA_H

#include <string>
using namespace std;

class Carrera
{
private:
    int id;
    string nombre;

public:
    Carrera(int id,string nombre);

    int getId() const;
    string getNombre() const;
};

#endif // CARRERA_H
