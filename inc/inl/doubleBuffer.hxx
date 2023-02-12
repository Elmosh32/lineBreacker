#ifndef DOUBLE_BUFFER_HXX
#define DOUBLE_BUFFER_HXX

#include <string>

#include "doubleBuffer.hpp"

template <typename T>
Pixel<T>::Pixel(T a_red, T a_green, T a_blue)
: m_red(a_red)
, m_green(a_green)
, m_blue(a_blue)
{};

template <typename T>
Pixel<T> &Pixel<T>::operator=(const Pixel &a_pixel)
{
    m_red = a_pixel.m_red;
    m_green = a_pixel.m_green;
    m_blue = a_pixel.m_blue;
    return *this;
}

template <typename T>
bool Pixel<T>::operator==(const Pixel &a_pixel)
{
    return (m_red == a_pixel.m_red && m_green == a_pixel.m_green && m_blue == a_pixel.m_blue);
}

template <typename T>
std::string Pixel<T>::getPixelColor()
{
    if (m_red == 255 && m_green == 255 && m_blue == 255) {
        return "white";
    } else if (m_red == 0 && m_green == 0 && m_blue == 0) {
        return "black";
    } else if (m_red == 0 && m_green == 0 && m_blue == 255) {
        return "blue";
    }else if (m_red == 0 && m_green == 255 && m_blue == 0) {
        return "green";
    } else if (m_red == 255 && m_green == 0 && m_blue == 0) {
        return "red";
    }
    
    return "no_color";
}

template <typename T>
DoubleBuffer<T>::DoubleBuffer(size_t a_width, size_t a_height, std::string a_type)
: m_width(a_width)
, m_height(a_height)
, m_front(new T[m_width * m_height])
, m_imageType(a_type)
{
    try
    {
        m_back = new T[m_width * m_height];
    }
    catch (std::bad_alloc const &x)
    {
        delete[] m_front;
        throw BufferException("Allocation error");
    }
}

template <typename T>
DoubleBuffer<T>::DoubleBuffer(const DoubleBuffer &a_copyBuffer)
{
    try
    {
        m_height = a_copyBuffer.m_height;
        m_width = a_copyBuffer.m_width;
        m_front = new T[m_width * m_height];
        m_back = new T[m_width * m_height];
        std::copy(a_copyBuffer.m_front, a_copyBuffer.m_front + (m_width * m_height), m_front);
        std::copy(a_copyBuffer.m_back, a_copyBuffer.m_back + (m_width * m_height), m_back);
    }
    catch (std::bad_alloc const &x)
    {
        throw BufferException("Allocation error");
    }
}

template <typename T>
DoubleBuffer<T> &DoubleBuffer<T>::operator=(const DoubleBuffer &a_copyBuffer)
{
    if (this != &a_copyBuffer)
    {
        m_height = a_copyBuffer.m_height;
        m_width = a_copyBuffer.m_width;
        auto back = new T[m_width * m_height];
        auto front = new T[m_width * m_height];
        std::copy(a_copyBuffer.m_front, a_copyBuffer.m_front + (m_width * m_height), front);
        std::copy(a_copyBuffer.m_back, a_copyBuffer.m_back + (m_width * m_height), back);

        delete[] m_front;
        delete[] m_back;

        m_front = front;
        m_back = back;
    }

    return *this;
}

template <typename T>
DoubleBuffer<T>::~DoubleBuffer()
{
    delete[] m_front;
    delete[] m_back;
}

template <typename T>
void DoubleBuffer<T>::switchSides()
{
    std::swap(m_front, m_back);
}

template <typename T>
void DoubleBuffer<T>::fill(const T &a_color, Side a_side)
{
    if (a_side == Side::Front)
    {
        std::fill(m_front, m_front + (m_width * m_height), a_color);
    }
    else
    {
        std::fill(m_back, m_back + (m_width * m_height), a_color);
    }
    
}

template <typename T>
void DoubleBuffer<T>::clear(Side a_side)
{
    fill({}, a_side);
}

