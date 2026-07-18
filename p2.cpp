#include <iostream>
#include <print>
#include <format>
#include <cmath>
#include <memory>
#include <vector>

class Point final
{
public:
    //It does not violate encapsulation to keep these members public as there's no other internal state depending on these values.
    /**
     * X coordinate
     */
    float m_x;
    /**
     * Y coordinate
     */
    float m_y;
    /**
     * Z coordinate
     */
    float m_z;


    /**
     * Point constructor
     * @param x The x coordinate
     * @param y The y coordinate
     * @param z The z coordinate
     */
    Point(const float x, const float y, const float z) : m_x(x), m_y(y), m_z(z)
    {
    }

    ~Point()
    {
        // The Point class doesn't have any heap allocated resources, so there's nothing to do here.
        std::println("Destroyed point");
    }

    // All fields are trivially copyable, so we can use the default copy constructor and copy assignment operator implementation
    Point(const Point& other) = default;

    Point& operator=(const Point& other) = default;

    /**
     * Translate the point by #d units alongside the #axis direction
     * @param d Distance to translate by
     * @param axis Direction (x, y, or z) to translate in
     * @return 0 if successful, -1 if an error occurred
     */
    int translate(const float d, const char axis)
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

    /**
     * Computer the distance to another point
     * @param other Point to compare to
     * @return The distance in units from #this to #other
     */
    [[nodiscard]] float distance_to(const Point& other) const
    {
        return std::hypot(
            other.m_x - m_x,
            other.m_y - m_y,
            other.m_z - m_z
        );
    }


    /**
     * Explicit cast to string for display purposes
     */
    [[nodiscard]] explicit operator std::string() const
    {
        return std::format("Point({}, {}, {})", m_x, m_y, m_z);
    }
};


class Triangle
{
public:
    /**
     * Array of length 3 containing the vertices of this triangle
     */
    std::array<Point*, 3> m_vertex;

    Triangle(
        Point* vertex1,
        Point* vertex2,
        Point* vertex3
    )
        : m_vertex({vertex1, vertex2, vertex3})
    {
    }

    // The default constructor will set all vertices to nullptr
    Triangle() = default;

    ~Triangle()
    {
        std::println("Destroyed Triangle");
        // Delete all owned data
        for (const auto vertex : m_vertex)
            delete vertex;
    }

    // Copy constructor, needed to be a valid vector element without breaking memory safety
    // We just copy every heap-allocated point and save it in the new instance
    Triangle(const Triangle& other) : m_vertex({
        other.m_vertex[0] == nullptr ? nullptr : new Point(*other.m_vertex[0]),
        other.m_vertex[1] == nullptr ? nullptr : new Point(*other.m_vertex[1]),
        other.m_vertex[2] == nullptr ? nullptr : new Point(*other.m_vertex[2])
    })
    {
    }

    // Copy assignment operator; Also needed to be a valid vector element. Replaces an existing instance with the contents
    // of another
    Triangle& operator=(const Triangle& other)
    {
        // Self-assignment guard
        if (this == &other) return *this;

        // Delete old data
        for (const auto vertex : m_vertex)
            delete vertex;

        // Copy new data
        m_vertex = {
            other.m_vertex[0] == nullptr ? nullptr : new Point(*other.m_vertex[0]),
            other.m_vertex[1] == nullptr ? nullptr : new Point(*other.m_vertex[1]),
            other.m_vertex[2] == nullptr ? nullptr : new Point(*other.m_vertex[2]),
        };

        return *this;
    }


    /**
     * Translate all vertices by #d units alongside the #axis direction
     * @param d Distance to translate by
     * @param axis Direction (x, y, or z) to translate in
     * @return 0 if all translations were successful, -1 if an error occurred
     */
    int translate(const float d, const char axis)
    {
        ensure_initialized();

        int res = 0;
        res |= m_vertex[0]->translate(d, axis);
        res |= m_vertex[1]->translate(d, axis);
        res |= m_vertex[2]->translate(d, axis);
        return res;
    }

