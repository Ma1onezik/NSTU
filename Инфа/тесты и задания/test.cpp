#include <iostream>
#include <typeinfo>

class Shape {
public:
    virtual ~Shape() {}  // Виртуальный деструктор для полиморфизма
    virtual void draw() = 0;
};

class Circle : public Shape {
public:
    void draw() override { std::cout << "Drawing Circle" << std::endl; }
};

class Square : public Shape {
public:
    void draw() override { std::cout << "Drawing Square" << std::endl; }
};

int main() {
    Shape* shapes[] = { new Circle(), new Square(), new Circle() };
    
    // Использование typeid
    for (int i = 0; i < 3; i++) {
        std::cout << "Shape " << i << " type: " 
                  << typeid(*shapes[i]).name() << std::endl;
        
        // Сравнение типов
        if (typeid(*shapes[i]) == typeid(Circle)) {
            std::cout << "  Это круг!" << std::endl;
        } else if (typeid(*shapes[i]) == typeid(Square)) {
            std::cout << "  Это квадрат!" << std::endl;
        }
        
        // Использование dynamic_cast для проверки типа
        if (Circle* c = dynamic_cast<Circle*>(shapes[i])) {
            std::cout << "  dynamic_cast подтвердил: это круг" << std::endl;
            c->draw();
        }
    }
    
    // Получение информации о типе
    const std::type_info& ti = typeid(Circle);
    std::cout << "\nИмя типа Circle: " << ti.name() << std::endl;
    
    // Сравнение type_info объектов
    if (ti == typeid(Circle)) {
        std::cout << "Типы совпадают" << std::endl;
    }
    
    // Очистка
    for (auto s : shapes) delete s;
    
    return 0;
}