template <typename T>
void DoubleBuffer<T>::drawFilledCircle(T a_color, Point a_point, size_t a_radius, Side a_side)
{
    for (int rad = a_radius; rad >= 0; rad--)
    {

        for (double i = 0; i <= M_PI * 2; i += 0.001)
        {

            int pX = a_point.m_x + rad * std::cos(i);
            int pY = a_point.m_y + rad * std::sin(i);
            Point p1(pX, pY);

            drawPixel(a_color, p1, a_side);
        }
    }
      
}
template <typename T>
void DoubleBuffer<T>::drawCircle(T a_color, Point a_point, size_t a_radius, Side a_side)
{
    if (inBounds(a_point)){
        int x = a_radius, y = 0;
        int radiusError = 1 - x;

        while (x >= y)
        {
            Point p1(x + a_point.m_x, y + a_point.m_y);
            drawPixel(a_color, p1, a_side);

            Point p2(y + a_point.m_x, x + a_point.m_y);
            drawPixel(a_color, p2, a_side);

            Point p3(-x + a_point.m_x, y + a_point.m_y);
            drawPixel(a_color, p3, a_side);

            Point p4(-y + a_point.m_x, x + a_point.m_y);
            drawPixel(a_color, p4, a_side);

            Point p5(-x + a_point.m_x, -y + a_point.m_y);
            drawPixel(a_color, p5, a_side);

            Point p6(-y + a_point.m_x, -x + a_point.m_y);
            drawPixel(a_color, p6, a_side);

            Point p7(x + a_point.m_x, -y + a_point.m_y);
            drawPixel(a_color, p7, a_side);

            Point p8(y + a_point.m_x, -x + a_point.m_y);
            drawPixel(a_color, p8, a_side);

            y++;
            if (radiusError < 0)
                radiusError += 2 * y + 1;
            else
            {
                x--;
                radiusError += 2 * (y - x + 1);
            }
        }
    }
}

template <typename T>
void DoubleBuffer<T>::drawFilledBox(T a_color, Point a_firstPoint, Point a_secondPoint, Side a_side)
{
    if (inBounds(a_firstPoint) && inBounds(a_secondPoint))
    {
        if (a_firstPoint.m_x != a_secondPoint.m_x && a_firstPoint.m_y != a_secondPoint.m_y)
        {
            auto minX = std::min(a_firstPoint.m_x, a_secondPoint.m_x);
            auto maxX = std::max(a_firstPoint.m_x, a_secondPoint.m_x);

            auto minY = std::min(a_firstPoint.m_y, a_secondPoint.m_y);
            auto maxY = std::max(a_firstPoint.m_y, a_secondPoint.m_y);
            for (int i = minX; i <= maxX; i++)
            {
                Point p1(i, minY);
                Point p2(i, maxY);

                drawLine(a_color, p1, p2, a_side); 
            }
        }
    }
}

template <typename T>
void DoubleBuffer<T>::drawBox(T a_color, Point a_firstPoint, Point a_secondPoint, Side a_side)
{
    if (inBounds(a_firstPoint) && inBounds(a_secondPoint))
    {
        if (a_firstPoint.m_x != a_secondPoint.m_x && a_firstPoint.m_y != a_secondPoint.m_y)
        {
            Point p1(a_firstPoint.m_x, a_secondPoint.m_y);
            Point p2(a_secondPoint.m_x, a_firstPoint.m_y);

            drawLine(a_color, a_firstPoint, p2, a_side);
            drawLine(a_color, a_firstPoint, p1, a_side);
            drawLine(a_color, a_secondPoint, p1, a_side);
            drawLine(a_color, a_secondPoint, p2, a_side);
        }
    }
}

template <typename T>
void DoubleBuffer<T>::drawSmile(T a_color, Point a_firstPoint, Point a_secondPoint, Side a_side)
{
    if (inBounds(a_firstPoint) && inBounds(a_secondPoint))
    {
        if (a_firstPoint.m_x == a_secondPoint.m_x && a_firstPoint.m_y != a_secondPoint.m_y)
        {
            auto width = static_cast<size_t>(std::abs(static_cast<int>(a_firstPoint.m_y - a_secondPoint.m_y)));
          
            auto minY = std::min(a_firstPoint.m_y, a_secondPoint.m_y);
            auto maxY = std::max(a_firstPoint.m_y, a_secondPoint.m_y);

            auto x1 = a_firstPoint.m_x;

            for (size_t i = 0; i < width/2; i++){
                Point p1(x1, minY);
                Point p2(x1, minY + minY/30);
                drawLine(a_color, p1, p2, a_side);

                Point p3(x1, maxY);
                Point p4(x1, maxY - minY/30);
                drawLine(a_color, p3, p4, a_side);

                if (i % 5 == 0){
                    x1++;
                }

                minY++;
                maxY--;
            }
        }
    }
}