    /**
     * Computes the area of the triangle. The triangle should be initialized before calling this method.
     * @return The computed area of the triangle
     */
    [[nodiscard]] float calcArea() const
    {
        ensure_initialized();

        const float a = m_vertex[0]->distance_to(*m_vertex[1]);
        const float b = m_vertex[0]->distance_to(*m_vertex[2]);
        const float c = m_vertex[1]->distance_to(*m_vertex[2]);
        const float s = (a + b + c) * 0.5f;

        // heron's formula
        return std::sqrt(s * (s - a) * (s - b) * (s - c));
    }


    /**
     * Explicit cast to string for display purposes
     */
    [[nodiscard]] explicit operator std::string() const
    {
        return std::format(
            "Triangle(\n\t{},\n\t{},\n\t{}\n)",
            m_vertex[0] == nullptr ? "nullptr" : static_cast<std::string>(*m_vertex[0]),
            m_vertex[1] == nullptr ? "nullptr" : static_cast<std::string>(*m_vertex[1]),
            m_vertex[2] == nullptr ? "nullptr" : static_cast<std::string>(*m_vertex[2])
        );
    }

    /**
     * Helper function to check if all pointers have been set
     * @return Whether the triangle is fully initialized or not
     */
    [[nodiscard]] bool is_initialized() const
    {
        return m_vertex[0] != nullptr && m_vertex[1] != nullptr && m_vertex[2] != nullptr;
    }

    /**
     * Helper function that throws if any of the vertices are null
     */
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
            enum class MenuOption : int
            {
                Quit = 0,
                Create = 1,
                Edit = 2,
                Delete = 3,
                Translate = 4,
                Display = 5
            };

            clear_screen();
            std::println("Triangle Manipulation Menu");
            std::println();
            print_triangles();
            std::println();
            std::println("Commands:");
            std::println("[1] Create triangle");
            std::println("[2] Edit triangle");
            std::println("[3] Delete triangle");
            std::println("[4] Translate triangle");
            std::println("[5] Display triangles");
            std::println();
            std::println("[0] Quit");
            std::println();
            const int option = prompt<int>("Select an option: ");

            clear_screen();

            switch (static_cast<MenuOption>(option))
            {
                using enum MenuOption;
            case Quit:
                return;
            case Create:
                m_triangles.emplace_back();
                break;
            case Edit:
                {
                    // Pick triangle
                    const auto triangle_idx = select_triangle();
                    if (triangle_idx == -1) break;

                    // Branch to submenu
                    run_edit_menu(m_triangles[triangle_idx]);
                    break;
                }
            case Delete:
                {
                    // Pick triangle
                    const auto triangle_idx = select_triangle();
                    if (triangle_idx == -1) break;

                    // Delete that triangle
                    m_triangles.erase(m_triangles.begin() + triangle_idx);
                    break;
                }
            case Translate:
                {
                    // Pick triangle
                    const auto triangle_idx = select_triangle(true);
                    if (triangle_idx == -1) break;

                    // Pick direction
                    char direction;
                    while (true)
                    {
                        direction = prompt<char>("Enter direction to translate in (x, y, or z): ");
                        // Validate the direction and try again if not valid
                        direction = static_cast<char>(tolower(direction));
                        if (direction != 'x' && direction != 'y' && direction != 'z')
                        {
                            std::println("Invalid direction");
                            continue;
                        }

                        break;
                    }

                    // Pick distance
                    const auto d = prompt<float>("Enter value to translate by: ");

                    // This should never happen as we validate the direction before, but if it does, we should still log it.
                    if (m_triangles[triangle_idx].translate(d, direction) == -1)
                    {
                        std::println("Error while translating triangle");
                        pause();
                    }

                    break;
                }
            case Display:
                // A bit of a useless command, but the assignment asks for it explicitly, so here it is
                print_triangles();
                pause();
                break;
            default:
                // Bad option, just try again
                std::println("Invalid Option ({})!", option);
                pause();
            }
        }
    }

