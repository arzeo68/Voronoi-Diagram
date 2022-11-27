#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define WIDTH 1000
#define HEIGHT 1000
#define MAX WIDTH * HEIGHT

// color 0xAABBGGRR
#define BLUE    0xFFFF0000
#define RED     0xFF0000FF
#define GREEN   0xFF00FF00
#define CYAN    0xFFFFFF00
#define YELLOW  0xFF00FFFF
#define BLACK   0xFF000000
#define WHITE   0xFFFFFFFF

#define GRUVBOX_BRIGHT_RED     0xFF3449FB
#define GRUVBOX_BRIGHT_GREEN   0xFF26BBB8
#define GRUVBOX_BRIGHT_YELLOW  0xFF2FBDFA
#define GRUVBOX_BRIGHT_BLUE    0xFF98A583
#define GRUVBOX_BRIGHT_PURPLE  0xFF9B86D3
#define GRUVBOX_BRIGHT_AQUA    0xFF7CC08E
#define GRUVBOX_BRIGHT_ORANGE  0xFF1980FE

#define COLOR uint32_t

typedef struct {
    int x;
    int y;
} SEED;


#define SEED_COUNT 25
#define COLOR_COUNT 7

COLOR colors[COLOR_COUNT] = {
    GRUVBOX_BRIGHT_RED,
    GRUVBOX_BRIGHT_GREEN,
    GRUVBOX_BRIGHT_YELLOW,
    GRUVBOX_BRIGHT_BLUE,
    GRUVBOX_BRIGHT_PURPLE,
    GRUVBOX_BRIGHT_AQUA,
    GRUVBOX_BRIGHT_ORANGE
};
COLOR image[HEIGHT][WIDTH];
SEED seeds[SEED_COUNT];


// MATH
double distance(int xA, int yA, int xB, int yB) {
    return sqrt(pow(xB - xA, 2) + pow(yB - yA, 2));
}

// DRAW
void draw_circle(int x, int y, int radius, COLOR color) {
    for (size_t i = y - radius; i < y + radius; i++)
        for (size_t j = x - radius; j < x + radius; j++)
            if (distance(x,y,j,i) <= radius && i < HEIGHT && i >= 0 && j < WIDTH && j >= 0)
                image[i][j] = color;
}

void drawSeed() {
    for (size_t i = 0; i < SEED_COUNT; i++)
        draw_circle(seeds[i].x, seeds[i].y, 10, WHITE);
}

static void fill(COLOR color) {
    for (size_t y = 0; y < HEIGHT; ++y)
        for (size_t x = 0; x < HEIGHT; ++x)
            image[y][x] = color;
} 

static void save_to_ppm(const char *filepath) {
  int i, j;
  FILE *fp = fopen(filepath, "wb"); /* b - binary mode */
  (void) fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
  for (j = 0; j < HEIGHT; ++j)
    for (i = 0; i < WIDTH; ++i) {
      static unsigned char color[3];
        color[0] = image[j][i];
        color[1] = (image[j][i] >> 8);
        color[2] = (image[j][i] >> 16);
      (void)fwrite(color, 1, 3, fp);
    }
  (void)fclose(fp);
}

// VORODOI
SEED setupSeeds() {
    const int r = 10;
    for (size_t i = 0; i < SEED_COUNT; i++)
    {
        seeds[i].x = (rand() % (WIDTH - r)) + r;
        seeds[i].y = (rand() % (HEIGHT - r)) + r;
    }
}

static void firstImpl() {
 for (int j = 0; j < HEIGHT; ++j)
    for (int i = 0; i < WIDTH; ++i) {
        double currentMaxDist = MAX;
        int minIdx = 0;
        for (size_t s = 0; s < SEED_COUNT; s++)
        { 
            double dist = 0;
            if ( (dist = distance(i,j,seeds[s].x, seeds[s].y)) < currentMaxDist) {
                currentMaxDist = dist;
                minIdx = s;
            }
        }
        image[j][i] = colors[minIdx % COLOR_COUNT];
        
    }
}

int main() {
    srand(time(NULL));
    fill(BLACK);
    setupSeeds();
    firstImpl();
    drawSeed();
    save_to_ppm("./out.ppm");
}