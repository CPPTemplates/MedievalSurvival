namespace math
{
        //  Function to return gcd of a and b
    template <typename T>
    constexpr T gcd(const T &a, const T &b)
    {
        if (a == 0)
            return b;
        return gcd(b % a, a);
    }

    template <typename T>
    // Function for extended Euclidean Algorithm
    constexpr T gcdExtended(const T &a, const T &b, T &x, T &y)
    {

        // Base Case
        if (a == 0)
        {
            x = 0, y = 1;
            return b;
        }

        // To store results of recursive call
        //variables have to be initialized in constant expressions
        T x1 = 0, y1 = 0;
        T gcd = gcdExtended(b % a, a, x1, y1);

        // Update x and y using results of recursive
        // call
        x = y1 - (b / a) * x1;
        y = x1;

        return gcd;
    }
} // namespace math
