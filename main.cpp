#include "raylib.h"
#include <vector>

const int screen_width = 800;
const int screen_height = 600;
const int block_size = 50;
const int pad = 1;

struct SnakeSegment
{
	int x;
	int y;


	void draw()
	{
		auto start_x {x * block_size + pad};
		auto start_y {y * block_size + pad};

		auto size {block_size - 2*pad};

		DrawRectangle(start_x, start_y, size, size, BLUE);
	}
};

struct Snake
{
	std::vector<SnakeSegment> segments;

	void draw()
	{
		for (unsigned int i = 0; i < segments.size(); ++i) {
			segments[i].draw();
		}
	}
};

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

	Snake my_snake {{ {1,2}, {2,2} , {3,2} , {3,3} }};

    InitWindow(screen_width, screen_height, "Snake!");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            my_snake.draw();


            //DrawText("Snake!", 50 , screen_height - 60, 40, LIGHTGRAY);

            //DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
