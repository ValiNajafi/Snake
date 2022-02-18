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

		DrawRectangle(start_x, start_y, size, size, Fade(BLUE, 0.3f));
	}

	void move(int dx, int dy)
	{
		x += dx;
		y += dy;
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

	void move(int dx, int dy)
	{
		auto size {segments.size()};

		for (unsigned int i = 0; i < (size-1); ++i) {
			segments[i].x = segments[i+1].x;
			segments[i].y = segments[i+1].y;
		}

		segments[size-1].move(dx, dy);
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

    	if (IsKeyPressed(KEY_DOWN))
    	{
    		my_snake.move(0, 1);
    	} else if (IsKeyPressed(KEY_UP))
    	{
    		my_snake.move(0, -1);
    	} else if (IsKeyPressed(KEY_LEFT))
    	{
    		my_snake.move(-1, 0);
    	} else if (IsKeyPressed(KEY_RIGHT))
    	{
    		my_snake.move(1, 0);
    	}

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
