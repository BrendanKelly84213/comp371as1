#include <iostream>
#include <print>
#include <format>
#include <cmath>
#include <memory>
#include <vector>

// TODO: Ask if we can use smart pointers and std optional
// TODO: Clarify whats up with destructors
// TODO: Clarify why translate takes an int not a float
// TODO: Clarity whats the point of nullability for points

class Point final
{
public:
    float m_x;
    float m_y;
    float m_z;

    Point() : m_x(0), m_y(0), m_z(0)
    {
    }

    Point(const float x, const float y, const float z) : m_x(x), m_y(y), m_z(z)
    {
    }

    ~Point() = default;

    int translate(const int d, const char axis)
    {
        switch (axis)
        {
        case 'x':
            m_x += d;
            return 0;
        case 'y':
            m_y += d;
            return 0;
        case 'z':
            m_z += d;
            return 0;
        default:
            return -1;
        }
    }

    [[nodiscard]] float distance_to(const Point& other) const
    {
        return std::hypot(
            m_x - other.m_x,
            m_y - other.m_y,
            m_z - other.m_z
        );
    }


    [[nodiscard]] explicit operator std::string() const
    {
        return std::format("Point({}, {}, {})", m_x, m_y, m_z);
    }
};


class Triangle
{
public:
    std::array<std::optional<std::unique_ptr<Point>>, 3> m_vertex;

    Triangle(
        std::unique_ptr<Point> vertex1,
        std::unique_ptr<Point> vertex2,
        std::unique_ptr<Point> vertex3
    )
        : m_vertex({std::move(vertex1), std::move(vertex2), std::move(vertex3)})
    {
    }

    Triangle()
        : m_vertex({std::nullopt, std::nullopt, std::nullopt})
    {
    }

    // ~Triangle() = default;


    int translate(const int d, const char axis)
    {
        ensure_initialized();

        int res = 0;
        res |= m_vertex[0].value()->translate(d, axis);
        res |= m_vertex[1].value()->translate(d, axis);
        res |= m_vertex[2].value()->translate(d, axis);
        return res;
    }

    [[nodiscard]] float calcArea() const
    {
        ensure_initialized();

        const float a = m_vertex[0].value()->distance_to(*m_vertex[1].value());
        const float b = m_vertex[0].value()->distance_to(*m_vertex[2].value());
        const float c = m_vertex[1].value()->distance_to(*m_vertex[2].value());
        const float s = (a + b + c) * 0.5f;

        // heron's formula
        return std::sqrt(s * (s - a) * (s - b) * (s - c));
    }

    [[nodiscard]] explicit operator std::string() const
    {
        return std::format(
            "Triangle(\n\t{},\n\t{},\n\t{}\n1)",
            m_vertex[0].transform([](const std::unique_ptr<Point>& x)
            {
                return static_cast<std::string>(*x);
            }).value_or("nullptr"),
            m_vertex[0].transform([](const std::unique_ptr<Point>& x)
            {
                return static_cast<std::string>(*x);
            }).value_or("nullptr"),
            m_vertex[0].transform([](const std::unique_ptr<Point>& x)
            {
                return static_cast<std::string>(*x);
            }).value_or("nullptr")
        );
    }

    [[nodiscard]] bool is_initialized() const
    {
        return m_vertex[0].has_value() && m_vertex[1].has_value() && m_vertex[2].has_value();
    }

    void ensure_initialized() const
    {
        if (!is_initialized()) throw std::runtime_error("Attempted to use uninitialized Triangle instance");
    }
};

class Driver
{
public:
    void run_main_menu()
    {
        while (true)
        {
            std::println("Triangle Manipulation Menu");
            std::println("Triangles:");
            print_triangles();
            std::println("Commands:");
            std::println("[1] Create triangle");
            std::println("[2] Edit triangle");
            std::println("[3] Delete triangle");
            std::println("[4] Translate triangle");
            std::println("[5] Display triangles");
            std::println();
            std::println("[0] Quit");

            switch (const int option = prompt_int("Select an option: "))
            {
            case 0:
                return;
            case 1:
                m_triangles.emplace_back();
                break;
            case 2:
                {
                    const auto triangle_idx = select_triangle();
                    if (!triangle_idx.has_value()) break;

                    run_edit_menu(m_triangles[triangle_idx.value()]);

                    break;
                }
            case 3:
                {
                    const auto triangle_idx = select_triangle();
                    if (!triangle_idx.has_value()) break;

                    m_triangles.erase(m_triangles.begin() + triangle_idx.value());
                    break;
                }
            case 4:
                //TODO
                break;
            case 5:
                print_triangles();
                break;
            default:
                std::println("Invalid Option ({})!", option);
            }
        }
    }

private:
    std::vector<Triangle> m_triangles;

    void print_triangles()
    {
        for (size_t idx = 0; auto& triangle : m_triangles)
        {
            std::println("[{}] {}", idx, static_cast<std::string>(triangle));
            idx++;
        }
    }

    static int prompt_int(const char* prompt)
    {
        std::print("\n{}", prompt);

        int option;
        std::cin >> option;

        return option;
    }

    std::optional<int> select_triangle()
    {
        while (true)
        {
            const int triangle_idx = prompt_int("Select a triangle (-1 to cancel): ");

            if (triangle_idx == -1) return std::nullopt;

            if (triangle_idx < 0 || triangle_idx >= m_triangles.size())
            {
                std::println("Invalid triangle index");
                continue;
            }

            return triangle_idx;
        }
    }

    void run_edit_menu(Triangle& triangle)
    {
        while (true)
        {
            std::println("Points: ");
            for (size_t idx = 0; auto& point : triangle.m_vertex)
            {
                std::println("[{}] {}", idx, point.transform([](const std::unique_ptr<Point>& x)
                {
                    return static_cast<std::string>(*x);
                }).value_or("nullptr"));
                idx++;
            }

            int point_idx;
            while (true)
            {
                point_idx = prompt_int("Select a point (-1 to cancel): ");
                if (point_idx == -1) return;
                if (point_idx < 0 || point_idx >= 3)
                {
                    std::println("Invalid point number (must be between 0 and 2 inclusively");
                    continue;
                }
                break;
            }

            auto& point = triangle.m_vertex[point_idx];
            const auto x = prompt_int("Enter value for x: ");
            const auto y = prompt_int("Enter value for y: ");
            const auto z = prompt_int("Enter value for z: ");
            *point = std::make_unique<Point>(Point(x, y, z));
        }
    }
};


int main()
{
    auto t = Triangle(
        std::make_unique<Point>(Point(1, 3, 6)),
        std::make_unique<Point>(Point(2, 5, 8)),
        std::make_unique<Point>(Point(4, 7, 9))
    );
    std::println("{}", t.calcArea());
    t.translate(1, 'z');
    std::println("{}", static_cast<std::string>(t));
    std::println("{}", t.calcArea());

    Driver driver;
    driver.run_main_menu();

    return 0;
}