template <typename T>
void DoubleBuffer<T>::drawLine(T a_color, Point a_firstPoint, Point a_secondPoint, Side a_side)
{
    if (inBounds(a_firstPoint) && inBounds(a_secondPoint)){
        if (a_firstPoint.m_x == a_secondPoint.m_x && a_firstPoint.m_y != a_secondPoint.m_y){
            drawRow(a_color, a_firstPoint, a_secondPoint, a_side);
        } else if (a_firstPoint.m_y == a_secondPoint.m_y && a_firstPoint.m_x != a_secondPoint.m_x){
            drawColumn(a_color, a_firstPoint, a_secondPoint, a_side);
        } else if (a_secondPoint.m_x - a_firstPoint.m_x == a_firstPoint.m_y - a_secondPoint.m_y){
            drawDiagonalRightToLeft(a_color, a_firstPoint, a_secondPoint, a_side);
        } else if (a_firstPoint.m_x - a_secondPoint.m_x == a_firstPoint.m_y - a_secondPoint.m_y){
            drawDiagonalLeftToRight(a_color, a_firstPoint, a_secondPoint, a_side);
        } else if (a_firstPoint.m_y == a_secondPoint.m_y && a_firstPoint.m_x == a_secondPoint.m_x){
            drawPixel(a_color, a_firstPoint, a_side);
        }
    }
}

template <typename T>
void DoubleBuffer<T>::drawDiagonalRightToLeft(T a_color, Point a_firstPoint, Point a_secondPoint, Side a_side)
{
    auto minX = std::min(a_secondPoint.m_x, a_firstPoint.m_x);
    auto maxY = std::max(a_secondPoint.m_y, a_firstPoint.m_y);
    auto end = std::abs(static_cast<int>(a_firstPoint.m_x - a_secondPoint.m_x));
    auto counter = 0;

    while (counter != end + 1)
    {
        Point p(minX, maxY);
        drawPixel(a_color, p, a_side);
        minX++;
        maxY--;
        counter++;
    }
}

template <typename T>
void DoubleBuffer<T>::drawDiagonalLeftToRight(T a_color, Point a_firstPoint, Point a_secondPoint, Side a_side)
{
    auto minX = std::min(a_secondPoint.m_x, a_firstPoint.m_x);
    auto minY = std::min(a_secondPoint.m_y, a_firstPoint.m_y);
    auto end = std::abs(static_cast<int>(a_firstPoint.m_x - a_secondPoint.m_x));
    auto counter = 0;

    while (counter != end + 1)
    {
        Point p(minX, minY);
        drawPixel(a_color, p, a_side);
        minX++;
        minY++;
        counter++;
    }
}

template <typename T>
void DoubleBuffer<T>::drawColumn(T a_color, Point a_firstPoint, Point a_secondPoint, Side a_side)
{
    size_t minX = std::min(a_secondPoint.m_x, a_firstPoint.m_x);
    size_t maxX = std::max(a_secondPoint.m_x, a_firstPoint.m_x);
    Point p(minX, a_firstPoint.m_y);
    while (minX <= maxX)
    {
        drawPixel(a_color, p, a_side);
        minX++;
        p.m_x = minX;
    }
}

template <typename T>
void DoubleBuffer<T>::drawRow(T a_color, Point a_firstPoint, Point a_secondPoint, Side a_side)
{
    auto minY = std::min(a_secondPoint.m_y, a_firstPoint.m_y);
    auto maxY = std::max(a_secondPoint.m_y, a_firstPoint.m_y);
    Point p(a_firstPoint.m_x, minY);

    while (minY <= maxY)
    {
        drawPixel(a_color, p, a_side);
        minY++;
        p.m_y = minY;
    }
}

template <typename T>
void DoubleBuffer<T>::drawPixel(T a_color, Point a_point, Side a_side)
{
    if (inBounds(a_point))
    {
        auto index = (a_point.m_x * m_width) + a_point.m_y;

        getSide(a_side)[index] = a_color;

    }
}

// template <typename T>
// void DoubleBuffer<T>::setPixel(int a_x, int a_y, T a_color, Side a_side)
// {
//     if (a_side == Side::Front)
//     {
//         m_front[a_x * m_height + a_y] = a_color;
//     }
//     else if (a_side == Side::Back)
//     {
//         m_back[a_x * m_height + a_y] = a_color;
//         // std::cout << (int)m_back[(a_x * m_width) + a_y] << "here\n";
//     }
// }

