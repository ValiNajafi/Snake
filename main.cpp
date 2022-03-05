#include "raylib.h"
#include <vector>
#include <time.h>
#include <iostream>
#include <string>

const int screen_width = 800;
const int screen_height = 600;
const int block_size = 20;
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

		DrawRectangle(start_x, start_y, size, size, Fade(BLUE, 0.5f));
	}
	void drawhead(int speed_x, int speed_y)
		{
			auto start_x {x * block_size + pad};
			auto start_y {y * block_size + pad};

			auto size {block_size - 2*pad};
			auto Headcenter_x {start_x+size/2};
			auto Headcenter_y {start_y+size/2};
			auto Headradius {block_size/2};
			auto eye1_x  {Headcenter_x - Headradius/2*speed_y};
			auto eye1_y  {Headcenter_y + Headradius/2*speed_x};
			auto eye2_x  {Headcenter_x + Headradius/2*speed_y};
			auto eye2_y  {Headcenter_y - Headradius/2*speed_x};
			auto eye_radius {Headradius/5};
			DrawCircle(Headcenter_x, Headcenter_y, Headradius, Fade(GREEN, 0.7f));
			DrawCircle(eye1_x, eye1_y, eye_radius, Fade(RED, 0.9f));
			DrawCircle(eye2_x, eye2_y, eye_radius, Fade(BLUE, 0.9f));
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
	int speed_x;
	int speed_y;

	void draw()
	{
		for (unsigned int i = 0; i < segments.size()-1; ++i) {
			segments[i].draw();
		}
		segments[segments.size()-1].drawhead(speed_x, speed_y);
	}
	SnakeSegment head()
	{
		return segments[segments.size()-1];
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

	void do_move()
	{
		move(speed_x, speed_y);
	}

	void process_key()
	{
		if (IsKeyPressed(KEY_DOWN) && speed_y==0)
		{
			speed_y = 1;
			speed_x = 0;
		} else if (IsKeyPressed(KEY_UP) && speed_y ==0)
		{
			speed_y = -1;
			speed_x = 0;
		} else if (IsKeyPressed(KEY_LEFT) && speed_x ==0)
		{
			speed_y = 0;
			speed_x = -1;
		} else if (IsKeyPressed(KEY_RIGHT) && speed_x == 0)
		{
			speed_y = 0;
			speed_x = 1;
		}
	}

	void grow(int num_segment)
	{
		auto head_index {segments.size() - 1};

		SnakeSegment new_segment{ segments[head_index].x, segments[head_index].y};

		for (int i = 0; i < num_segment; ++i) {
			segments.push_back(new_segment);
		}
	}
};
struct food_block
{
	int x;
	int y;

	void draw()//(Color color)
	{
		auto center_x {x * block_size + block_size/2};
		auto center_y {y * block_size + block_size/2};

		auto radius {block_size / 2};

		DrawCircle(center_x, center_y, radius, Fade(RED, 0.9f));
	}
	void renew()
	{
		x = rand()%(screen_width/block_size);
		y = rand()%(screen_height/block_size);
	}
};
bool colision(SnakeSegment snake_head, food_block food)
{
	if ((snake_head.x == food.x) and (snake_head.y == food.y))
		return true;
	else
		return false;
};
bool self_colision(Snake snake)
{
	bool colid {false};
	for (unsigned int i = 0; i < snake.segments.size()-1; ++i) {
		if ((snake.head().x == snake.segments[i].x) and (snake.head().y == snake.segments[i].y)){
			colid = true;
		}
	}
	return colid;
};
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

	Snake my_snake {{{5,5}}, 1, 0};
	my_snake.grow(3);
	srand(time(0));
	std::cout << rand() << "\n";
	food_block food {rand()%(screen_width/block_size), rand()%(screen_height/block_size)};
	int score=0;
//	food.draw(); //wrong place for drawing!!!!

    InitWindow(screen_width, screen_height, "Snake!");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    int frame_cnt = 0;
    int game_cnt = 0;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

    	frame_cnt++;

    	if (IsKeyPressed(KEY_Q))
    	{
    		my_snake.grow(1);
    	} else if (IsKeyPressed(KEY_W))
    	{
    		my_snake.grow(5);
    	}

    	my_snake.process_key();
    	if (frame_cnt >=15)
    	{

			if ((my_snake.segments[my_snake.segments.size()-1].x < 0) //kept to show complexity
				or (my_snake.head().y < 0)
				or (my_snake.head().x > screen_width/block_size-1)
				or (my_snake.head().y > screen_height/block_size-1)
				//or (self_colision(my_snake))
				)
			{
				DrawText("Game Over, Press SPACE", 50 , screen_height - 200, 50, BLACK);
				game_cnt++;
				if (game_cnt>=200)
					break;
				else if (IsKeyDown(KEY_SPACE))
				{
					my_snake.segments= {{5,5}};
					my_snake.speed_x=1;
					my_snake.speed_y=0;
					my_snake.grow(3);
				}
			} else
			{
				my_snake.do_move();
				frame_cnt = 0;
			//	DrawText(std::to_string(score), 50 , screen_height - 50, 10, BLACK);
				if (colision(my_snake.head(), food))
				{
					my_snake.grow(1);
					food.renew();
					score +=100;
				};

			}

    	}
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            my_snake.draw();
            food.draw();


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
