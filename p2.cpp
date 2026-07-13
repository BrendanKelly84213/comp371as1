#include <iostream>
#include <cmath>

class Point {
    public:
        Point(const Point& point) = default; 
        Point(float x, float y, float z) : m_x(x), m_y(y), m_z(z)
        {}
        
        // Not sure what they want us to destruct here
        ~Point() {}

        int translate(int d, char axis) {
            if (axis == 'x') {
                m_x += d;
                return 0;
            }

            if (axis == 'y') {
                m_y += d;
                return 0;
            }

            if (axis == 'z') {
                m_z += d;
                return 0;
            }

            return -1;
        }

        float x() const  { return m_x; }
        float y() const  { return m_y; }
        float z() const  { return m_z; }

    private:
        float m_x;
        float m_y;
        float m_z;
};

inline float square(float a) {
    return a*a;
}

inline float distance(const Point* a, const Point* b) {
    return std::sqrt(square((a->x() - b->x())) + square(a->y() - b->y()) + square(a->z() - b->z()));
}

class Triangle {
    public:
        Triangle(const Point& vertex1, const Point& vertex2, const Point& vertex3) 
            : m_vertex1(new Point(vertex1)), m_vertex2(new Point(vertex2)), m_vertex3(new Point(vertex3))
        {
        }   

        Triangle() 
            : m_vertex1(nullptr), m_vertex2(nullptr), m_vertex3(nullptr)
        {
        }   

        ~Triangle() {
            delete m_vertex1;
            delete m_vertex2;
            delete m_vertex3;
        }

        int translate(int d, char axis) {
            int res = m_vertex1->translate(d, axis); 
            if (res == -1) {
                return res;
            }
            res = m_vertex2->translate(d, axis); 
            if (res == -1) {
                return res;
            }
            return m_vertex3->translate(d, axis); 
        }

        float calcArea() {
            float a = distance(m_vertex1, m_vertex2);
            float b = distance(m_vertex1, m_vertex3);
            float c = distance(m_vertex2, m_vertex3);
            float s = (a + b + c)*0.5f; 
            // herons formula 
            return std::sqrt(s*(s - a)*(s - b)*(s - c));
        }

    private:
        // raw pointers because assignment says we need a destructor implication I'm assuming they want us to do it like this
        Point* m_vertex1;
        Point* m_vertex2;
        Point* m_vertex3;
};
        
int main()
{
    return 0;
}
