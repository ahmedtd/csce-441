
class color
{
public:
    color(double r, double g, double b, double a);

    double r() const;
    double g() const;
    double b() const;
    double a() const;
private:
    double [4] mComponents;
};
