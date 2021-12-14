#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>

#define SIZE 60
// Major radius
#define R 20
// Minor radius
#define r 10
// Frames per second
#define FPS 60
// Rotation speed in rad/s
#define XSPEED 15.0
#define YSPEED 10.0

void printDonut(int donut[SIZE][SIZE])
{
    char c[] = "$@B%8&WM*ohkbdpqwmZO0QLCJUYXzvnxjft|()1}]?-_+~<>i!lI;:,^`'.";
    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            if (donut[x][y] != SIZE)
            {
                std::cout << c[donut[x][y] * 60 / SIZE];
            }
            else {
                std::cout << ' ';
            }
        }
        if(x != SIZE - 1) std::cout << '\n';
    }
    std::cout.flush();
}

void clearBuf(int buf[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            buf[i][j] = SIZE;
        }
    }
}

bool isInDonut(int x, int y, int z)
{
    return (sqrt(x * x + y * y) - R) * (sqrt(x * x + y * y) - R) + z * z <= r*r;
}

int getX(int x, int y, int z, float phiX, float phiY)
{
    x -= SIZE / 2;
    y -= SIZE / 2;
    z -= SIZE / 2;
    return x*cos(phiY) - y*sin(phiX)*sin(phiY) - z*cos(phiX)*sin(phiY) + SIZE / 2;
}

int getY(int x, int y, int z, float phiX, float phiY)
{
    x -= SIZE / 2;
    y -= SIZE / 2;
    z -= SIZE / 2;
    return y*cos(phiX) - z*sin(phiX) + SIZE / 2;
}

int getZ(int x, int y, int z, float phiX, float phiY)
{
    x -= SIZE / 2;
    y -= SIZE / 2;
    z -= SIZE / 2;
    return x * sin(phiY) + y*sin(phiX)*cos(phiY) + z*cos(phiX)*cos(phiY) + SIZE / 2;
}

int main()
{
    char stdinBuf[SIZE * SIZE];
    std::setvbuf(stdout, stdinBuf, _IOFBF, sizeof stdinBuf);
    int buf[SIZE][SIZE];
    clearBuf(buf);
    float phiX = 0.0f;
    float phiY = 0.0f;
    while (true)
    {
        for (int x = 0; x < SIZE; x++)
        {
            for (int y = 0; y < SIZE; y++)
            {
                for (int z = 0; z < SIZE; z++)
                {
                    if (isInDonut(x - SIZE / 2, y - SIZE / 2, z - SIZE / 2)) 
                    {
                        int x1 = getX(x, y, z, phiX, phiY);
                        int y1 = getY(x, y, z, phiX, phiY);
                        int z1 = getZ(x, y, z, phiX, phiY);
                        buf[x1][y1] = std::min(z1, buf[x1][y1]);
                    }
                }
            }
        }
        
        phiX += XSPEED / FPS;
        phiY += YSPEED / FPS;
        printDonut(buf);
        clearBuf(buf);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS));
        printf("\033[%d;%dH", 0, 0);
    }
}