template <typename T>
void DoubleBuffer<T>::saveFile(int a_version)
{
    if (m_imageType == "color")
    {
        saveColoredImage(a_version);
    }
    else if (m_imageType == "gray")
    {
        saveGrayImage(a_version);
    }
    else{
        saveBWImage(a_version);
    }
   
}

template <typename T>
void DoubleBuffer<T>::saveColoredImage(int a_version)
{
    FILE *fp;

    std::string version = std::to_string(a_version);
    version.insert(0, 4 - version.length(), '0');
    auto fileName = "images/life." + version + ".ppm";
    const char *file = fileName.c_str();
    fp = std::fopen(file, "w");
    std::fprintf(fp, "P3\n");
    std::fprintf(fp, "%d %d\n", m_width, m_height);
    std::fprintf(fp, "%d\n", 255);

    for (int i = 0; i < m_width * m_height; i++)
    {
        std::fprintf(fp, "%d %d %d ", m_front[i].m_red, m_front[i].m_green, m_front[i].m_blue);
        if ((i + 1) % m_width == 0){
            std::fprintf(fp, "\n");
        }
    }
    fclose(fp);
}

template <typename T>
void DoubleBuffer<T>::saveGrayImage(int a_version)
{
    FILE *fp;

    std::string version = std::to_string(a_version);
    version.insert(0, 4 - version.length(), '0');
    auto fileName = "images/life." + version + ".bpm";
    const char *file = fileName.c_str();
    fp = std::fopen(file, "w");
    std::fprintf(fp, "P2\n");
    std::fprintf(fp, "%ld %ld\n", m_width, m_height);

    for (size_t i = 0; i < m_width * m_height; i++)
    {
        std::fprintf(fp, "%d ", m_front[i]);
        if ((i + 1) % m_width == 0)
        {
            std::fprintf(fp, "\n");
        }
        
    }
    fclose(fp);
}

template <typename T>
void DoubleBuffer<T>::saveBWImage(int a_version)
{
    std::string version = std::to_string(a_version);
    version.insert(0, 4 - version.length(), '0');
    auto fileName = "images/life." + version + ".bpm";

    FILE *fp;

    const char *file = fileName.c_str();
    fp = std::fopen(file, "w");
    std::fprintf(fp, "P1\n");
    std::fprintf(fp, "%d %d\n", m_width, m_height);

    for (int i = 0; i < m_width * m_height; i++)
    {
        std::fprintf(fp, "%d ", m_front[i]);
        if ((i + 1) % m_width == 0){
            std::fprintf(fp, "\n");
        }
    }
    fclose(fp);
}

template <typename T>
bool DoubleBuffer<T>::inBounds(Point a_point)
{
    if (a_point.m_x < m_width && a_point.m_y < m_height && a_point.m_x >= 0 && a_point.m_y >= 0)
    {
        return true;
    }

    return false;
}

template <typename T>
T DoubleBuffer<T>::getPixel(int a_x, int a_y, Side a_side)
{
    if (a_side == Side::Front){
        return m_front[a_x * m_height + a_y];
    }

    return m_back[a_x * m_height + a_y];
}

template <typename T>
T DoubleBuffer<T>::getPixel(int a_x, int a_y)
{
    return m_front[a_x * m_height + a_y];
}



template <typename T>
T *DoubleBuffer<T>::getSide(Side a_side) const
{
    if(a_side == Side::Front){
        return m_front;
    } else if (a_side == Side::Back){
        return m_back;
    }

    return {};
}

template <typename T>
size_t DoubleBuffer<T>::getHeight() const
{
    return m_height;
}

template <typename T>
size_t DoubleBuffer<T>::getWidth() const
{
    return m_width;
}

template <typename T>
void DoubleBuffer<T>::printAllBuffers()
{
    printBuffer(Side::Front);
    printBuffer(Side::Back);   
}

template <typename T>
void DoubleBuffer<T>::printBuffer(Side a_side)
{
    for (int i = 0; i < m_width * m_height; i++)
    {
        std::cout << getSide(a_side)[i] << " ";
        if ((i + 1) % m_width == 0)
        {
            std::cout << "\n";
        }
    }
    std::cout << "\n";
}

#endif /* DOUBLE_BUFFER_HXX */