private:
    /**
     * List of triangles currently managed by the driver class instance
     */
    std::vector<Triangle> m_triangles;

    /**
     * Prints a numbered one-indexed list of triangles with their values
     * @param only_initialized If only initialized triangles should be printed
     */
    void print_triangles(const bool only_initialized = false)
    {
        std::println("Triangles:");
        for (size_t idx = 1; auto& triangle : m_triangles)
        {
            if (!only_initialized || triangle.is_initialized())
            {
                std::println(
                    "[{}] {} area = {}", idx,
                    static_cast<std::string>(triangle),
                    triangle.is_initialized() ? std::to_string(triangle.calcArea()) : "???"

                );
            }

            idx++;
        }
    }

    /**
     * Helper function that prompts the user to type in a value then reads in the value
     * @tparam T Type of the value to read
     * @param prompt Message to prompt the user with. Should ideally end with ": "
     * @return The value inputted by the user
     */
    template <typename T>
    static T prompt(const char* prompt)
    {
        std::print("{}", prompt);

        T option;
        std::cin >> option;

        return option;
    }

    /**
     * Prompts the user to press enter then pauses until they do so
     */
    static void pause()
    {
        std::print("Press enter to continue...");
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    /**
     * Clears the screen. Depends on platform support for ANSI escape codes
     */
    static void clear_screen()
    {
        std::print("\033[2J\033[1;1H");
    }

    /**
     * Makes the user pick a triangle to proceed
     * @param require_initialized If only initialized triangles should be considered for selection
     * @return The 0 indexed index of the triangle, or -1 if the user canceled
     */
    int select_triangle(const bool require_initialized = false)
    {
        while (true)
        {
            print_triangles(require_initialized);
            std::println();
            std::println("[0] Cancel");
            std::println();
            // we do minus one as 0 is our "quit"
            const int triangle_idx = prompt<int>("Select a triangle (0 to cancel): ") - 1;

            // User canceled
            if (triangle_idx == -1) return -1;

            // Index out of range
            if (triangle_idx < 0 || triangle_idx >= m_triangles.size())
            {
                std::println("Invalid triangle index");
                continue;
            }

            // Check for init status if required by param
            if (require_initialized && !m_triangles[triangle_idx].is_initialized())
            {
                std::println("Selected triangle is not initialized");
                continue;
            }

            return triangle_idx;
        }
    }

    /**
     * Edits a specific triangle's vertices
     * @param triangle Triangle to edit
     */
    static void run_edit_menu(Triangle& triangle)
    {
        while (true)
        {
            // First, print all points
            std::println("Points: ");
            for (size_t idx = 1; const auto& point : triangle.m_vertex)
            {
                std::println(
                    "[{}] {}",
                    idx,
                    point == nullptr ? "nullptr" : static_cast<std::string>(*point)
                );
                idx++;
            }
            std::println();
            std::println("[0] Done");
            std::println();

            // Make the user pick one
            int point_idx;
            while (true)
            {
                point_idx = prompt<int>("Select a point (0 when done): ") - 1;
                // They're done, let's return
                if (point_idx == -1) return;
                // Index out of range
                if (point_idx < 0 || point_idx >= 3)
                {
                    std::println("Invalid point number");
                    continue;
                }
                break;
            }

            // Pick coordinate values. this step can't be canceled for sanity reasons
            auto& point = triangle.m_vertex[point_idx];
            const auto x = prompt<float>("Enter value for x: ");
            const auto y = prompt<float>("Enter value for y: ");
            const auto z = prompt<float>("Enter value for z: ");

            delete point;
            point = new Point(x, y, z);
        }
    }
};


int main()
{
    Driver().run_main_menu();

    return 0;
}
