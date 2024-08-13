#ifndef PERLIN_H
#define PERLIN_H

// Define vector2 struct (tuple/coordinate)
typedef struct 
{
    float x, y;
} vector2;

// Random seed
extern int seed;

// Function prototypes, perlin.c
int noise(int x, int y);
double interpolate(double a, double b, double weight);
double linear_interpolation(double a, double b, double weight);
double cubic_interpolation(double a, double b, double weight);
vector2 random_gradient(int input_x, int input_y);
double dot_grid_gradient(int input_x, int input_y, double x, double y);
double perlin_noise(double x, double y);
double perlin(int x, int y, double frequency, int depth);

// Function prototypes, functions.c
char ascii_filter(double value);
//void generate_perlin_noise_map(MAP * map);


#endif