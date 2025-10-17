#include <iostream>

template <typename T>
class Campo2D {
private:
    T **_datos;
    int _filas;
    int _columnas;

    void liberarMemoria() {
        if (_datos != nullptr) {
            for (int i = 0; i < _filas; ++i) {
                delete[] _datos[i];
            }
            delete[] _datos;
            _datos = nullptr;
        }
    }

public:
    Campo2D(int f, int c) {
        if (f <= 0 || c <= 0) {
            std::cerr << "Error: Las dimensiones deben ser positivas. Objeto no creado correctamente." << std::endl;
            _datos = nullptr;
            _filas = 0;
            _columnas = 0;
            return;
        }
        
        std::cout << "Creando Grid de " << f << "x" << c << "..." << std::endl;
        _filas = f;
        _columnas = c;
        _datos = new T*[_filas];
        for (int i = 0; i < _filas; ++i) {
            _datos[i] = new T[_columnas]();
        }
    }

    ~Campo2D() {
        std::cout << "\nDestructor invocado. Liberando memoria de la Matriz 2D..." << std::endl;
        liberarMemoria();
    }

    void setValor(int f, int c, T valor) {
        if (f >= 0 && f < _filas && c >= 0 && c < _columnas) {
            _datos[f][c] = valor;
        } else {
            std::cerr << "Error: Índices [" << f << "][" << c << "] fuera de rango." << std::endl;
        }
    }
    
    T getValor(int f, int c) const {
        if (f >= 0 && f < _filas && c >= 0 && c < _columnas) {
            return _datos[f][c];
        }
        std::cerr << "Error: Índices [" << f << "][" << c << "] fuera de rango. Retornando valor por defecto." << std::endl;
        return T();
    }

    void redimensionar(int nuevaF, int nuevaC) {
        if (nuevaF <= 0 || nuevaC <= 0) {
            std::cerr << "Error: Las nuevas dimensiones deben ser positivas." << std::endl;
            return;
        }
        std::cout << "\nRedimensionando Grid a " << nuevaF << "x" << nuevaC << "..." << std::endl;

        T **nuevosDatos = new T*[nuevaF];
        for (int i = 0; i < nuevaF; ++i) {
            nuevosDatos[i] = new T[nuevaC]();
        }

        int filasACopiar = (_filas < nuevaF) ? _filas : nuevaF;
        int columnasACopiar = (_columnas < nuevaC) ? _columnas : nuevaC;

        for (int i = 0; i < filasACopiar; ++i) {
            for (int j = 0; j < columnasACopiar; ++j) {
                nuevosDatos[i][j] = _datos[i][j];
            }
        }

        liberarMemoria();

        _datos = nuevosDatos;
        _filas = nuevaF;
        _columnas = nuevaC;
        std::cout << "Datos copiados. Memoria antigua liberada." << std::endl;
    }

    T calcularGradientePromedio(int inicioF, int finF, int inicioC, int finC) {
        if (inicioF < 0 || finF > _filas || inicioC < 0 || finC > _columnas || inicioF >= finF || inicioC >= finC) {
            std::cerr << "Error: Región de submatriz inválida." << std::endl;
            return T();
        }

        std::cout << "\nCalculando Gradiente Promedio en la región..." << std::endl;

        double sumaGradientes = 0.0;
        int puntosCalculados = 0;

        for (int i = inicioF; i < finF - 1; ++i) {
            for (int j = inicioC; j < finC - 1; ++j) {
                double diffH = static_cast<double>(_datos[i][j+1] - _datos[i][j]);
                double gradH = (diffH < 0) ? -diffH : diffH;

                double diffV = static_cast<double>(_datos[i+1][j] - _datos[i][j]);
                double gradV = (diffV < 0) ? -diffV : diffV;
                
                sumaGradientes += (gradH + gradV) / 2.0;
                puntosCalculados++;
            }
        }
        
        if (puntosCalculados == 0) {
            std::cout << "No se pudo calcular el gradiente (región muy pequeña)." << std::endl;
            return T();
        }

        return static_cast<T>(sumaGradientes / puntosCalculados);
    }

    void mostrarGrid() const {
        if (!_datos) {
            std::cout << "El grid no está inicializado o es inválido." << std::endl;
            return;
        }
        for (int i = 0; i < _filas; ++i) {
            std::cout << "| ";
            for (int j = 0; j < _columnas; ++j) {
                std::cout << _datos[i][j] << " | ";
            }
            std::cout << std::endl;
        }
    }
};


int main() {
    std::cout << "--- Simulador Genérico de Campo 2D ---\n" << std::endl;
---
    std::cout << ">> Inicializando Campo Gravitatorio (Tipo FLOAT) <<" << std::endl;
    Campo2D<float> campoFloat(3, 3);
    
    std::cout << "\nEstableciendo valores iniciales..." << std::endl;
    campoFloat.setValor(0, 0, 10.0f);
    campoFloat.setValor(0, 1, 8.0f);
    campoFloat.setValor(0, 2, 5.0f);
    campoFloat.setValor(1, 0, 12.0f);
    campoFloat.setValor(1, 1, 9.0f);
    campoFloat.setValor(1, 2, 6.0f);
    campoFloat.setValor(2, 0, 15.0f);
    campoFloat.setValor(2, 1, 11.0f);
    campoFloat.setValor(2, 2, 7.0f);

    std::cout << "\nGrid Actual (Paso 0):" << std::endl;
    campoFloat.mostrarGrid();

    float gradiente = campoFloat.calcularGradientePromedio(0, 3, 0, 3);
    std::cout << "Gradiente Promedio calculado: " << gradiente << " unidades/metro." << std::endl;
    
    campoFloat.redimensionar(4, 4);
    std::cout << "\nGrid después de redimensionar a 4x4:" << std::endl;
    campoFloat.setValor(3, 3, 99.9f); 
    campoFloat.mostrarGrid();

    campoFloat.redimensionar(2, 2);
    std::cout << "\nGrid después de redimensionar a 2x2:" << std::endl;
    campoFloat.mostrarGrid();

    std::cout << "\n-------------------------------------------------\n" << std::endl;

    std::cout << ">> Inicializando Campo de Potencial (Tipo INT) <<" << std::endl;
    Campo2D<int> campoInt(2, 4);
    
    std::cout << "\nEstableciendo valores iniciales..." << std::endl;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 4; ++j) {
            campoInt.setValor(i, j, (i + 1) * 10 + j);
        }
    }

    std::cout << "\nGrid Actual (Enteros):" << std::endl;
    campoInt.mostrarGrid();

    int gradienteInt = campoInt.calcularGradientePromedio(0, 2, 0, 4);
    std::cout << "Gradiente Promedio calculado: " << gradienteInt << " unidades." << std::endl;
    
    std::cout << "\n-------------------------------------------------" << std::endl;

    std::cout << "\nOpción: Salir" << std::endl;
    std::cout << "El programa terminará y los destructores se llamarán automáticamente." << std::endl;
    
    return 0;
}